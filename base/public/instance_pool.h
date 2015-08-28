// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-07-22 16:34:53
// File  : instance_pool.h
// Brief :

#ifndef  __INSTANCE_POOL_H_
#define  __INSTANCE_POOL_H_

#include "base/public/shared_ptr.h"
#include "base/public/scoped_ptr.h"
#include "base/public/mutex.h"
#include "base/public/concurrent_queue.h"

namespace base {

template <class T> class InstanceWrapper;

template <class T>
class InstancePool {
 friend class  InstanceWrapper<T>;
 
 public:
  static InstancePool<T> & Get() {
    if (instance_pool.get() == NULL) {
      base::MutexLock lock(&mutex);
      if (instance_pool.get() == NULL) {
        instance_pool.reset(new InstancePool<T>());
      }
    }
    return *(instance_pool.get());
  }

  bool GetInstanceWrapper(scoped_ptr<InstanceWrapper<T> > * ptr) {
    CHECK(values_vec_.size()) << "do not add any instance in this pool";
    ptr->reset(NULL);
    T * tmp;
    VLOG(5) << "waiting for pop an instance";
    values_.Pop(tmp);
    VLOG(5) << "get an instance";
    ptr->reset(new InstanceWrapper<T>(tmp));
    return true;
  }

 bool Init(int num = 10) {
   for (int i = 0; i < num; i++) {
     VLOG(5) << "create " << i <<"th instance";
     base::shared_ptr<T> tmp(new T());
     values_.Push(tmp.get());
     values_vec_.push_back(tmp);
   }
   return true;
 }
 
 // use as AddInstance (new T())
 bool AddInstance(T * ptr) {
   VLOG(5) << "add instance:" << ptr;
   base::shared_ptr<T> tmp(ptr);
   values_.Push(ptr);
   values_vec_.push_back(tmp);
   return true;
 }
 
 private:
  bool Back(T * back) {
    values_.Push(back);
    VLOG(5) << "back instance:" << back;
    return true;
  }

 private:
  static base::Mutex mutex;
  static scoped_ptr<InstancePool<T> > instance_pool;
 
 private:
  base::ConcurrentQueue<T*> values_;
  std::vector<base::shared_ptr<T> > values_vec_;
};

template <class T> base::Mutex InstancePool<T>::mutex;
template <class T> scoped_ptr<InstancePool<T> > InstancePool<T>::instance_pool;

template <class T>
class InstanceWrapper {
 public:
  explicit InstanceWrapper(T * instance) {
    instance_ = instance;
  }

  ~InstanceWrapper() {
    InstancePool<T>::Get().Back(instance_);
  }

  T & Get() {
    return *instance_;
  }

 private:
  T * instance_;
};

}  // namespace base
#endif  //  __INSTANCE_POOL_H_
