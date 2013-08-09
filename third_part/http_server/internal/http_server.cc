// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#include "third_part/http_server/public/http_server.h"

#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


#include <vector>
#include <algorithm>

#include "base/public/logging.h"
#include "base/public/net.h"
#include "base/public/string_util.h"
#include "file/public/file.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/file_monitor/public/monitor_handler.h"
#include "third_part/crypto/public/base64.h"
#include "third_part/url_util/public/url_util.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT


DEFINE_bool(use_select, false, "");
DEFINE_string(http_server_auth_file,
              "basic_auth.passwd",
              "Auth file path for httpserver (relative to yrdata)");

namespace {
// this value is used to set epoll size when epoll initiallize
//  NOTE: according to unix manual--
//          The size is not the maximum size of the backing store
//          but just a hint to the kernel about how to dimension
//          internal structures.
const int kMaxEpollSize = 10000;
}  // namespace

namespace util {
namespace http_server {

string Request::GetBodyContent() const {
  return body_content_;
}

string Request::GetHeaderValue(const string& head_name) const {
  map<string, string>::const_iterator it = header_.find(head_name);
  return it != header_.end() ? it->second : "";
}

string Request::GetUrlParamValue(const string& param_name) const {
  map<string, string>::const_iterator it = url_params_.find(param_name);
  return it != url_params_.end() ? it->second : "";
}

string Request::GetPostParamValue(const string& param_name) const {
  map<string, string>::const_iterator it = post_params_.find(param_name);
  return it != post_params_.end() ? it->second : "";
}

Respond::Respond() {
  status_ = 200;
  AddHeader("Content-Type", "text/html; charset=utf-8");
  // http://www.w3.org/Protocols/rfc2616/rfc2616-sec3.html#sec3.12
  AddHeader("Accept-Ranges", "bytes");
  AddHeader("Access-Control-Allow-Origin", "*");
}

void Respond::SetStatus(const int32 &status) {
  status_ = status;
}

void Respond::AddContent(const string &info) {
  content_.append(info);
}

void Respond::AddContent(const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  StringAppendV(&content_, format, ap);
  va_end(ap);
}

void Respond::AddHeader(const string &key, const string &value) {
  header_[key] = value;
}

void Respond::AddHeader(const string &key, const char *value_format, ...) {
  va_list ap;
  va_start(ap, value_format);
  string value = StringPrintV(value_format, ap);
  va_end(ap);
  header_[key] = value;
}

string Respond::FlushToString() {
  AddHeader("Content-Length", "%u", content_.size());
  string result;
  result.reserve(4096);
  std::string http_status = "HTTP/1.1 ";
  http_status += IntToString(status_);
  if (status_ == 302)
    http_status += " MOVED_TEMPORARILY\r\n";
  else
    http_status += " OK\r\n";
  result.append(http_status);
  // header
  for (map<string, string>::iterator it = header_.begin();
       it != header_.end(); ++it) {
    StringAppendF(&result, "%s: %s\r\n", it->first.c_str(), it->second.c_str());
  }
  result.append("\r\n");
  // content
  result.append(content_);
  return result;
}

HttpServer::HttpServer(uint32_t port,
                       uint32_t interval_ms,
                       bool require_auth) : Thread(true) {
  Init(port, interval_ms, require_auth);
}

void HttpServer::Init(uint32_t port,
                      uint32_t interval_ms,
                      bool require_auth) {
  dft_handler_ = NULL;
  port_ = port;
  interval_ms_ = interval_ms;
  stop_ = false;
  wait_sec_ = 0;
  require_auth_ = require_auth;

  recv_buf_ = new char[kRecvBufLen];
  recv_buf_[kRecvBufLen - 1] = 0;
  // http://hi.baidu.com/syjfd/item/90f6251c2f29537f7a5f25ef
  // when the server recv and sending, if the client close
  // SIGPIPE will be catched by the server recv/send process. The defautl
  // operation of 'SIGPIPE' is terminating the recv/send process.
  // If set signal(SIGPIPE, SIG_IGN), the recv/send function will return -1 and
  // set ERRNO=EPIPE not terminate the process.
  signal(SIGPIPE, SIG_IGN);

  epoll_instance_ = epoll_create(kMaxEpollSize);
  CHECK_NE(epoll_instance_, -1);
  if (require_auth_) {
    util::FileMonitor* monitor = util::FileMonitor::GetInstance();
    Closure* callback = NewPermanentCallback(this, &HttpServer::LoadAuthFile);
    monitor->Register(FLAGS_http_server_auth_file, callback);
    LoadAuthFile();
  }
}

HttpServer::HttpServer(uint32_t port,
                       uint32_t interval_ms) : Thread(true) {
  Init(port, interval_ms, false);
}

HttpServer::~HttpServer() {
  close(epoll_instance_);
  delete [] recv_buf_;
  delete dft_handler_;
  gtl::STLDeleteValues(&handlers_);
}

void HttpServer::TimingStart(int wait_sec) {
  HERE(3);
  wait_sec_ = wait_sec;
  Start();
}

void HttpServer::Run() {
  HERE(3);
  sleep(wait_sec_);
  Init();  // virtual empty function.
  if (FLAGS_use_select) {
    RunWithSelect();
  } else {
    RunWithEpoll();
  }
}

void HttpServer::RunWithSelect() {
  CHECK_NE(port_, 0) << "port should not be 0";
  int ret = 0;
  int lis_fd = 0;

  lis_fd = TcpListen(port_, 5);
  CHECK_GE(lis_fd, 0)
    << StringPrintf("TcpListen failed, maybe port %d is wrong", port_);
  SetNonblocking(lis_fd);

  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1000;
  fd_set rfd;

  vector<int> req_queue;
  int work_fd = 0;
  while (!stop_) {
    int max_fd = lis_fd;
    FD_ZERO(&rfd);
    FD_SET(lis_fd, &rfd);
    for (vector<int>::iterator it = req_queue.begin();
         it != req_queue.end();
         ++it) {
      FD_SET(*it, &rfd);
      max_fd = max(max_fd, *it);
    }

    ret = NetSelect(max_fd + 1, &rfd, NULL, NULL, &timeout);
    if (ret < 0) {
      LOG(ERROR) << "NetSelect failed";
      continue;
    }
    //  check if there is a new request
    if (FD_ISSET(lis_fd, &rfd)) {
      work_fd = TcpAccept(lis_fd, NULL, NULL);
      if (work_fd < 0) {
        LOG(ERROR) << "TcpAccept failed";
        continue;
      }
      SetNonblocking(work_fd);
      req_queue.push_back(work_fd);
      VLOG(10) << "accept net socket";
    }

    //  handle request and close
    for (int i = req_queue.size() - 1; i >= 0; --i) {
      work_fd = req_queue[i];
      if (FD_ISSET(work_fd, &rfd)) {
        if (!HandleReq(work_fd)) {
          req_queue.erase(req_queue.begin() + i);
          close(work_fd);
        }
      }
    }
    usleep(1000*interval_ms_);
  }
  close(lis_fd);
}

void HttpServer::RunWithEpoll() {
  HERE(3);
  CHECK_NE(port_, 0) << "port is should not be 0";
  int lis_fd = 0;
  int num_ready_fds = 0;
  struct epoll_event lis_fd_event;
  memset(&lis_fd_event, 0, sizeof(struct epoll_event));
  struct epoll_event ready_events[kMaxEpollSize];

  lis_fd = TcpListen(port_, 5);
  CHECK_GE(lis_fd, 0)
    << StringPrintf("TcpListen failed, maybe port %d is wrong", port_);

  lis_fd_event.events = EPOLLIN;
  lis_fd_event.data.fd = lis_fd;

  SetNonblocking(lis_fd);

  if (epoll_ctl(epoll_instance_, EPOLL_CTL_ADD, lis_fd, &lis_fd_event) == -1) {
    LOG(FATAL) << "failed to listen socket:" << lis_fd
               << " error:" << strerror(errno);
    return;
  }

  int work_fd = 0;
  const int kWaitTimeout = 1000;
  while (!stop_) {
    memset(ready_events, 0, sizeof(struct epoll_event) * kMaxEpollSize);
    num_ready_fds =epoll_wait(epoll_instance_, ready_events,
                              kMaxEpollSize, kWaitTimeout);
    if (num_ready_fds < 0) {
      LOG(ERROR) << "epoll_wait failed:" << strerror(errno);
      continue;
    }

    for (int i = 0; i < num_ready_fds; ++i) {
      work_fd = ready_events[i].data.fd;
      if (ready_events[i].data.fd == lis_fd) {  // a new connection
        work_fd = TcpAccept(lis_fd, NULL, NULL);
        if (work_fd < 0) {
          LOG(ERROR) << "TcpAccept failed";
          continue;
        }
        SetNonblocking(work_fd);

        struct epoll_event fd_event;
        fd_event.events = EPOLLIN;
        fd_event.data.fd = work_fd;

        if (epoll_ctl(epoll_instance_, EPOLL_CTL_ADD, work_fd, &fd_event) < 0) {
          LOG(ERROR) << "failed to add socket[" << work_fd << "] to listen";
          continue;
        }
        VLOG(2) << "accept net socket:" << work_fd;
        continue;
      }

      if (!HandleReq(work_fd)) {
        epoll_ctl(epoll_instance_, EPOLL_CTL_DEL,
                  ready_events[i].data.fd, NULL);
        VLOG(3) << "closed fd:" << work_fd;
        close(work_fd);
      }
    }
  }
  close(lis_fd);
}

bool HttpServer::ParseRequest(char *request, int request_len,
                              int work_fd, Request *req_info) {
  char *url = NULL;
  char *url_param = NULL;
  char *header = NULL;
  char *content = NULL;
  if (0 == strncmp(request, "GET ", strlen("GET "))) {
    req_info->type_ = Request::kHttpGet;
    url = request + strlen("GET ");
  } else if (0 == strncmp(request, "POST ", strlen("POST "))) {
    req_info->type_ = Request::kHttpPost;
    url = request + strlen("POST ");
  } else {
    LOG(ERROR) << "http request format error, do not have 'GET' or 'POST' info";
    return false;
  }

  CHECK_LE(url, request + request_len);
  char *pos = strstr(request + 1, " HTTP");
  if (NULL == pos) {
    LOG(ERROR) << "http request format error, do not have 'HTTP' info"
               << "full content:\n" << request;
    return false;
  }
  CHECK_LE(pos, request + request_len);
  *pos = 0;
  url_param = strchr(url, '?');
  if (url_param) {
    *url_param++ = 0;
  }
  CHECK_LE(url_param, request + request_len);
  pos += strlen(" HTTP");
  CHECK_LE(pos, request + request_len);
  char *old_pos = pos;
  // check request is complete
  pos = strstr(pos, "\r\n\r\n");
  if (pos) {
    CHECK_LE(pos, request + request_len);
    content = pos + strlen("\r\n\r\n");
    CHECK_LE(content, request + request_len);
    // deal with header
    header = strstr(old_pos, "\r\n");
    if (header == pos) {
      header = NULL;
    } else {
      header += strlen("\r\n");
    }
    CHECK_LT(header, request + request_len);
    *pos = 0;
  } else {
    LOG(ERROR) << "http request format error, do not have '\\r\\n\\r\\n'";
    return false;
  }
  string log = StringPrintf("url[%s], url_param[%s], header[%s], content[%s]",
                            url,
                            url_param,
                            header,
                            content);
  VLOG(10) << log;
  // Get param
  if (url_param) {
    VLOG(10) << "parse get params: ";
     url_util::ParseKvlist(url_param, "=", '&',
                           &req_info->url_params_,
                           &req_info->url_param_vec_);
  }
  if (header) {
    VLOG(10) << "parse header params: ";
     url_util::ParseKvlist(header, ": ", '\n', &req_info->header_, NULL);
  }

  if (content) {
    VLOG(10) << "parse post params: ";
     url_util::ParseKvlist(content, "=", '&',
                           &req_info->post_params_,
                           &req_info->post_param_vec_);
    req_info->body_content_ = content;
  }
  TrimString(url, "/ ", &req_info->url_);
  return true;
}

bool HttpServer::BasicAuth(string auth_string) {
  const string basic_auth_method = "Basic ";
  if (!StartsWithASCII(auth_string, basic_auth_method, true)) {
    VLOG(3) << "Not found basic auth method in header";
    return false;
  }

  string base64string = auth_string.substr(basic_auth_method.length());

  string value;
  crypto::Base64Decode(base64string, &value);
  vector<string> user_pass_pair;
  SplitString(value, ':', &user_pass_pair);
  if (user_pass_pair.size() != 2) {
    return false;
  }
  if (user_passes_.size() == 0) {
    LOG(WARNING) << "Zero auth entry. basic auth disabled";
    return true;
  }
  for (int i = 0; i < user_passes_.size(); i++) {
    if (user_passes_[i].first != user_pass_pair[0]) {
      continue;
    } else {
      // http://blog.csdn.net/qdieyou/article/details/1569429
      string crypted_pass(crypt(user_pass_pair[1].c_str(),
                                user_passes_[i].second.c_str()));
      if (crypted_pass == user_passes_[i].second) {
        return true;
      } else {
        return false;
      }
    }
  }

  return false;
}

void HttpServer::LoadAuthFile() {
  string content;
  string path = FLAGS_http_server_auth_file;
  VLOG(2) << "Load auth file:" << path;
  user_passes_.clear();
  if (file::File::ReadFileToString(path, &content)) {
    vector<string> lines;
    SplitString(content, '\n', &lines);
    for (int i = 0; i < lines.size(); i++) {
      vector<string> user_pass;
      SplitString(lines[i], ':', &user_pass);
      if (user_pass.size() != 2) {
        continue;
      }
      user_passes_.push_back(pair<string, string>(user_pass[0], user_pass[1]));
    }
    if (user_passes_.size() == 0) {
      LOG(WARNING) << "Zero auth entry loaded from:" << path;
    }
  } else {
    LOG(ERROR) << "Load auth file failed: " << path;
  }
}

bool HttpServer::HandleWraper(const Request &request, Respond *respond) {
  bool auth_pass = false;
  if (!request.GetHeaderValue("Authorization").empty()) {
    VLOG(3) << "Receive Authorization header";
    auth_pass = BasicAuth(request.GetHeaderValue("Authorization"));
  }
  if (require_auth_) {
    if (!auth_pass) {
      respond->SetStatus(401);
      respond->AddHeader(
          "WWW-Authenticate", "Basic realm=\"Maintenance Code\"");
      VLOG(3) << "require basic auth, send back www-Authenticate header";
      return true;
    }
  }
  const string &url = request.url();
  StrToCallback::iterator it = handlers_.find(url);
  if (it != handlers_.end()) {
    return it->second->Run(request, respond);
  }
  if (dft_handler_) {
    return dft_handler_->Run(request, respond);
  }
  return true;
}

bool HttpServer::RecvRequest(int sock) {
  int total = 0;
  int read_num = 0;
  memset(recv_buf_, 0, kRecvBufLen);
  const char* get_signature = "GET ";
  const char* post_signature = "POST ";
  char* buf_index = recv_buf_;
  while (true) {
    read_num = TcpRecv(sock, buf_index, kRecvBufLen, 1000);
    if (read_num < 0) {
      LOG(ERROR) << "ERROR reading request";
      return false;
    }
    if (read_num == 0) {
      VLOG(10) << "client has close socket";
      return false;
    }
    total += read_num;
    recv_buf_[total]='\0';
    // must use this temp to prevent misleading compiling error
    int temp_kRecvBufLen = kRecvBufLen;
    CHECK_LT(total, temp_kRecvBufLen);
    char* index = strstr(recv_buf_, "\r\n\r\n");
    if (index != NULL) {
      if (strncmp(recv_buf_, get_signature, strlen(get_signature)) == 0) {
        //  GET
        VLOG(10) << "GET method,find end str...";
        break;
      } else if (strncmp(recv_buf_, post_signature,
                         strlen(post_signature)) == 0) {
        VLOG(10) << "POST method";
        // http://www.cppblog.com/range/archive/2011/10/09/157888.html
        char* start = strcasestr(recv_buf_,
                      "Content-Length:") + strlen("Content-Length:");
        CHECK_GE(start, recv_buf_);
        // http://baike.baidu.com/view/745156.htm
        char* end = strstr(start, "\r\n");
        CHECK(start != NULL);
        CHECK(end != NULL);

        string len(start, end);
        int length = StringToInt(len);
        VLOG(10) << "Content-Length:" << length;
        int real_len = strlen(index + 4);
        if (length == real_len) {
          VLOG(10) << "all data:" << recv_buf_;
          return true;
        } else {
          int left_len = length- real_len;
          if (total + left_len >= kRecvBufLen) {
            VLOG(10) << "not enough buffer";
            return false;
          }
          if (TcpRecvLen(sock, recv_buf_ + total, left_len, kRecvTimeout) < 0) {
            return false;
          } else {
            VLOG(10) << "post content:" << recv_buf_;
            return true;
          }
        }
      } else {
        LOG(ERROR) << "not support http method:"
                   << string(recv_buf_).substr(0, 10);
        return false;
      }
    } else {
      VLOG(10) << "can not find end of headers, continue to reveive";
      buf_index = recv_buf_ + total;
      continue;
    }
  }
  recv_buf_[total] = '\0';
  VLOG(3) << "reveived data:" << recv_buf_;
  return true;
}

bool HttpServer::HandleReq(int work_fd) {
  if (RecvRequest(work_fd) == false) {
    LOG(ERROR) << "fail to recv request, see the log for detail";
    return false;
  }
  int n = strlen(recv_buf_);
  Request req_info;
  if (!ParseRequest(recv_buf_, n, work_fd, &req_info)) {
    LOG(WARNING) << "ParseRequest failed, invalid http request";
    return false;
  }
  Respond respond;
  if (HandleWraper(req_info, &respond)) {
    string str_respond = respond.FlushToString();
    VLOG(10) << "respond data:\n" <<  str_respond;
    n = TcpSend(work_fd, str_respond.c_str(), str_respond.length());
    if ( n < 0 ) {
      LOG(WARNING) << "TcpSend failed";
      return false;
    }
    VLOG(10) << "Success handle http request";
  }
  string connection = req_info.GetHeaderValue("Connection");
  string::size_type pos = connection.find("keep-alive");
  return pos != string::npos;
}

}  // namespace http_server
}  // namespace util
