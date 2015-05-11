// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-05-10 23:18:24
// File  : unit2.cc
// Brief :

#include "algorithm/static_inline/header.h"

namespace algorithm {

void Unit2InlineFuncTest() {
  HERE(INFO);
  InlineFunc(10 + 2);
  InlineFunc(20 + 2);
  HERE(INFO);
}

void Unit2StaticFuncTest() {
  HERE(INFO);
  StaticFunc(10 + 2);
  StaticFunc(20 + 2);
  HERE(INFO);
}

void Unit2StaticInlineFuncTest() {
  HERE(INFO);
  StaticInlineFunc(10 + 2);
  StaticInlineFunc(20 + 2);
  HERE(INFO);
}

}  // namespace algorithm
