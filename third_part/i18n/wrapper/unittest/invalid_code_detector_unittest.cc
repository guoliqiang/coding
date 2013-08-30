// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include <string>

#include "../public/invalid_code_detector.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

using std::string;

namespace i18n {
TEST(InvalidCodeDetectorUnitTest, InvalidCode) {
  i18n::InvalidCodeDetector detector;
  std::string output = "";

  file::File::ReadFileToString("./testdata/utf8.html", &output);
  EXPECT_EQ(detector.IsValidCode(output), true);

  output.clear();
  file::File::ReadFileToString("./testdata/non_utf8.html", &output);
  EXPECT_EQ(detector.IsValidCode(output), false);

  output.clear();
  EXPECT_EQ(detector.IsValidCode(output), true);
}
}  // namespace i18n
