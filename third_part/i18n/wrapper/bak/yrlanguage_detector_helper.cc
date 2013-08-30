// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include "util/i18n/yrlanguage_detector_helper.h"

#include "util/i18n/encoding.h"
#include "util/i18n/yrencoding_dictionary.h"

namespace i18n {

YRLanguageDetectorHelper::YRLanguageDetectorHelper()
    : dict_(new YREncodingDictionary()) {
}

YRLanguageDetectorHelper::~YRLanguageDetectorHelper() {
}

void YRLanguageDetectorHelper::GetLanguageForUtf8(
              const uint8* content,
              int64 content_size,
              int64* index_start,
              int64 language_possibility_array[]) {
  int64 begin = *index_start;
  // get the length of utf8 charactor
  int value_byte = *(content+begin) & 0xff;
  int offset = 0;
  while (value_byte & 0x80) {
    ++offset;
    value_byte <<= 1;
  }

  if (offset < 2 || begin+offset > content_size) {
    ++(*index_start);
    return;
  }

  *index_start = begin + offset;

  // get the int value of unciode
  const int mask_utf8[] = {0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc};
  int binary_value = *(content+begin) & ~mask_utf8[offset];

  for (int index = 1; index < offset; ++index) {
    if ((*(content+begin+index) & 0xc0) != 0x80) {
      return;
    }
    binary_value =
      (binary_value << 6) | (*(content+begin+index) & ~mask_utf8[0]);
  }

  bool isSimplifyChinese = false;
  bool isTraditionalChinese = false;
  if (binary_value < YREncodingDictionary::TWO_BYTES_DICT_SIZE &&
      binary_value >= 0) {
    isSimplifyChinese =
      dict_->IsInSimplifyChineseUnicodeDict(binary_value);

    isTraditionalChinese =
      dict_->IsInTraditionalChineseUnicodeDict(binary_value);
  }

  if (isSimplifyChinese) {
    ++language_possibility_array[ZH_CN];
  }

  if (isTraditionalChinese) {
    ++language_possibility_array[ZH_TW];
  }

  if (!isSimplifyChinese &&
      !isTraditionalChinese) {
    ++language_possibility_array[UNKNOWN_LANGUAGE];
  }
}

}  // namespace i18n
