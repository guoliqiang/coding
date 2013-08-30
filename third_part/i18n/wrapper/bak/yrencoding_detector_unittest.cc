// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com(Lei Lu)
#include <string>

#include "testing/gtest/include/gtest/gtest.h"
#include "util/i18n/encoding.h"
#include "util/i18n/yrencoding_detector.h"
#include "file/file.h"
#include "base/logging.h"

// test encoding detect function
TEST(YREncodingDetector, EncodingDetect) {
  i18n::YREncodingDetector detector;
  std::string content = "";

  LOG(INFO) << "-----------------------------Utf8";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/utf8.html",
                               &content);
  EXPECT_EQ(i18n::UTF8, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // EUC-JP test
  LOG(INFO) << "-----------------------------EUC_JP";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/EUC_JP_1.html",
                               &content);
  EXPECT_EQ(i18n::EUC_JP, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // Shift_JIS test
  LOG(INFO) << "-----------------------------SHIFT_JIS";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/Shift_JIS_1.html",
                               &content);
  EXPECT_EQ(i18n::SHIFT_JIS, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // GBK test
  LOG(INFO) << "-----------------------------GBK";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk.html",
                               &content);
  EXPECT_EQ(i18n::GBK, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  LOG(INFO) << "-----------------------------GBK_2";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk_2.html",
                               &content);
  EXPECT_EQ(i18n::GBK, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  LOG(INFO) << "-----------------------------GBK3";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk_3.html",
                               &content);
  EXPECT_EQ(i18n::GBK, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  LOG(INFO) << "-----------------------------GBK4";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk_4.html",
                               &content);
  EXPECT_EQ(i18n::GBK, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  LOG(INFO) << "-----------------------------GBK5";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/gbk_5.html",
                               &content);
  EXPECT_EQ(i18n::GBK, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));

  // BIG5 test
  LOG(INFO) << "-----------------------------BIG5";
  content = "";
  file::File::ReadFileToString("util/i18n/testdata/big5.html",
                               &content);
  EXPECT_EQ(i18n::BIG5, detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size()));
}
