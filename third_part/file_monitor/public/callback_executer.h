// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_CALLBACK_EXECUTER_H_
#define UTIL_FILE_MONITOR_CALLBACK_EXECUTER_H_

#include <map>
#include <vector>
#include <string>
#include "base/public/thread.h"
#include "base/public/time.h"
#include "base/public/scoped_ptr.h"
#include "third_part/file_monitor/public/file_monitor_type.h"
namespace base {
  class Mutex;
}
namespace util {
class CallbackExecuter : public base::Thread {
 public:
  CallbackExecuter();
  ~CallbackExecuter();
 public:
  void AddCallback(int wd,
                   const std::string& path,
                   const CallbackSet& callback,
                   WatchEventType eventtype);
 protected:
  virtual void Run();
 private:
  bool IsTimespanEnough(const base::TimeTicks& val1,
                        const base::TimeTicks& val2);
 public:
  void StopRunning();
 private:
  ThreadStatus               status_;
  CallbackMap                callbackmap_;
  scoped_ptr<base::Mutex>    mutex_;
 private:
  DISALLOW_COPY_AND_ASSIGN(CallbackExecuter);
};
}
#endif  // UTIL_FILE_MONITOR_CALLBACK_EXECUTER_H_
