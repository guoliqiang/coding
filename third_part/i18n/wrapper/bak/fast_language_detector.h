// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (James Gao)

#ifndef UTIL_I18N_FAST_LANGUAGE_DETECTOR_H_
#define UTIL_I18N_FAST_LANGUAGE_DETECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "base/icu_utf.h"
#include "base/scoped_ptr.h"
#include "util/i18n/encoding.h"
#include "util/i18n/language.h"
#include "util/i18n/language_detector.h"

namespace i18n {

/* Difference from MozillaEncodingDetector, YRLanguageDetector:
 * This class focus on detecting language from short query or word,
 * unicode char categories is used to check language.
 * MozillaEncodingDetector or YRLanguageDetector use the statistics method,
 * which is more suitable for web document which have long content.
 */
class FastLanguageDetector : public LanguageDetector {
 public:
  static void InitModule();
  static bool IsJapanese(base_icu::UChar32 c);
  static bool IsKorean(base_icu::UChar32 c);
  static bool IsTradictionalChs(base_icu::UChar32 c);

  FastLanguageDetector();

  virtual ~FastLanguageDetector();

  virtual Language DetectLanguage(const uint8* content,
                                  int64 content_size);

  virtual Language DetectLanguageForUTF8(const uint8* content,
                                         int64 content_size) {
    std::string s(reinterpret_cast<const char*>(content), content_size);
    return DetectLanguageForUTF8(s);
  }

  virtual Language DetectLanguageForUTF8(const std::string& text) const;

 private:
  static bool traditional_chs_[65536];

  DISALLOW_COPY_AND_ASSIGN(FastLanguageDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_FAST_LANGUAGE_DETECTOR_H_
