// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#include "../public/thread_pool.h"

namespace base {

ThreadPool::ThreadPool(int n_threads) : queue_(n_threads) {
  for (int i = 0; i < n_threads; i++) {
    WorkerThread *thread = new WorkerThread(&ThreadPool::Worker, this);
    workers_.push_back(thread);
  }
}

ThreadPool::~ThreadPool() {
  for (size_t i = 0; i < workers_.size(); i++) {
    Add(NULL);
  }
  for (size_t i = 0; i < workers_.size(); i++) {
    workers_[i]->Join();
    delete workers_[i];
  }
}

void ThreadPool::StartWorkers() {
  for (size_t i = 0; i < workers_.size(); i++) {
    workers_[i]->Start();
  }
}

void* ThreadPool::Worker(void *p) {
  ThreadPool *pool = reinterpret_cast<ThreadPool*>(p);
  while (true) {
    Closure *closure = pool->queue_.Get();
    if (closure == NULL) {
      return NULL;
    }
    closure->Run();
  }
}

}  // namespace base
