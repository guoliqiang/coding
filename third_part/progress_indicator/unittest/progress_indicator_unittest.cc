// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)
#include <unistd.h>
#include "base/public/basictypes.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/progress_indicator/public/progress_indicator.h"

namespace util {

class ProgressIndicatorTest : public testing::Test {
  public:
  protected:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ProgressIndicatorTest, uint32Test) {
  ProgressIndicator<uint32> indicator(100);
  indicator.SetOutput(stderr);
  for (uint32 i = 0; i <= 100; i++) {
    indicator.SetValue(i);
    usleep(10000);
  }
}

TEST_F(ProgressIndicatorTest, uint64Test) {
  ProgressIndicator<uint64> indicator(100);
  for (uint64 i = 0; i <= 100; i++) {
    indicator.SetValue(i);
    usleep(10000);
  }
}

TEST_F(ProgressIndicatorTest, doubleTest) {
  ProgressIndicator<double> indicator(300);
  indicator.SetDescription("Test double:");
  for (int i = 0; i <= 300; i++) {
    indicator.SetValue(static_cast<double>(i));
    usleep(100000);
  }
}
TEST_F(ProgressIndicatorTest, noMaxTest) {
  ProgressIndicator<uint32> indicator(0);
  indicator.SetDescription("Test noMax:", "Unit");
  for (int i = 0; i <= 100; i++) {
    indicator.SetValue(static_cast<double>(i));
    usleep(100000);
  }
  indicator.Finish();
}

TEST_F(ProgressIndicatorTest, uint32TestOver) {
  ProgressIndicator<uint32> indicator(100);
  indicator.SetOutput(stderr);
  for (uint32 i = 0; i <= 102; i++) {
    indicator.SetValue(i);
    usleep(10000);
  }
}

}


