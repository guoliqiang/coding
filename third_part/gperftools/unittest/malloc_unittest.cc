// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-08 15:11:46
// File  : malloc_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

TEST(Gperftools, Normal) {
  for (int i = 0; i < 100000; i++) {
    char * ptr = new char[4014];
    delete [] ptr;
  }
}

