// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 22:10:20
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

struct RefCount {
  RefCount(int v = 0) : count(v){}
  void Increment() {
    count++;
  }
  void Decrement() {
    count--;
  }
  bool IsZero() {
    return count == 0;
  }
  int count;
};

// 浅拷贝的应用
template <typename type>
class SmartPtr {
 public:
  SmartPtr(type * v = NULL) : object_(v) {
     ref_ = (v == NULL) ? new RefCount(0) : new RefCount(1);
  }

  ~SmartPtr() {
    ref_->Decrement();
    if (ref_->IsZero()) {
      delete object_;
      delete ref_;
    }
  }
  // 这是拷贝构造不用swap
  SmartPtr(const SmartPtr<type> & v) {
    object_ = v.object_;
    ref_ = v.ref_;
    ref_->Increment();
  }

  SmartPtr<type> * operator = (const SmartPtr<type> & v) {
    if (object_ == v.object_) return this;
    // 这个trick大大简化了代码, 通过调用tmp的析构函数实现
    {
      SmartPtr<type> tmp;
      Swap(this, &tmp);
    }
    object_ = v.object_;
    ref_ = v.ref_;
    ref_->Increment();
    return this;
  }

  SmartPtr<type> * Get() {
    return object_;
  }

 private:
   void Swap(SmartPtr<type> * p1, SmartPtr<type> * p2) {
     std::swap(p1->object_, p2->object_);
     std::swap(p1->ref_, p2->ref_);
   }

   type * object_;
   RefCount * ref_;
};

class Foo {
 public:
  Foo() {
    LOG(INFO) << "call Foo()";
  }
  ~Foo() {
    LOG(INFO) << "call ~Foo()";
  }
 private:
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  SmartPtr<Foo> ptr(new Foo());
  SmartPtr<Foo> ptr2(ptr);
  SmartPtr<Foo> ptr3;
  ptr3 = ptr2;
  SmartPtr<Foo> ptr4;
  return 0;
}
