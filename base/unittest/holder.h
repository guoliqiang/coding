// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-02 00:28:32
// File  : holder.h
// Brief :

#ifndef  __HOLDER_H_
#define  __HOLDER_H_

#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include "base/public/scoped_ptr.h"

// like boost::shared_ptr
template <class T>
class ObjectHolder {
 public:
  ObjectHolder() {}
  ObjectHolder(T * t) {
    delete t;
  }
};

// like boost::scoped_ptr
template <class T>
class ObjectScopedHolder {
 public:
  ~ObjectScopedHolder() {
    typedef char type_must_be_complete[ sizeof(T) ? 1 : -1];
    delete t_;
  }
  ObjectScopedHolder(T * t) { t_ = t; }

  T * t_;
};

class Item;

class Holder {
 public:
  Holder();
  ObjectHolder<Item> item_;  // only link ~Item when constructor
  // ObjectScopedHolder<Item> item2_;// line will link ~Item, so it will fail.
};

#endif  //__HOLDER_H_
