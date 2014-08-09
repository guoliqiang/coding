// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-08-09 17:30:02
// File  : thread_safe_guard.h
// Brief :

#ifndef  __THREAD_SAFE_GUARD_H_
#define  __THREAD_SAFE_GUARD_H_

#include <map>
#include <string>
#include <sys/syscall.h>
#include "logging.h"
#include "mutex.h"
#include "light_singleton.h"
#include "string_util.h"

namespace base {

class ThreadInfoHolder {
 public:
  friend class LightSingleton<ThreadInfoHolder>;

  static ThreadInfoHolder & GetInstance() {
    return *LightSingleton<ThreadInfoHolder>::GetInstance();
  }

  void Add(const std::string & key, int32 tid) {
    base::MutexLock lock(&mutex_);
    if (info_.count(key)) {
      CHECK(false) << key << " want add to thread " << tid
                   << " but, it has in thread " << info_[key];
    }
    info_[key] = tid;
  }

  void Erase(const std::string & key) {
    base::MutexLock lock(&mutex_);
    CHECK(info_.count(key)) << key;
    info_.erase(key);
  }

 private:
  ThreadInfoHolder() {}
 
 private:
  std::map<std::string, int32> info_;
  base::Mutex mutex_;
  DISALLOW_COPY_AND_ASSIGN(ThreadInfoHolder);
};

class ThreadInfoWrapper {
 public:
  ThreadInfoWrapper(const std::string & key) : key_(key) {
    int32 id = CurrentThreadId();
    ThreadInfoHolder::GetInstance().Add(key, id);
  };

  ~ThreadInfoWrapper() {
    ThreadInfoHolder::GetInstance().Erase(key_);
  }

  int32 CurrentThreadId() {
    return syscall(__NR_gettid);
  }
 private:
  const std::string key_;
  DISALLOW_COPY_AND_ASSIGN(ThreadInfoWrapper);
};

}  // namespace base

#define THREAD_SATE_STRING(str) base::ThreadInfoWrapper _tss_foo(str)
#define THREAD_SAFE_PTR(ptr) base::ThreadInfoWrapper \
_tsp_foo(Uint64ToString(reinterpret_cast<uint64>(ptr)))
#define THREAD_SAFE_FUN  THREAD_SATE_STRING(__FUNCTION__)

#endif  // __THREAD_SAFE_GUARD_H_
