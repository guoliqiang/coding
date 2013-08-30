// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com(Huiqiang Yang)

#include <string>

#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "../public/language.h"
#include "../public/mozilla_language_detector.h"
#include "third_part/parser_master/public/parse_master.h"

using parse_master::ParseMaster;
using parse_master::DomNode;

namespace {

const char remove_chars[] = {' ', '\n', '\t', '\r', '\f', };

class LanguageDetectorHelper {
 public:
  LanguageDetectorHelper() {
    pm_.RegistParserOnText(base::NewPermanentCallback(this,
          &LanguageDetectorHelper::ParseOnText));
  }

  void ParseOnText(const DomNode& node) {
    std::string output = "";
    RemoveChars(node.GetContent(), remove_chars, &output);
    text_ += output;
  }

  i18n::Language Detect(const std::string& content) {
    text_.clear();
    pm_.SetBuff(content.data(), content.length());
    pm_.Parse();
    return detector_.DetectLanguage(text_);
  }
 private:
  i18n::MozillaLanguageDetector detector_;
  std::string text_;
  ParseMaster pm_;
};
}

// test encoding detect function
TEST(MozillaLanguageDetector, LanguageDetect) {
  std::string content = "";
  LanguageDetectorHelper detect_helper;

  LOG(INFO) << "-----------------------------Utf8";
  content = "";
  file::File::ReadFileToString("./testdata/utf8.html",
                               &content);
  EXPECT_EQ(i18n::ZH_CN, detect_helper.Detect(content));


  LOG(INFO) << "-----------------------------Utf8_Janpan";
  content = "";
  file::File::ReadFileToString("./testdata/utf8_jp.html",
                               &content);
  EXPECT_EQ(i18n::JA, detect_helper.Detect(content));

  // EUC-JP test
  LOG(INFO) << "-----------------------------EUC_JP";
  content = "";
  file::File::ReadFileToString("./testdata/EUC_JP_1.html",
                               &content);
  EXPECT_EQ(i18n::JA, detect_helper.Detect(content));

  // Shift_JIS test
  LOG(INFO) << "-----------------------------SHIFT_JIS";
  content = "";
  file::File::ReadFileToString("./testdata/Shift_JIS_1.html",
                               &content);
  EXPECT_EQ(i18n::JA, detect_helper.Detect(content));

  // GBK test
  LOG(INFO) << "-----------------------------GBK";
  content = "";
  file::File::ReadFileToString("./testdata/gbk.html",
                               &content);
  EXPECT_EQ(i18n::ZH_CN, detect_helper.Detect(content));

  // BIG5 test
  LOG(INFO) << "-----------------------------BIG5";
  content = "";
  file::File::ReadFileToString("./testdata/big5.html",
                               &content);
  EXPECT_EQ(i18n::ZH_TW, detect_helper.Detect(content));

  LOG(INFO) << "-----------------------------Utf8_ZH";
  content = "";
  file::File::ReadFileToString("./testdata/utf8.html",
                               &content);
  EXPECT_EQ(i18n::ZH_CN, detect_helper.Detect(content));


  LOG(INFO) << "-----------------------------Utf8_JP";
  content = "";
  file::File::ReadFileToString("./testdata/utf8_japan.html",
                               &content);
  EXPECT_EQ(i18n::JA, detect_helper.Detect(content));
}
