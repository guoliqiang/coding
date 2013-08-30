// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#ifndef UTIL_I18N_ENCODING_DETECTOR_H_
#define UTIL_I18N_ENCODING_DETECTOR_H_

#include "base/basictypes.h"
#include "util/i18n/encoding.h"

namespace i18n {

class EncodingDetector {
 public:
  EncodingDetector() {}
  virtual ~EncodingDetector() {}

  virtual Encoding DetectEncoding(const uint8* content,
                                  int64 content_size) = 0;

  virtual bool DetectUtf8(const uint8* content,
                          int64 content_size) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(EncodingDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_ENCODING_DETECTOR_H_
