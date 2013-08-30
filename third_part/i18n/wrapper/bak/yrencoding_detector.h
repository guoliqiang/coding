// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_YRENCODING_DETECTOR_H_
#define UTIL_I18N_YRENCODING_DETECTOR_H_

#include "base/scoped_ptr.h"
#include "base/basictypes.h"
#include "util/i18n/encoding.h"
#include "util/i18n/language.h"
#include "util/i18n/encoding_detector.h"

namespace i18n {

class YREncodingDetectorHelper;

class YREncodingDetector : public EncodingDetector {
 public:
  YREncodingDetector();
  virtual ~YREncodingDetector();

  virtual Encoding DetectEncoding(const uint8* content,
                                  int64 content_size);

  // Return: true - is utf8, false - not utf8
  bool DetectUtf8(const uint8* content,
                  int64 content_size);

  Encoding GetMetaCharset(const uint8* content);
 private:
  // **********************
  // encoding detect func
  // **********************
  // GBK/GB2312/Big5
  // treat GB2312 as GBK as GB2312 is subset of GBK
  bool DetectDoubleBytesEncoding(const uint8* conent,
                                 int64 content_size,
                                 int64 encoding_possibility_array[]);
  // Shift_JIS
  bool DetectShiftJIS(const uint8* content,
                      int64 content_size,
                      int64 encoding_possibility_array[]);
  // EUC_JP
  bool DetectEUC_JP(const uint8* content,
                    int64 content_size,
                    int64 encoding_possibility_array[]);
  // try all the supported encoding to find out the most possible encoding
  // type, may loop the content string several times
  Encoding TryAllToDetectEncoding(const uint8* content,
                                  int64 content_size,
                                  int64 encoding_possibility_array[]);

  // **********************
  // helper funcs
  // **********************
  Encoding EncodingMapping(const char* charset);
  Encoding GetExpectedEncodingType(int64 encoding_possibility_array[]);
  int32 GetTwoByteValue(int32 head, int32 tail) {
    return ((head & 0x000000ff) << 8) | (tail & 0x000000ff);
  }

  scoped_ptr<YREncodingDetectorHelper> char_detector_;

  DISALLOW_COPY_AND_ASSIGN(YREncodingDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_YRENCODING_DETECTOR_H_
