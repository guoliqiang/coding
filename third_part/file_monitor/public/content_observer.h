// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_CONTENT_OBSERVER_H_
#define UTIL_FILE_MONITOR_CONTENT_OBSERVER_H_

#include <string>
namespace util {
class ContentObserver {
 public:
  virtual bool ContentChanged(const std::string& filepath,
                              const std::string& md5value,
                              int wd) = 0;
  virtual bool UpdateHashValue(const std::string& filepath,
                               const std::string& md5value,
                               int wd) = 0;
  virtual ~ContentObserver() {}
};
}
#endif  // UTIL_FILE_MONITOR_CONTENT_OBSERVER_H_
