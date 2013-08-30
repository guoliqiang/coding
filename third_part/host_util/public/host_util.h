// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author: bingbai@yunrang.com (Bing Bai)
//
#ifndef UTIL_HOST_UTIL_HOST_UTIL_H_
#define UTIL_HOST_UTIL_HOST_UTIL_H_

#include <string>
#include "base/public/logging.h"
namespace util {
class host_util {
 public:
  host_util();
  ~host_util();
  static std::string LocalHostName();
 private:
  DISALLOW_COPY_AND_ASSIGN(host_util);
};
}
#endif  // UTIL_HOST_UTIL_HOST_UTIL_H_
