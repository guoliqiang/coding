// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include "util/i18n/yrencoding_detector_helper.h"

#include "util/i18n/encoding.h"
#include "util/i18n/yrencoding_dictionary.h"

namespace i18n {

YREncodingDetectorHelper::YREncodingDetectorHelper()
    : dict_(new YREncodingDictionary()) {
}

YREncodingDetectorHelper::~YREncodingDetectorHelper() {
}

bool YREncodingDetectorHelper::IsUtf8(const uint8* content,
                                  int64 content_size,
                                  int64& index_start) {
  int value_byte = *(content+index_start) & 0xff;
  int count = 0;
  while (value_byte & 0x80) {
    ++count;
    value_byte <<= 1;
  }

  if (count >= 2 && index_start+count <= content_size) {
    for (int offset = 1; offset < count; ++offset) {
      if ((*(content+index_start+offset) & 0xc0) != 0x80) {
        return false;
      }
    }
    index_start += count;
  } else {
    return false;
  }
  return true;
}

bool YREncodingDetectorHelper::IsGBK(const int32& binary_value) {
  return dict_->IsInDictionary(GBK, binary_value);
}

bool YREncodingDetectorHelper::IsBig5(const int32& binary_value) {
  return dict_->IsInDictionary(BIG5, binary_value);
}

bool YREncodingDetectorHelper::IsShiftJis(const int32& binary_value) {
  return dict_->IsInDictionary(SHIFT_JIS, binary_value);
}

bool YREncodingDetectorHelper::IsEucJp(const int32& binary_value) {
  return dict_->IsInDictionary(EUC_JP, binary_value);
}

}  // namespace i18n
