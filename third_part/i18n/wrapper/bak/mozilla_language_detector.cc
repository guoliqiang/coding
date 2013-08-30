// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "util/i18n/mozilla_language_detector.h"

#include <string>

#include "base/yr.h"
#include "base/logging.h"
#include "util/i18n/mozilla_encoding_detector.h"
#include "third_party/i18n/language_detector/common.h"
#include "third_party/i18n/language_detector/textcat.h"
#include "util/i18n/encoding.h"
#include "util/release/release_path.h"

DEFINE_string(language_data_path,
              "util/i18n/LM",
              "data file folder path");

namespace i18n {

MozillaLanguageDetector::MozillaLanguageDetector() {
  std::string base_directory =
    release::GetReleaseYrDataPath(FLAGS_language_data_path);
  CHECK((handle_ = textcat_Init(base_directory.c_str())) != NULL);
}

MozillaLanguageDetector::~MozillaLanguageDetector() {
  textcat_Done(handle_);
}

i18n::Language MozillaLanguageDetector::DetectLanguage(
    const std::string &content) {
  i18n::Language lan = i18n::UNKNOWN_LANGUAGE;
  i18n::MozillaEncodingDetector chardet;
  i18n::Encoding encoding = chardet.DetectEncoding(content);

  if (content.empty()) {
    return i18n::UNKNOWN_LANGUAGE;
  }
  if (encoding == i18n::GB18030 || encoding == i18n::GBK || encoding
      == i18n::GB2312) {
    lan = i18n::ZH_CN;
  } else if (encoding == i18n::EUC_KR) {
    lan = i18n::KO;
  } else if (encoding == i18n::BIG5) {
    lan = i18n::ZH_TW;
  } else if (encoding == i18n::SHIFT_JIS || encoding == i18n::EUC_JP) {
    lan = i18n::JA;
  } else if (encoding == i18n::ASCII) {
    lan = i18n::EN;
  } else if (encoding == i18n::UTF8) {
    lan = DetectLanguageForUTF8(content);
  }
  return lan;
}

i18n::Language MozillaLanguageDetector::DetectLanguageForUTF8(
    const std::string &content) {
  if (content.empty()) {
    return i18n::UNKNOWN_LANGUAGE;
  }

  char *result = NULL;
  i18n::Language lan = i18n::UNKNOWN_LANGUAGE;
  result = textcat_Classify(handle_, content.c_str(), content.size());
  if (strcmp("english", result) == 0) {
    lan = i18n::EN;
  } else if (strcmp("chinese", result) == 0) {
    lan = i18n::ZH_CN;
  } else if (strcmp("japanese", result) == 0) {
    lan = i18n::JA;
  } else if (strcmp("korean", result) == 0) {
    lan = i18n::KO;
  } else if (strcmp("french", result) == 0) {
    lan = i18n::FR;
  } else if (strcmp("russian", result) == 0) {
    lan = i18n::RU;
  } else if (strcmp("arabic-iso8859_6", result) == 0 || strcmp(
        "arabic-windows1256", result) == 0) {
    lan = i18n::AR_SA;
  } else if (strcmp("spanish", result) == 0) {
    lan = i18n::ES;
  } else if (strcmp("german", result) == 0) {
    lan = i18n::DE;
  } else {
    lan = i18n::UNKNOWN_LANGUAGE;
  }
  return lan;
}

}  // namespace i18n
