// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#ifndef UTIL_I18N_LANGUAGE_DETECTOR_H_
#define UTIL_I18N_LANGUAGE_DETECTOR_H_

#include "base/basictypes.h"
#include "util/i18n/language.h"

namespace i18n {

class LanguageDetector {
 public:
  LanguageDetector() {}
  virtual ~LanguageDetector() {}

  virtual Language DetectLanguage(const uint8* content,
                                  int64 content_size) = 0;

  virtual Language DetectLanguageForUTF8(const uint8* content,
                                         int64 content_size) = 0;
 private:
  DISALLOW_COPY_AND_ASSIGN(LanguageDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_LANGUAGE_DETECTOR_H_
