// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-09-21 23:27:33
// File  : time_limit_data_unittest.cc
// Brief :

#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "base/public/time_limit_data.h"
#include "base/public/time.h"
#include "base/public/logging.h"

TEST(TimeLimitData, CountFind) {
  base::TimeLimitKVData<std::string, std::string> data;
  data.Add("key1", "value1", 2);
  EXPECT_EQ(data.Count("key1"), true);
  base::MilliSleep(3000);
  EXPECT_EQ(data.Count("key1"), false);
  data.Add("key1", "value1", 2);
  base::shared_ptr<std::string> v = data.Find("key1");
  CHECK(v.get() != NULL);
  base::MilliSleep(3000);
  v = data.Find("key1");
  CHECK(v.get() == NULL);
}

TEST(TimeLimitData, Size) {
  base::TimeLimitKVData<std::string, std::string> data;
  data.Add("key1", "value1", 2);
  data.Add("key2", "value2", 4);
  base::MilliSleep(3000);
  CHECK(data.ImpreciseSize() == 2);
  CHECK(data.PreciseSize() == 1);
  CHECK(data.ImpreciseSize() == 1);
}

TEST(TimeLimitData, Iterator) {
  base::TimeLimitKVData<std::string, std::string> data;
  data.Add("key1", "value1", 2);
  data.Add("key2", "value2", 4);
  base::MilliSleep(3000);
  int cnt = 0;
  for (data.Begin(); !data.IsEnd(); data.Next()) {
    base::shared_ptr<std::string> v = data.CurrentValue();
    if (v.get() != NULL) {
      CHECK(*v.get() == "value2");
      cnt++;
    }
  }
  CHECK(cnt == 1);
}

TEST(TimeLimitData, IteratorEx) {
  base::TimeLimitKVData<std::string, std::string> data;
  data.Add("key1", "value1", 2);
  data.Add("key2", "value2", 4);
  base::MilliSleep(3000);
  int cnt = 0;
  for (data.Begin(); !data.IsEnd(); data.Next()) {
    base::MilliSleep(2000);
    base::shared_ptr<std::string> v = data.CurrentValue();
    CHECK(v.get() == NULL);
  }
  CHECK(cnt == 0);
}

