// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)
//         wangbin@yunrang.com (Bin Wang)

// TODO(yesp) : 使用异步socket重新实现

#ifndef UTIL_HTTP_CLIENT_HTTP_CLIENT_H_
#define UTIL_HTTP_CLIENT_HTTP_CLIENT_H_

#include <netinet/in.h>

#include <cstdio>
#include <cstdlib>

#include <string>
#include <vector>
#include <utility>
#include <map>

#include "base/public/basictypes.h"
#include "base/public/scoped_ptr.h"

namespace util {
class GzipCompression;
}

namespace util {
namespace http_client {

enum HttpMethodType {
  kHttpGet,
  kHttpPost,
  kHttpHead
};

enum StatusCode {
  Success    = 0,
  BadHeader  = 1,
  Failure    = 2,
};

class HttpClient {
 public:
  HttpClient(void);
  ~HttpClient(void);
  bool Run();

  bool SetUrl(const char* url);
  bool SetUrl(const std::string& url);
  int AddHeaders(std::string key, std::string value);
  void GetRespondData(std::string* out);
  int SetMethod(HttpMethodType type);
  int Reset();
  void SetPostData(const std::string& postdata);
  int GetRespondCode();

  const std::vector<std::pair<std::string, std::string> >& cookies() const {
    return cookies_;
  }

  //  use flag print_response_header instead
  void BannedPrintRespHeaders() YR_DEPRECATED {}
  //  use flag print_request_header instead
  void SetPrintHeaders(bool print) YR_DEPRECATED {}

 private:
  int BuildRequest();
  int ReceiveResponse();
  StatusCode TryParseHeader();
  void ParseResponse();
  void ParseCookies();
  int SendRequest();
  void ParseStatusLine(const char* status_line);
  void CloseSocket();
 private:
  int sock_fd_;
  int reponse_code_;
  int port_;
  int content_length_;
  int body_offset_;
  int total_len_;
  std::string host_;
  std::string url_;
  char* response_total_;
  std::string body_;
  HttpMethodType method_;
  std::string postdata_;
  std::string send_str_;
  bool end_of_header_;
  scoped_ptr<util::GzipCompression> gzip_compresser_;

  std::map<std::string, std::string> request_headers_;
  std::vector<std::pair<std::string, std::string> > cookies_;
  std::map<std::string, std::string> response_headers_;
  DISALLOW_COPY_AND_ASSIGN(HttpClient);
};

bool HttpGet(const std::string& url, std::string* out);
//  this function will get html ,then conv to utf8 encoding, what's more,
//  it will replace charset with utf8 if possible
bool HttpGetAndTranslateToUtf8(const std::string& url, std::string* out);
bool HttpGet(const std::string& url, const std::string& file);
bool ParseHttpHeader(const std::string& header,
                     int32* response_code,
                     std::map<std::string, std::string>* headers);
}
}
#endif  // UTIL_HTTP_CLIENT_HTTP_CLIENT_H_
