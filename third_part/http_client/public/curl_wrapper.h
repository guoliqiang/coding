// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: liangwang@yunrang.com (Wang Liang)

#ifndef UTIL_HTTP_CLIENT_CURL_WRAPPER_H_
#define UTIL_HTTP_CLIENT_CURL_WRAPPER_H_

#include <string>
#include "base/public/basictypes.h"

namespace util {

class CurlWrapper {
 public:
  CurlWrapper() {}
  virtual ~CurlWrapper() {}
  static int GetRespondData(const std::string& url ,
                            std::string* out,
                            std::string* err_info);

 private:
  DISALLOW_COPY_AND_ASSIGN(CurlWrapper);
};
}  // namespace util

#endif  // UTIL_HTTP_CLIENT_CURL_WRAPPER_H_
