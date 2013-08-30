// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "util/i18n/mozilla_encoding_detector.h"

#include <string>

#include "base/yr.h"
#include "base/logging.h"
#include "util/i18n/invalid_code_detector.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace i18n {
TEST(MozillaEncodingDetector, EncodingDetect) {
  i18n::MozillaEncodingDetector detector;
  std::string content;

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/utf8.html", &content);
  EXPECT_EQ(detector.DetectUtf8(content), true);

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/EUC_JP_1.html", &content);
  EXPECT_EQ(i18n::EUC_JP, detector.DetectEncoding(content));

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/Shift_JIS_1.html", &content);
  EXPECT_EQ(i18n::SHIFT_JIS, detector.DetectEncoding(content));

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/gbk.html", &content);
  EXPECT_EQ(i18n::GB18030, detector.DetectEncoding(content));

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/big5.html", &content);
  EXPECT_EQ(i18n::BIG5, detector.DetectEncoding(content));

  content.clear();
  file::File::ReadFileToString("util/i18n/testdata/empty.html", &content);
  EXPECT_EQ(i18n::UNKNOWN_ENCODING, detector.DetectEncoding(content));
}
}  // namespace i18n
