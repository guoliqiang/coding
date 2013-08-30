// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com(Huiqiang Yang)

#include <string>

#include "base/logging.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/i18n/language.h"
#include "util/i18n/fast_language_detector.h"
#include "util/parse_master2/public/parse_master.h"

namespace i18n {
// test encoding detect function
TEST(FastLanguageDetector, LanguageDetect) {
  FastLanguageDetector d;

  EXPECT_EQ(i18n::ZH_CN, d.DetectLanguageForUTF8("中文"));
  EXPECT_EQ(i18n::ZH_CN, d.DetectLanguageForUTF8("中文 chinese"));
  EXPECT_EQ(i18n::ZH_CN, d.DetectLanguageForUTF8("中文 chinese 123.344$#$"));

  EXPECT_EQ(i18n::ZH_TW, d.DetectLanguageForUTF8("繁體字"));
  EXPECT_EQ(i18n::ZH_TW, d.DetectLanguageForUTF8("中文台灣"));

  EXPECT_EQ(i18n::EN, d.DetectLanguageForUTF8("english"));
  EXPECT_EQ(i18n::EN, d.DetectLanguageForUTF8("english 123.344$#$"));

  EXPECT_EQ(i18n::JA, d.DetectLanguageForUTF8("'かれか'"));
  EXPECT_EQ(i18n::JA, d.DetectLanguageForUTF8("セントバレンタインデー日文"));

  EXPECT_EQ(i18n::KO, d.DetectLanguageForUTF8("두 사람이 하께"));
  EXPECT_EQ(i18n::KO, d.DetectLanguageForUTF8("english 두 사람이 하께"));
}

}  // namespace i18n
