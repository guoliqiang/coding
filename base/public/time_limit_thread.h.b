// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-25 22:42:22
// File  : time_limit_thread.h
// Brief :

#ifndef  __TIME_LIMIT_THREAD2_H_
#define  __TIME_LIMIT_THREAD2_H_

#include <pthread.h>
#include "base/public/basictypes.h"
#include "base/public/mutex.h"

namespace base {

class ContainerBase {
 public:
  ContainerBase() { }
  virtual ~ContainerBase() { }
};

template <typename Type>
class DataContainer : public ContainerBase {
 public:
  DataContainer() : ContainerBase() {
    data_ = NULL;
  }
  ~DataContainer() {
    if (data_ != NULL) delete data_;
  }
  Type * Data() {
    data_ = new Type;
    return data_;
  }
 private:
  Type * data_;
};

class TimeLimitThread {
 public:
  TimeLimitThread(int time_out)
      : started_(false),
        finished_(false),
        time_out_(time_out) { }

  virtual ~TimeLimitThread() {
    for (int i = 0; i < data_.size(); i++) {
      delete data_[i];
    }
  }

  pthread_t tid() const {
    return tid_;
  }
  void Start();
  bool Join();
  template<typename Type>
  Type * GetData() {
    DataContainer<Type> * ptr = new DataContainer<Type>();
    Type * rs = ptr->Data();
    data_.push_back(ptr);
    return rs;
  }

  template<typename Type>
  Type * GetData(const Type & value) {
    DataContainer<Type> * ptr = new DataContainer<Type>();
    Type * rs = ptr->data();
    *rs = value;
    data_.push_back(ptr);
    return rs;
  }

 protected:
  virtual void Run() = 0;
  void Exit();

  static void* ThreadRunner(void* arg) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    TimeLimitThread *t = reinterpret_cast<TimeLimitThread*>(arg);
    t->Run();
    t->Exit();
    return NULL;
  }

  pthread_t tid_;
  bool started_;
  bool finished_;
  int time_out_;
  Mutex mutex_;
  CondVar cond_var_;
  std::vector<ContainerBase*> data_;
  DISALLOW_COPY_AND_ASSIGN(TimeLimitThread);
};

}  // namespace base

#endif  //__TIME_LIMIT_THREAD2_H_
