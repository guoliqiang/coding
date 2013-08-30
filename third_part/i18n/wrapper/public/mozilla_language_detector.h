// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#ifndef UTIL_I18N_MOZILLA_LANGUAGE_DETECTOR_H_
#define UTIL_I18N_MOZILLA_LANGUAGE_DETECTOR_H_

#include <string>

#include "language.h"
#include "encoding.h"

namespace i18n {
class MozillaLanguageDetector {
 public:
  MozillaLanguageDetector();
  ~MozillaLanguageDetector();
  i18n::Language DetectLanguage(const std::string &content);
  i18n::Language DetectLanguageForUTF8(const std::string &content);
  i18n::Language DetectLanguage(const std::string &content,
                                i18n::Encoding encoding_type);
 
 private:
  void* handle_;
};
}  // i18n

#endif  // UTIL_I18N_MOZILLA_LANGUAGE_DETECTOR_H_
