// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#ifndef UTIL_I18N_MOZILLA_ENCODING_DETECTOR_H_
#define UTIL_I18N_MOZILLA_ENCODING_DETECTOR_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "third_party/i18n/encoding_detector/nsUniversalDetector.h"
#include "util/i18n/encoding.h"

namespace i18n {

class MozillaEncodingDetector : public nsUniversalDetector {
 public:
  MozillaEncodingDetector();
  virtual ~MozillaEncodingDetector();

  i18n::Encoding DetectEncoding(const std::string &content);
  bool DetectUtf8(const std::string &content);
  const std::string GetEncoding(i18n::Encoding enc);

 protected:
  virtual void Report(const char* aCharset);

 private:
  std::map<std::string, i18n::Encoding> encoding_map_;
  std::string default_encoding_;
  char* ToUpper(char *src);

  DISALLOW_COPY_AND_ASSIGN(MozillaEncodingDetector);
};

}  // namespace i18n

#endif  // UTIL_I18N_MOZILLA_ENCODING_DETECTOR_H_
