// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-24 16:18:27
// File  : statck_container_unittest.cc
// Brief :

#include "third_part/stack_container/public/stack_container.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/logging.h"

TEST(StackContainer, Nornal) {
  StackVector<int, 3> vec;
  for (int i = 0; i < 10; i++) vec->push_back(i);
  for (int i = 0; i < vec->size(); i++) LOG(INFO) << vec[i];
}
