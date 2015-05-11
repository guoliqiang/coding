// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-05-10 23:16:53
// File  : unit1.cc
// Brief :

#include "algorithm/static_inline/header.h"

namespace algorithm {

void Unit1InlineFuncTest() {
  HERE(INFO);
  InlineFunc(10 + 1);
  InlineFunc(20 + 1);
  HERE(INFO);
}

void Unit1StaticFuncTest() {
  HERE(INFO);
  StaticFunc(10 + 1);
  StaticFunc(20 + 1);
  HERE(INFO);
}

void Unit1StaticInlineFuncTest() {
  HERE(INFO);
  StaticInlineFunc(10 + 1);
  StaticInlineFunc(20 + 1);
  HERE(INFO);
}

}  // namespace algorithm
