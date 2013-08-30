// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "third_part/http_client/public/http_client.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <cassert>
#include <algorithm>
#include "base/public/logging.h"
#include "base/public/time.h"
#include "base/public/string_util.h"
#include "base/public/net.h"
#include "file/public/file.h"
#include "third_part/googleurl/wrapper/public/url.h"
#include "third_part/url_util/public/url_util.h"
#include "third_part/i18n/wrapper/public/utf8_converter.h"
#include "third_part/compression/public/gzip_compression.h"

DEFINE_int32(receive_timeout, 120*1000, "time to wait receiving data( ms )");
//  根据网上资料，在大多数实现中,connect的超时时间在75秒到几分钟之间
//  0表示不限制时间
DEFINE_int32(connect_timeout, 10 * 1000, "time to connect to server ( ms )");
DEFINE_bool(print_response_header, false, "print response headers info");
DEFINE_bool(print_request_header, true, "print request headers info");
DEFINE_bool(use_user_agent, false, "if true, use spider's user "
            "agent, else use firefox's user agent");
DEFINE_bool(http_use_version_10, true, "if false, use http/1.1");
DEFINE_int32(max_page_size, 2 * 1024 * 1024,
             "Size MUST be at lease 2*1024*1024(2M)");

using namespace std;  // NOLINT
using namespace base;  // NOLINT

using url_util::ParseKvlist;

namespace util {
namespace http_client {

namespace {
const int kBlockSize = 10 * 1024;
const char* kEndOfLine = "\r\n";
const char* kEndOfHeader = "\r\n\r\n";
const char* kUserAgent =
    "Mozilla/5.0 (compatible; JikeSpider; +http://shoulu.jike.com/spider.html)";
const char* kFirefoxUserAgent =
    "Mozilla/5.0 (X11; U; Linux x86_64; en-US; rv:1.9.2.18)"
    " Gecko/20110628 Ubuntu/10.04 (lucid) Firefox/3.6.18";
}


//  构造函数，应该创建socket.
HttpClient::HttpClient():sock_fd_(-1),
                         reponse_code_(-1),
                         port_(80),
                         content_length_(0),
                         body_offset_(0),
                         total_len_(0),
                         response_total_(NULL),
                         method_(kHttpGet),
                         end_of_header_(false),
                         gzip_compresser_(new util::GzipCompression()) {}

HttpClient::~HttpClient() {
  if (response_total_) {
    free(response_total_);
    response_total_ = NULL;
  }
  CloseSocket();
}

bool HttpClient::SetUrl(const string& url) {
  return SetUrl(url.c_str());
}

bool HttpClient::SetUrl(const char* url) {
  url_ = url;
  if (!(util::url::GetHost(url_, &host_))) {
    LOG(WARNING) << "get host fail. url: " << url_;
    return false;
  }
  string port_str;
  util::url::GetPort(url_, &port_str);
  if (!port_str.empty()) {
    port_ = StringToInt(port_str);
  }
  return true;
}

int HttpClient::SetMethod(HttpMethodType method) {
  method_ = method;
  return 0;
}

int HttpClient::AddHeaders(string key, string value) {
  request_headers_[key] = value;
  return 0;
}

int HttpClient::BuildRequest() {
  string path;
  util::url::GetPathForRequest(url_, &path);
  //  request line
  string method_str;
  if (method_ == kHttpGet) {
    method_str = "GET";
  } else if (method_ == kHttpPost) {
    method_str = "POST";
  } else if (method_ == kHttpHead) {
    method_str = "HEAD";
  }
  //  暂时不支持gzip,使用1.0版本
  send_str_.append(method_str + " " + path +" HTTP/");
  if (FLAGS_http_use_version_10) {
    send_str_.append("1.0");
  } else {
    send_str_.append("1.1");
  }
  send_str_.append(kEndOfLine);
  send_str_.append(string("Host:") + host_ + kEndOfLine);

  //  Add some other headers if need
  if (request_headers_.find("Accept") ==  request_headers_.end()) {
    AddHeaders("Accept", "*/*");
  }
  if (request_headers_.find("Connection") ==  request_headers_.end()) {
    AddHeaders("Connection", "Close");
  }

  map<string, string>::const_iterator iter = request_headers_.begin();
  for (; iter != request_headers_.end(); ++iter) {
    send_str_.append(iter->first + ": " +
                     iter->second + kEndOfLine);
  }

  if (method_ == kHttpPost) {
    send_str_.append("Content_Length:"
                     + IntToString(postdata_.length()) + kEndOfLine);
    send_str_.append(kEndOfLine);
    // TODO(yesp) : 支持Postdata 按照 mutilpart,url_encode等方式发送
    send_str_.append(postdata_);
  } else {
    send_str_.append(kEndOfLine);
  }
  return 0;
}

void HttpClient::GetRespondData(string* out) {
  out->assign(body_);
}

void HttpClient::ParseStatusLine(const char* status_line) {
  vector<string> tokens;
  SplitString(status_line, ' ', &tokens);
  CHECK_GE(tokens.size(), 2);
  reponse_code_ = StringToInt(tokens[1]);
}

int HttpClient::Reset() {
  reponse_code_ = -1;
  port_ = 80;
  content_length_ = 0;
  body_offset_ = 0;
  total_len_ = 0;
  host_.clear();

  url_.clear();
  if (response_total_) {
    free(response_total_);
    response_total_ = NULL;
  }
  body_.clear();
  method_ = kHttpGet;

  postdata_.clear();
  send_str_.clear();
  end_of_header_ = false;

  response_headers_.clear();
  request_headers_.clear();
  cookies_.clear();

  CloseSocket();
  return 0;
}

int HttpClient::SendRequest() {
  // TODO(yesp) : support long connection
  sock_fd_ = TcpConnect(host_.c_str(), port_, FLAGS_connect_timeout);
  SetNonblocking(sock_fd_);
  if (sock_fd_ == -1) {
    LOG(ERROR) << "Failed connect: " << url_ << " in time:"
               << FLAGS_connect_timeout << "(Ms)";
    return -1;
  }
  //  构造请求行和头部
  BuildRequest();
  //  发送请求行和头部
  if (FLAGS_print_request_header) {
    LOG(INFO) << "request header:\n" << send_str_;
  }
  int result = TcpSend(sock_fd_, send_str_.c_str(), send_str_.length());
  if (result < 0) {
    LOG(WARNING) << "failed to send request line and headers";
    return -1;
  }
  return 0;
}

bool HttpClient::Run() {
  if (FLAGS_use_user_agent) {
    AddHeaders("User-Agent", kUserAgent);
  } else {
    AddHeaders("User-Agent", kFirefoxUserAgent);
  }

  //  1,send request
  if (SendRequest() != 0) {
    LOG(INFO) << "failed to send request";
    return false;
  }
  //  2,recv response
  if (ReceiveResponse() != 0) {
    return false;
  }
  return true;
}

static void ParseHttpHeaders(const string& str,
                             map<string, string> *kv_pair_map) {
  vector<string> pairs;
  vector<string> kvpair;
  SplitString(str, '\n', &pairs);
  VLOG(3) << "pairs num:" << pairs.size();
  for (int i = 0; i < pairs.size(); ++i) {
    kvpair.clear();
    string::size_type index = pairs[i].find(':');
    if (index != string::npos) {
      string key = StringToLowerASCII(pairs[i].substr(0, index));
      string value = pairs[i].substr(index + 1);
      TrimWhitespace(key, TRIM_ALL, &key);
      TrimWhitespace(value, TRIM_ALL, &value);
      //  Treat multiple message-header fields with the same field-name
      //  See http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2
      if (kv_pair_map->find(key) == kv_pair_map->end()) {
        kv_pair_map->insert(make_pair(key, value));
      } else {
        //  multiple value for this key
        (*kv_pair_map)[key].append(StringPrintf(", %s", value.c_str()));
      }
    } else {
      LOG(ERROR) << "bad header:" << pairs[i];
    }
  }
}

StatusCode HttpClient::TryParseHeader() {
  char* index = strstr(response_total_, kEndOfHeader);
  if (!index) {
    return Failure;
  }
  VLOG(4) << "end of header";
  LOG(INFO) << response_total_;
  end_of_header_ = true;
  body_offset_ = index - response_total_ + 4;
  *(index) = '\0';
  *(index + 1) = '\0';
  *(index + 2) = '\0';
  *(index + 3) = '\0';

  char* index2 = strstr(response_total_, kEndOfLine);
  if (!index2) {
    return BadHeader;
  }
  char* headers = index2 + 2;
  *(headers - 1) = '\0';
  *(headers - 2) = '\0';

  char* status_line_ = response_total_;
  ParseStatusLine(status_line_);

  //  3,解析头部
  ParseHttpHeaders(headers, &response_headers_);

  //  Print headers
  if (FLAGS_print_response_header) {
    printf("response_header:\n");
    map<string, string>::iterator iter = response_headers_.begin();
    for (; iter != response_headers_.end(); ++iter) {
      printf("%s:%s\n", iter->first.c_str(), iter->second.c_str());
    }
  }
  if (response_headers_.find("content-length") != response_headers_.end()) {
    content_length_ = StringToInt(response_headers_["content-length"]);
  }
  return Success;
}

int HttpClient::ReceiveResponse() {
  response_total_ = reinterpret_cast<char*>(malloc(kBlockSize));
  CHECK(response_total_);

  int rv = 0;
  int total_size = 0;

  int64_t start_tm = GetTimeInMs();
  do {
    int64_t now = GetTimeInMs();
    if (start_tm + FLAGS_receive_timeout < now) {
      LOG(WARNING) << "fetch url timeout:" << url_;
      return -1;
    }
    rv = TcpRecv(sock_fd_, response_total_ + total_size,
                 kBlockSize, FLAGS_receive_timeout);
    if (rv < 0) {
      LOG(INFO) << "some error in TcpRecv, recv len = :" << rv;
      break;
    }
    total_size += rv;
    if (!end_of_header_) {
      if (TryParseHeader() == BadHeader) {
        VLOG(2) << "bad respond header";
        return -1;
      }
    }
    VLOG(4) << "total_size:" << total_size << ", content_length_:"
            << content_length_ << ", body_offset_:" << body_offset_;
    if (end_of_header_ &&
        content_length_ != 0 &&
        total_size == content_length_ + body_offset_) {
      break;
    }
    response_total_ = reinterpret_cast<char*>(
                        realloc(response_total_, total_size + kBlockSize));
  } while (rv);

  if (total_size == 0) {
    LOG(WARNING) << "fail to recv any data";
    return -1;
  }
  total_len_ =  total_size;
  response_total_[total_size] = '\0';

  ParseResponse();
  CloseSocket();
  return 0;
}

void HttpClient::ParseResponse() {
  char* body = response_total_ + body_offset_;
  int body_length_ = total_len_ - body_offset_;
  body_.assign(body, body_length_);

  if (response_headers_.find("content-encoding") != response_headers_.end()) {
    VLOG(3) << "respond data is compressed";
    // TODO(yesp) : try to support other zip format
    string encoding = response_headers_["content-encoding"];
    if (encoding == "gzip" || encoding == "x-gzip") {
      string result;
      if (gzip_compresser_->Uncompress(body_, FLAGS_max_page_size, &result)) {
        body_.assign(result);
      } else {
        LOG(ERROR) << "fail to uncompress respond data";
      }
    }
  }
  //  now http_client do not support auto set cookies for request
  //  ParseCookies();
}

void HttpClient::ParseCookies() {
  map<string, string>::iterator iter = response_headers_.begin();
  for (; iter != response_headers_.end(); ++iter) {
    if (iter->first == "set-cookie") {
      vector<pair<string, string> > tmp;
      ParseKvlist(iter->second, "=", ';', NULL, &tmp);
      for (size_t k = 0; k < tmp.size(); ++k) {
        cookies_.push_back(tmp[k]);
      }
    }
  }
}

int HttpClient::GetRespondCode() {
  return reponse_code_;
}

void HttpClient::SetPostData(const string& postdata) {
  postdata_ = postdata;
}

void HttpClient::CloseSocket() {
  if (sock_fd_ != -1) {
    close(sock_fd_);
    sock_fd_ = -1;
  }
}

bool HttpGet(const string& url, string* out) {
  HttpClient client;
  if (!client.SetUrl(url))
    return false;
  if (client.Run()) {
    if (client.GetRespondCode() != 200) {
      LOG(ERROR) << "response code != 200, code=" << client.GetRespondCode();
      return false;
    }
    string data;
    client.GetRespondData(&data);
    if (data.empty()) {
      LOG(WARNING) << "code == 200, but size = 0";
      return false;
    }
    out->assign(data);
    return true;
  } else {
    return false;
  }
}

bool ReplaceMetadataToUtf8(const string& in, string* out) {
  int index1 = in.find("<head>");
  int index2 = in.find("<meta");
  int t1 = in.find("content-type");
  int t2 = in.find("Content-Type");
  int index3 = max(t1, t2);
  int index4 = in.find("</head>");

  if (index1 == string::npos ||
      index2 == string::npos ||
      index3 == string::npos ||
      (index3 > index4)) {
    return false;
  }
  int start = in.find("charset=", index3);
  if (start == string::npos) {
    return false;
  }
  start += strlen("charset=");
  int end = in.find(">", start);
  out->assign(in.substr(0, start));
  out->append("utf8\"");
  out->append(in.substr(end));
  return true;
}

bool HttpGetAndTranslateToUtf8(const string& url, string* out) {
  string content;
  if (!HttpGet(url, &content)) {
    return false;
  } else {
    string utf8_content;
    util::Utf8Converter converter;
    if (!converter.ConvertToUtf8(content, &utf8_content)) {
      LOG(ERROR) << "fail to convert to utf8 encoding";
      return false;
    }
    //  change metadata
    string utf8_meta;
    if (!ReplaceMetadataToUtf8(utf8_content, &utf8_meta)) {
      VLOG(4) << "fail to replace meta charset. but return true";
    } else {
      utf8_content = utf8_meta;
    }
    out->assign(utf8_content);
  }
  return true;
}

bool HttpGet(const string& url, const string& file) {
  HttpClient client;
  client.SetUrl(url);
  if (client.Run()) {
    string data;
    client.GetRespondData(&data);
    file::File::WriteStringToFile(data, file);
    return true;
  } else {
    return false;
  }
}

bool ParseHttpHeader(
    const string& header, int32* response_code, map<string, string>* headers) {
  size_t status_line_end = header.find("\n");
  if (status_line_end == string::npos) {
    return false;
  }

  // Get response code
  string status_line = header.substr(0, status_line_end);
  vector<string> tokens;
  SplitString(status_line, ' ', &tokens);
  if (tokens.size() < 2) {
    return false;
  }
  string code;
  TrimWhitespaceASCII(tokens[1], TRIM_ALL, &code);
  *response_code = StringToInt(code);

  // Get headers key-value
  string header_block = header.substr(status_line_end + 1);
  vector<string> pairs;
  SplitString(header_block, '\n', &pairs);
  for (size_t i = 0; i < pairs.size(); ++i) {
    const string& pair = pairs[i];

    size_t k = 0;
    for (; k < pair.size() && pair[k] != ':'; ++k) {}
    if (k >= pair.size()) {
      VLOG(3) << "invalid header:" << pair;
      continue;
    }

    string key = pair.substr(0, k);
    string value = pair.substr(k + 1);

    TrimWhitespaceASCII(StringToLowerASCII(key), TRIM_ALL, &key);
    TrimWhitespaceASCII(StringToLowerASCII(value), TRIM_ALL, &value);

    if (headers->find(key) != headers->end()) {
      (*headers)[key] = (*headers)[key] + "\n" + value;
    } else {
      headers->insert(make_pair(key, value));
    }
  }
  return true;
}
}  //  namespace http_client
}  //  namespace util
