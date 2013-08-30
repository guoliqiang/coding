// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_YRLANGUAGE_DETECTOR_HELPER_H_
#define UTIL_I18N_YRLANGUAGE_DETECTOR_HELPER_H_

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "util/i18n/language.h"

namespace i18n {

class YREncodingDictionary;

// This calss detect encoding type or language for one or several
// bytes that is identified as a "charactor" under some encoding.
class YRLanguageDetectorHelper {
 public:
  YRLanguageDetectorHelper();
  ~YRLanguageDetectorHelper();

  void GetLanguageForUtf8(const uint8* content,
                          int64 content_size,
                          int64* index_start,
                          int64 language_possibility_arra[]);

 private:
  scoped_ptr<YREncodingDictionary> dict_;
};
}  // namespace i18n

#endif  // UTIL_I18N_YRLANGUAGE_DETECTOR_HELPER_H_
