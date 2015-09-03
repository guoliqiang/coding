// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 21:16:48
// File  : raw_merge_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/snowflake/public/id_worker.h"

namespace snowflake {


TEST(IDWorker, Normal) {
  IDWorker worker(0xFF);
  for (int i = 0; i < 100; i++) {
    LOG(INFO) << worker.NextID();
  }
}

}  // namespace snowflake
