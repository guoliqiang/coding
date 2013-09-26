// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 17:27:18
// File  : makefinal.h
// Brief :

#ifndef  BASE_MAKEFINAL_H_
#define  BASE_MAKEFINAL_H_

// http://www.linuxidc.com/Linux/2012-11/74492p4.htm
// Use sample:
// FINAL(Foo);
// class Foo : virtual public MakeFinalFoo {
// }
// Note: must use virtual inherit
// Fool class can not be inherit

#define FINAL(type)                 \
class MakeFinal ## type{            \
 public:                            \
  friend class type;                \
 private:                           \
  MakeFinal ## type() {}            \
  virtual ~MakeFinal ## type () {}  \
}

#endif  // BASE_MAKEFINAL_H_
