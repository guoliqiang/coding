// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_FILE_WATCHER_H_
#define UTIL_FILE_MONITOR_FILE_WATCHER_H_

#include <string>
#include "base/public/thread.h"
#include "base/public/scoped_ptr.h"
#include "base/public/mutex.h"
#include "third_part/file_monitor/public/monitor_observer.h"
#include "third_part/file_monitor/public/file_monitor_common.h"
namespace util {

class FileWatcher : public base::Thread {
 public:
  explicit FileWatcher(MonitorObserver* observer);
  virtual ~FileWatcher();

 public:
  // return wd
  int AddMonitor(const std::string& path);
  void RemoveWatching(int wd);
  void StopRunning();
 private:
  WatchEventType GetWatchEventType(unsigned int event);

 protected:
  virtual void Run();

 private:
  int              fd_;
  MonitorObserver* observer_;
  ThreadStatus     status_;
 private:
  DISALLOW_COPY_AND_ASSIGN(FileWatcher);
};
}
#endif  // UTIL_FILE_MONITOR_FILE_WATCHER_H_
