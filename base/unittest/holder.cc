// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-02 00:30:01
// File  : holder.cc
// Brief :

#include "holder.h"

class Item {
 public:
  ~Item() {
    LOG(INFO) << "~Item";
  }
};

Holder::Holder() : item_(new Item()) {}
