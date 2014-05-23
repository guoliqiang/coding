// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-05-22 20:25:29
// File  : parallel_pool.h
// Brief :

#ifndef  __PARALLEL_POOL_H_
#define  __PARALLEL_POOL_H_

#include "thread_pool.h"
#include "scoped_ptr.h"
#include "singleton.h"
#include "logging.h"

namespace base {
class ParallelPool {
 public:
  static ParallelPool * GetInstance() {
    return Singleton<ParallelPool>::get();
  }

  ParallelPool(int size = 10) {
    thread_pool_.reset(new ThreadPool(size));
    thread_pool_->StartWorkers();
  }

  void Add(Closure * fun) {
    thread_pool_->Add(fun);
  }
 private:
  scoped_ptr<ThreadPool> thread_pool_;
  DISALLOW_COPY_AND_ASSIGN(ParallelPool);
};
}

#endif  // __PARALLEL_POOL_H_
