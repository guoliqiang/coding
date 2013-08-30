// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_YRLANGUAGE_DETECTOR_H_
#define UTIL_I18N_YRLANGUAGE_DETECTOR_H_

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "util/i18n/encoding.h"
#include "util/i18n/language.h"
#include "util/i18n/language_detector.h"

namespace i18n {

class YREncodingDetector;
class YRLanguageDetectorHelper;

class YRLanguageDetector : public LanguageDetector {
 public:
  YRLanguageDetector();
  virtual ~YRLanguageDetector();

  virtual Language DetectLanguage(const uint8* content,
                                  int64 content_size);

  virtual Language DetectLanguageForUTF8(const uint8* content,
                                         int64 content_size);
 private:
  Language GetExpectedLanguageType(int64 language_possibility_array[]);

  scoped_ptr<YREncodingDetector> encoding_detector_;
  scoped_ptr<YRLanguageDetectorHelper> char_detector_;

  DISALLOW_COPY_AND_ASSIGN(YRLanguageDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_YRLANGUAGE_DETECTOR_H_
