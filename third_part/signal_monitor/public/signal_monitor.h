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

// do not need to add mutex, because a signal can not be captured by
// more than one thread.
class SignalMonitor {
 public:
  static SignalMonitor * GetInstance() {
    return LightSingleton<SignalMonitor>::GetInstance();
  }

  void AddCallback(int signal_num, base::Closure * callback,
      bool check = true) {
    CHECK(signal_num >= 1 && signal_num <= 64)  << signal_num
       << " is invalid";
    struct sigaction new_sa;
    struct sigaction old_sa;
    new_sa.sa_handler = SignalMonitor::SignalCall;
    sigemptyset(&new_sa.sa_mask);
    new_sa.sa_flags = SA_NOCLDSTOP;
    sigaction(signal_num, &new_sa, &old_sa);
    if (check) {
      CHECK(old_sa.sa_handler == NULL)  << "signal num " << signal_num
          << " alaredy in use";
    }
    
    // intptr_t old_signal_handler = reinterpret_cast<intptr_t>(
    //     signal(signal_num, SignalMonitor::SignalCall));
    // CHECK(old_signal_handler == 0) << "signal num "
    //     << signal_num << " already in use";
    callback_.insert(std::make_pair(signal_num,
        base::shared_ptr<base::Closure>(callback)));
  }

  void Execute(int signal_num) {
    if (callback_.count(signal_num)) {
      callback_[signal_num]->Run();
    } else {
      LOG(WARNING) << "not find callback for " << signal_num;
    }
  }

  static void SignalCall(int signal_num) {
    GetInstance()->Execute(signal_num);
  }

 private:
  std::map<int, base::shared_ptr<base::Closure> > callback_;
};

}  // namespace signal_monitor

#endif  //__SIGNAL_MONITOR_H_
