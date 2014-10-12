// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-10-12 11:33:41
// File  : btree_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

class Base {};
class SubA: public Base {};
class ClassB {};

TEST(Btree, Simple) {
  EXPECT_EQ(1, (std::is_same<int, int>::value));
  EXPECT_NE((std::is_same<double, int>::value), 1);
  EXPECT_NE((std::is_same<int32_t, int64_t>::value), 1);
  EXPECT_EQ(1, (std::is_convertible<SubA, Base>::value));
  EXPECT_NE(1, (std::is_convertible<ClassB, Base>::value));
}
