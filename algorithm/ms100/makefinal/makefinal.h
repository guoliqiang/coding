// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 17:29:44
// File  : makefinal.h
// Brief :

#ifndef  __MAKEFINAL_H_
#define  __MAKEFINAL_H_

#include "base/public/makefinal.h"

namespace algorithm {

FINAL(Foo);
class Foo : virtual public MakeFinalFoo {
 public:
   Foo() {};
   virtual ~Foo() {}
  private:
};

class Bar: public Foo {
 public:
  Bar() {}
  ~Bar() {}
 private:
};
}  // namespace algorithm
#endif  //__MAKEFINAL_H_
