// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-08 01:42:46
// File  : disk_cache.h
// Brief :

#include "base/public/logging.h"
#include "../public/disk_cache.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

namespace cache {
TEST(LRUCacheTest, Normal) {
  DiskCache<int> foo("/tmp/dcache");
  foo.SetData(1, "foo");
  std::string data;
  foo.GetData(1, &data);
  EXPECT_EQ(data, "foo");

  foo.SetData(2, "bar");
  foo.GetData(2, &data);
  EXPECT_EQ(data, "bar");

  foo.SetData(1, "test");
  foo.GetData(1, &data);
  EXPECT_EQ(data, "test");

  EXPECT_EQ(foo.GetData(1), true);
  EXPECT_EQ(foo.GetData(2), true);
  EXPECT_EQ(foo.GetData(3), false);
}

TEST(LRUCacheTest, SameKey) {
  DiskCache<int> foo("/tmp/dcache", 2, 10);
  foo.SetData(1, "www.pku.edu.cn");
  std::string data;
  foo.GetData(1, &data);
  EXPECT_EQ(data, "www.pku.edu.cn");

  data = "";
  foo.SetData(2, "guoliqiang2006@pku.edu.cn");
  foo.GetData(2, &data);
  EXPECT_EQ(data, "");

  EXPECT_EQ(foo.GetData(1), false);
  foo.SetData(1, "www.pku.edu.cn");
  foo.GetData(1, &data);
  EXPECT_EQ(data, "www.pku.edu.cn");
}
}  // namespace cache
