// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "util/i18n/mozilla_encoding_detector.h"

#include <string.h>
#include <stdio.h>

#include "base/logging.h"
#include "third_party/i18n/encoding_detector/CharDistribution.h"
#include "third_party/i18n/encoding_detector/nsUniversalDetector.h"
#include "util/i18n/encoding.h"

namespace i18n {

MozillaEncodingDetector::MozillaEncodingDetector() :nsUniversalDetector() {
  int enc = static_cast<int>(i18n::INVALID_ENCODING);

  for (; enc < static_cast<int>(i18n::ENCODING_TYPE_COUNT); ++enc) {
    encoding_map_.insert(std::pair<std::string, i18n::Encoding>(
        i18n::EncodingType2String(enc), static_cast<i18n::Encoding>(enc)));
  }
}

MozillaEncodingDetector::~MozillaEncodingDetector() {
}

i18n::Encoding MozillaEncodingDetector::DetectEncoding(
    const std::string &content) {
  if (content.empty()) {
    return i18n::UNKNOWN_ENCODING;
  }
  default_encoding_.assign("INVALID_ENCODING");
  Reset();
  HandleData(content.c_str(), static_cast<uint32>(content.size()));
  DataEnd();

  std::map<std::string, i18n::Encoding>::iterator iter =
        encoding_map_.find(default_encoding_);
  if (iter == encoding_map_.end()) {
    LOG(WARNING) << "Not found suitable encoding! " << default_encoding_;
    return i18n::INVALID_ENCODING;
  } else if (iter->second == GBK || iter->second == GB2312) {
    // use its sup-set for encoding
    return GB18030;
  } else {
    return iter->second;
  }
}

bool MozillaEncodingDetector::DetectUtf8(const std::string &content) {
  return DetectEncoding(content) == i18n::UTF8;
}

const std::string MozillaEncodingDetector::GetEncoding(i18n::Encoding enc) {
  return i18n::EncodingType2String(enc);
}

void MozillaEncodingDetector::Report(const char* char_set) {
  char buffer[512] = "";
  snprintf(buffer, sizeof(buffer), "%s", char_set);
  default_encoding_.assign(ToUpper(buffer));
}

char* MozillaEncodingDetector::ToUpper(char *src) {
  char *ptr = src;
  while (*ptr != '\0') {
    if (islower(*ptr))
      *ptr = toupper(*ptr);
    if ('-' == *ptr)
      *ptr = '_';
    ++ptr;
  }
  return src;
}

}  // namespace i18n
