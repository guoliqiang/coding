// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_YRENCODING_DETECTOR_HELPER_H_
#define UTIL_I18N_YRENCODING_DETECTOR_HELPER_H_

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "util/i18n/language.h"

namespace i18n {

class YREncodingDictionary;

// This calss detect encoding type or language for one or several
// bytes that is identified as a "charactor" under some encoding.
class YREncodingDetectorHelper {
 public:
  YREncodingDetectorHelper();
  ~YREncodingDetectorHelper();

  bool IsUtf8(const uint8* content,
              int64 content_size,
              int64& index_start);

  // follows 'Is*' functions don't judge with the encoding range,
  // they just check the most frequently used charactor
  bool IsGBK(const int32& binary_value);
  bool IsBig5(const int32& binary_value);
  bool IsShiftJis(const int32& binary_value);
  bool IsEucJp(const int32& binary_value);

 private:
  scoped_ptr<YREncodingDictionary> dict_;

  DISALLOW_COPY_AND_ASSIGN(YREncodingDetectorHelper);
};

}  // namespace i18n

#endif  // UTIL_I18N_YRENCODING_DETECTOR_HELPER_H_
