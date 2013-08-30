// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-30 17:10:21
// File  : arena_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "../public/arena.h"

class ArenaUnittest: public testing::Test {
};

class TestArena {
 public:
  explicit TestArena(int flag = 0) {
    LOG(INFO) << "call TestArena";
    flag_ = flag;
  }
  ~TestArena() {
    LOG(INFO) << "call ~TestArena";
  }

  void set_flag(int flag) {
    flag_ = flag;
  }

  int get_flag() {
    return flag_;
  }

  int flag_;
};

TEST_F(ArenaUnittest, test1) {
  util::arena::UnsafeArena arena_instance(1000);
  TestArena * ptr = new(&arena_instance) TestArena(10);
  EXPECT_EQ(ptr->get_flag(), 10);
  ptr->~TestArena();
  arena_instance.Reset();
}
