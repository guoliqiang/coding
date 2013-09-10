// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 12:37:17
// File  : deep_copy.h
// Brief :

#ifndef  __DEEP_COPY_H_
#define  __DEEP_COPY_H_

#include "base/public/logging.h"

namespace algorithm {

template<typename type>
class shared_ptr {
 public:
  shared_ptr(type * ptr = NULL) {
    ptr_ = ptr;
    count_ = new int(0);
    if (ptr_ != NULL) (*count_)++;
  }

  ~shared_ptr() {
    erase();
  }

  shared_ptr(const shared_ptr<type> & a) {
    ptr_ = a.ptr_;
    count_ = a.count_;
    if (ptr_ != NULL) (*count_)++;
  }

  shared_ptr<type> & operator = (const shared_ptr<type> & a) {
    if (this == &a) return *this;
    erase();
    ptr_ = a.ptr_;
    count_ = a.count_;
    (*count_)++;
    return *this;
  }

 private:
  void erase() {
    if ((*count_) != 0) {
      (*count_)--;
      if ((*count_) == 0) {
        LOG(INFO) << "delete prt";
        delete ptr_;
        delete count_;
        ptr_ = NULL;
        count_ = NULL;
      }
    } 
  }

 private:
  type * ptr_;
  int * count_;
};


template <typename type>
class Bar {
 public:
  Bar(type * value = NULL):value_(value){}
  Bar(const Bar<type> &a) {
    value_ = a.value_;
  }
 private:
  shared_ptr<type> value_;
};

template <typename type>
class Foo {
 public:
  Foo(int len) {
    LOG(INFO) << "constructor";
    len_ = len;
    value_ = NULL;
    if (len_ > 0) value_ = new type[len_];
  }
  ~Foo() {
    LOG(INFO) << "~Foo";
    delete value_;
    value_ = NULL;
  }
  Foo(const Foo & a) {
    LOG(INFO) << "copy constructor";
    len_ = a.len_;
    value_ = new type[len_];
    for (int i = 0; i < len_; i++) {
      value_[i] = a.value_[i];
    }
  }
  Foo & operator = (const Foo & a) {
    LOG(INFO) << "operator =";
    if (this == &a) return *this;  // note
    if (len_ > 0) {
      delete [] value_;
      value_ = NULL;
    }
    len_ = a.len_;
    value_ = NULL;
    if (len_) value_ = new type[len_];
    for (int i = 0; i< len_; i++) {
      value_[i] = a.value_[i];
    }
    return *this;
  }

 private:
   int len_;
   type * value_;
};
}  // namespace algorithm
#endif  //  __DEEP_COPY_H_
