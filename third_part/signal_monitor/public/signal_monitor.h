// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-19 20:23:52
// File  : signal_monitor.h
// Brief :

#ifndef  __SIGNAL_MONITOR_H_
#define  __SIGNAL_MONITOR_H_

#include <map>
#include <signal.h>
#include "base/public/logging.h"
#include "base/public/callback.h"
#include "base/public/shared_ptr.h"
#include "base/public/light_singleton.h"

namespace signal_monitor {

class SignalMonitor {
 public:
  static SignalMonitor * GetInstance() {
    return LightSingleton<SignalMonitor>::GetInstance();
  }

  void AddCallback(int signal_num, base::Closure * callback) {
    CHECK(signal_num >= 1 && signal_num <= 64)  << signal_num
       << " is invalid";
    base::MutexLock lock(&mutex_);
    intptr_t old_signal_handler = reinterpret_cast<intptr_t>(
        signal(signal_num, SignalMonitor::SignalCall));
    CHECK(old_signal_handler == 0) << "signal num "
        << signal_num << " already in use";
    callback_.insert(std::make_pair(signal_num,
        base::shared_ptr<base::Closure>(callback)));
  }

  void Excute(int signal_num) {
    base::MutexLock lock(&mutex_);
    if (callback_.count(signal_num)) {
      callback_[signal_num]->Run();
    } else {
      LOG(WARNING) << "not find callback for " << signal_num;
    }
  }

  static void SignalCall(int signal_num) {
    GetInstance()->Excute(signal_num);
  }

 private:
  std::map<int, base::shared_ptr<base::Closure> > callback_;
  base::Mutex mutex_;
};

}  // namespace signal_monitor

#endif  //__SIGNAL_MONITOR_H_
