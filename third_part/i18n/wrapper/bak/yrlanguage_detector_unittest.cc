// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com(Lei Lu)
#include <string>

#include "testing/gtest/include/gtest/gtest.h"
#include "util/i18n/language.h"
#include "util/i18n/yrlanguage_detector.h"
#include "file/file.h"
#include "base/logging.h"

// test encoding detect function
TEST(YRLanguageDetector, LanguageDetect) {
  i18n::YRLanguageDetector detector;
  std::string content = "";

  LOG(INFO) << "-----------------------------Utf8";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/utf8.html",
                               &content);
  EXPECT_EQ(i18n::ZH_CN, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  LOG(INFO) << "-----------------------------Utf8_Janpan";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/utf8_jp.html",
                               &content);
  EXPECT_EQ(i18n::JA, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));



  // EUC-JP test
  LOG(INFO) << "-----------------------------EUC_JP";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/EUC_JP_1.html",
                               &content);
  EXPECT_EQ(i18n::JA, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // Shift_JIS test
  LOG(INFO) << "-----------------------------SHIFT_JIS";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/Shift_JIS_1.html",
                               &content);
  EXPECT_EQ(i18n::JA, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // GBK test
  LOG(INFO) << "----------------------------GBK";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk.html",
                               &content);
  EXPECT_EQ(i18n::ZH_CN, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // BIG5 test
  LOG(INFO) << "-----------------------------BIG5";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/big5.html",
                               &content);
  EXPECT_EQ(i18n::ZH_TW, detector.DetectLanguage(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));
}
