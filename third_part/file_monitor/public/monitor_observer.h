// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_MONITOR_OBSERVER_H_
#define UTIL_FILE_MONITOR_MONITOR_OBSERVER_H_
#include <string>
#include "third_part/file_monitor/public/file_monitor_common.h"
namespace util {
class MonitorObserver {
 public:
  virtual void HandleInotifyEvent(int wd,
                                  WatchEventType reason,
                                  const std::string& name) = 0;
  virtual ~MonitorObserver() {}
};
}
#endif  // UTIL_FILE_MONITOR_MONITOR_OBSERVER_H_
