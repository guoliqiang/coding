// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include <string>

#include "base/yr.h"
#include "util/i18n/invalid_code_detector.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace i18n {
TEST(InvalidCodeDetectorUnitTest, InvalidCode) {
  i18n::InvalidCodeDetector detector;
  std::string output = "";

  file::File::ReadFileToString("util/i18n/testdata/utf8.html", &output);
  EXPECT_EQ(detector.IsValidCode(output), true);

  output.clear();
  file::File::ReadFileToString("util/i18n/testdata/non_utf8.html", &output);
  EXPECT_EQ(detector.IsValidCode(output), false);

  output.clear();
  EXPECT_EQ(detector.IsValidCode(output), true);
}
}  // namespace i18n
