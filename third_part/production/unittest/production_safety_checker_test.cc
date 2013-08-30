// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#include "util/production/production_safety_checker.h"
#include <stdlib.h>

#include "testing/gtest/include/gtest/gtest.h"

namespace util {

class CheckerTest: public testing::Test {
 public:
  CheckerTest() {}

  void SetUp() {
  }

  void TearDown() {
  }
};

TEST_F(CheckerTest, IsProductionTest) {
  unsetenv("YR_PRODUCTION");
  EXPECT_FALSE(ProductionSafetyChecker::IsProduction());
  setenv("YR_PRODUCTION", "72e50dbd", 1);
  EXPECT_TRUE(ProductionSafetyChecker::IsProduction());
}

TEST_F(CheckerTest, IDTest) {
  unsetenv("YR_PRODUCTION");
  EXPECT_TRUE(ProductionSafetyChecker::CheckUser());
  setenv("YR_PRODUCTION", "72e50dbd", 1);
  EXPECT_FALSE(ProductionSafetyChecker::CheckUser());
}

TEST_F(CheckerTest, YRNSProductionTest) {
  setenv("YR_PRODUCTION", "72e50dbd", 1);
  EXPECT_TRUE(
      ProductionSafetyChecker::CheckRegisteredkYRNSPath("/YRNS/testing/babla"));
  EXPECT_TRUE(
      ProductionSafetyChecker::CheckRegisteredkYRNSPath("/YRNS/sina/babla"));
}
TEST_F(CheckerTest, YRNSNonProductionTest) {
  unsetenv("YR_PRODUCTION");
  EXPECT_TRUE(
      ProductionSafetyChecker::CheckRegisteredkYRNSPath("/YRNS/testing/babla"));
  EXPECT_FALSE(
      ProductionSafetyChecker::CheckRegisteredkYRNSPath("/YRNS/sina/babla"));
}

}  // namespace util
