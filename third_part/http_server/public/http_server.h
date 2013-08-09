// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Yanliang Cui)

#ifndef UTIL_HTTP_SERVER_HTTP_SERVER_H_
#define UTIL_HTTP_SERVER_HTTP_SERVER_H_

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <string>

#include "base/public/thread.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "base/public/hash_tables.h"
#include "base/public/callback.h"

namespace util {
namespace http_server {

// the parsed http resuest
class Request {
 public:
  // type of request, only support GET and POST now. 
  enum Type {
    kHttpGet,
    kHttpPost,
  };

  std::string GetBodyContent() const;
  std::string GetHeaderValue(const std::string& head_name) const;
  std::string GetUrlParamValue(const std::string& param_name) const;
  std::string GetPostParamValue(const std::string& param_name) const;

  Type type() const { return type_; }
  std::string url() const { return url_; }

  const std::map<std::string, std::string> &header() const { return header_; }
  const std::map<std::string, std::string> &url_params() const {
    return url_params_;
  }
  const std::map<std::string, std::string> &post_params() const {
    return post_params_;
  }

  const std::vector<std::pair<std::string, std::string> >&
    url_param_vec() const {
    return url_param_vec_;
  }
  const std::vector<std::pair<std::string, std::string> >&
    post_param_vec() const {
    return post_param_vec_;
  }

 private:
  Type type_;
  std::string url_;
  friend class HttpServer;
  // http head
  std::map<std::string, std::string> header_;
  
  // the key-value contained in url, splited by '&', like:
  // http://www.omusic.cc/#model=song&songid=863083&stype=song&merge=1&play=1
  std::map<std::string, std::string> url_params_;
  
  // the ordered (specied in url) key-value in url, e.g.:
  std::vector<std::pair<std::string, std::string> > url_param_vec_;

  // the key-value of POST type, splited by '&'
  std::map<std::string, std::string> post_params_;
  
  // the ordered key-value of POST type.(specified in HTML)
  std::vector<std::pair<std::string, std::string> > post_param_vec_;
  
  std::string body_content_;
};

// response to http client
class Respond {
 public:
  explicit Respond();
  void SetStatus(const int32 &status);
  void AddContent(const std::string &info);
  void AddContent(const char* format, ...);
  void AddHeader(const std::string &key, const std::string &value);
  void AddHeader(const std::string &key, const char *value_format, ...);

 private:
  std::string FlushToString();

  friend class HttpServer;
  std::string content_;
  std::map<std::string, std::string> header_;
  int32 status_;
};

enum HandleStatus {
  kKeepAlive,     // succeed and keep connection.
  kCloseSock,     // succeed and cut connection.
  kHandleError    // failed and cut connection.
};

class HttpServer : public base::Thread {
 public:
  explicit HttpServer(uint32_t port,
                      uint32_t interval_ms = 100);
  explicit HttpServer(uint32_t port,
                      uint32_t interval_ms,
                      bool require_auth);
  ~HttpServer();

  void TryStop() {
    stop_ = true;
  }

  void Stop() {
    stop_ = true;
    Join();
  }

  void TimingStart(int wait_sec = 0);

  template <class Type>
  void AddHandler(const std::string& name,
                  Type* obj,
                  bool (Type::*member)(const Request&, Respond*)) {
    handlers_[name] = base::NewPermanentCallback(obj, member);
  }

  void AddHandler(const std::string& name,
                  bool (*fun)(const Request&, Respond*)) {
    handlers_[name] = base::NewPermanentCallback(fun);
  }

  void AddHandler(const std::string& name,
                  base::ResultCallback2<bool, const Request&, Respond*>*
                  callback) {
    handlers_[name] = callback;
  }

  template <class Type>
  void SetDftHandler(Type* obj,
                     bool (Type::*member)(const Request&, Respond*)) {
    dft_handler_ = base::NewPermanentCallback(obj, member);
  }

  void SetDftHandler(bool (*fun)(const Request&, Respond*)) {
    dft_handler_ = base::NewPermanentCallback(fun);
  }

  void SetDftHandler(base::ResultCallback2<bool, const Request&, Respond*>*
                     callback) {
    dft_handler_ = callback;
  }

  uint16 ServerPort() const { return port_; }

  virtual void Init() {}

 private:
  void Init(uint32_t port, uint32_t interval_ms, bool require_auth);
  bool BasicAuth(std::string auth_string);
  void LoadAuthFile();

  typedef base::hash_map<std::string,
          base::ResultCallback2<bool, const Request&, Respond*>*> StrToCallback;
  virtual void Run();

  bool HandleWraper(const Request &request, Respond *respond);
  bool RecvRequest(int sock);
  bool HandleReq(int work_fd);
  bool ParseRequest(char *request, int request_len,
                    int work_fd, Request *req_info);
  void RunWithSelect();
  void RunWithEpoll();

  StrToCallback handlers_;
  base::ResultCallback2<bool, const Request &, Respond*>* dft_handler_;

  static const uint32_t kRecvTimeout = 1000 * 10;
  static const uint32_t kRecvBufLen = 1024*1024*10;
  uint32_t port_;
  uint32_t interval_ms_;
  char *recv_buf_;
  bool stop_;
  int wait_sec_;
  
  int epoll_instance_;
  bool require_auth_;
  std::vector<std::pair<std::string, std::string> > user_passes_;
  DISALLOW_COPY_AND_ASSIGN(HttpServer);
};

}  // namespace http_server
}  // namespace util

#endif  // UTIL_HTTP_SERVER_HTTP_SERVER_H_
