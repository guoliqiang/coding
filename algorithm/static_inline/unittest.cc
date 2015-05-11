// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-05-10 23:26:15
// File  : unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "algorithm/static_inline/inline.h"

namespace algorithm {

void Unit1InlineFuncTest();
void Unit1StaticFuncTest();
void Unit1StaticInlineFuncTest();

void Unit2InlineFuncTest();
void Unit2StaticFuncTest();
void Unit2StaticInlineFuncTest();

}  // namespace algorithm


TEST(StaticInline, InlineFunc) {
  algorithm::Unit1InlineFuncTest();
  algorithm::Unit2InlineFuncTest();
}

TEST(StaticInline, StaticFunc) {
  algorithm::Unit1StaticFuncTest();
  algorithm::Unit2StaticFuncTest();
}

TEST(StaticInline, StaticInlineFunc) {
  algorithm::Unit1StaticInlineFuncTest();
  algorithm::Unit2StaticInlineFuncTest();
}

TEST(StaticInline, GlobalInline) {
  algorithm::GlobalInline(100);
}

TEST(StaticInline, MemberInline) {
  algorithm::FooInline foo;
  foo.FooInlineFunc(100);
}
