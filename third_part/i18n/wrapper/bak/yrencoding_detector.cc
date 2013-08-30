// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include "base/logging.h"
#include "util/i18n/yrencoding_detector.h"
#include "util/i18n/yrencoding_dictionary.h"
#include "util/i18n/yrencoding_detector_helper.h"

namespace i18n {

namespace {
// 以GBK为例子来说明:
// eg.
// 如果GBK常用字的编码占到了70%, 则认为该文档一定是GBK(70%~100%)
// 如果GBK常用字的编码小于20%,则认为一定不可能时GBK
// 如果所有的编码类型的有效字符都占到了20%~70%, 则取百分比最高的那一个
static const uint64 ENCODING_JUDGE_HIGH_THRESHOLD = 70;
static const uint64 ENCODING_JUDGE_LOW_THRESHOLD = 20;
}

YREncodingDetector::YREncodingDetector()
    : char_detector_(new YREncodingDetectorHelper()) {
}

YREncodingDetector::~YREncodingDetector() {
}

Encoding YREncodingDetector::DetectEncoding(
    const uint8* content,
    int64 content_size) {
  if (content_size == 0) {
    return INVALID_ENCODING;
  }
  Encoding meta_type = GetMetaCharset(content);

  bool detect_finish = false;
  Encoding detect_type = INVALID_ENCODING;
  int64 encoding_possibility_array[ENCODING_TYPE_COUNT] = {0};
  if (meta_type == UTF8) {
    detect_finish = DetectUtf8(content, content_size);
    detect_type = detect_finish ? UTF8 : INVALID_ENCODING;
  } else if (meta_type == GB2312 ||
             meta_type == GBK ||
             meta_type == BIG5) {
    // join all the pure double bytes encoding together to
    // reduce extra loop times
    detect_finish = DetectDoubleBytesEncoding(
        content, content_size, encoding_possibility_array);
    detect_type = detect_finish ?
      GetExpectedEncodingType(encoding_possibility_array) :
      INVALID_ENCODING;
  } else if (meta_type == EUC_JP) {
    detect_finish = DetectEUC_JP(
        content, content_size, encoding_possibility_array);
    detect_type = detect_finish ? EUC_JP : INVALID_ENCODING;
  } else if (meta_type == SHIFT_JIS) {
    detect_finish = DetectShiftJIS(
        content, content_size, encoding_possibility_array);
    detect_type = detect_finish ? SHIFT_JIS : INVALID_ENCODING;
  } else if (meta_type == EUC_KR) {
    return INVALID_ENCODING;
  } else {
    detect_finish = false;
    detect_type = INVALID_ENCODING;
  }

  if (detect_finish) {
    // detect result is the same as meta-charset, so there is no need to
    // check other detecting types.
    return detect_type;
  } else {
    // try all supported encoding types to get the most possible encoding type
    return TryAllToDetectEncoding(
        content, content_size, encoding_possibility_array);
  }
}

// Return: true - is utf8, false - not utf8
bool YREncodingDetector::DetectUtf8(
    const uint8* content,
    int64 content_size) {
  int64 current_index = 0;
  while (current_index < content_size) {
    if (*(content+current_index) <= 0x7f) {
      ++current_index;
      continue;
    }

    if (!char_detector_->IsUtf8(content, content_size, current_index)) {
      return false;
    }
  }
  return true;
}

// GBK/GB2312/Big5
bool YREncodingDetector::DetectDoubleBytesEncoding(
    const uint8* content,
    int64 content_size,
    int64 encoding_possibility_array[]) {
  int64 current_index = 0;
  int32 binary_value = 0;
  int64 total = 0;  // double bytes charactor's total amount
  int64 gbk_possibility = 0;
  int64 big5_possibility = 0;
  while (current_index < content_size) {
    // escape 1~127(ASCII)
    if (*(content+current_index) <= 0x7f) {
      ++current_index;
      continue;
    }

    if (current_index+1 >= content_size) {
      break;
    }

    // convert follows 2 bytes to binary_value
    binary_value = GetTwoByteValue(*(content+current_index),
                                   *(content+current_index+1));

    if (char_detector_->IsGBK(binary_value)) {
      ++gbk_possibility;
    }

    if (char_detector_->IsBig5(binary_value)) {
      ++big5_possibility;
    }
    current_index += 2;
    ++total;
  }

  if (total == 0) {
    return false;
  }

  Encoding type = gbk_possibility > big5_possibility ? GBK : BIG5;
  int64 max_value = gbk_possibility > big5_possibility ?
                    gbk_possibility : big5_possibility;
  uint64 passed_percent = (max_value*100)/total;
  encoding_possibility_array[type] = passed_percent;
  if (passed_percent >= ENCODING_JUDGE_HIGH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

// Shift_JIS
bool YREncodingDetector::DetectShiftJIS(
    const uint8* content,
    int64 content_size,
    int64 encoding_possibility_array[]) {
  int64 current_index = 0;
  int32 binary_value = 0;
  int64 total = 0;  // double bytes charactor's total amount
  int64 possibility = 0;
  while (current_index < content_size) {
    // escape 1~127(ASCII)
    if (*(content+current_index) <= 0x7f) {
      ++current_index;
      continue;
    }

    // 跳过假名
    if (*(content+current_index) >= 0xa1 &&
        *(content+current_index) >= 0xdf) {
      ++current_index;
      continue;
    }

    if (current_index+1 >= content_size) {
      break;
    }

    // convert follows 2 bytes to binary_value
    binary_value = GetTwoByteValue(*(content+current_index),
                                   *(content+current_index+1));

    if (char_detector_->IsShiftJis(binary_value)) {
      ++possibility;
    }
    current_index += 2;
    ++total;
  }

  if (total == 0) {
    return false;
  }
  uint64 passed_percent = (possibility*100)/total;
  encoding_possibility_array[SHIFT_JIS] = passed_percent;
  if (passed_percent >= ENCODING_JUDGE_HIGH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

// EUC_JP
bool YREncodingDetector::DetectEUC_JP(
    const uint8* content,
    int64 content_size,
    int64 encoding_possibility_array[]) {
  int64 current_index = 0;
  int32 binary_value = 0;
  int64 total = 0;  // double bytes charactor's total amount
  int64 possibility = 0;
  while (current_index < content_size) {
    // escape 1~127(ASCII)
    if (*(content+current_index) <= 0x7f) {
      ++current_index;
      continue;
    }

    // 跳过假名(double bytes)
    if (*(content+current_index)   == 0x8e &&
        *(content+current_index+1) >= 0xa1 &&
        *(content+current_index+1) <= 0xdf) {
      current_index += 2;
      continue;
    }

    // escape 3 bytes caracters
    if (*(content+current_index)   == 0x8f &&
        *(content+current_index+1) >= 0xa1 &&
        *(content+current_index+1) <= 0xfe &&
        *(content+current_index+2) >= 0xa1 &&
        *(content+current_index+2) <= 0xfe) {
      current_index += 3;
      continue;
    }

    if (current_index+1 >= content_size) {
      break;
    }

    // convert follows 2 bytes to binary_value
    binary_value = GetTwoByteValue(*(content+current_index),
                                   *(content+current_index+1));
    if (char_detector_->IsEucJp(binary_value)) {
      ++possibility;
    }
    current_index += 2;
    ++total;
  }

  if (total == 0) {
    return false;
  }

  uint64 passed_percent = (possibility*100)/total;
  encoding_possibility_array[EUC_JP] = passed_percent;
  if (passed_percent >= ENCODING_JUDGE_HIGH_THRESHOLD) {
    return true;
  } else {
    return false;
  }
}

// try all the supported encoding to find out the most possible encoding
// type, may loop the content string several times
Encoding YREncodingDetector::TryAllToDetectEncoding(
    const uint8* content,
    int64 content_size,
    int64 encoding_possibility_array[]) {
  if (encoding_possibility_array[GBK] == 0 &&
      encoding_possibility_array[BIG5] == 0) {
    if (DetectDoubleBytesEncoding(
        content, content_size, encoding_possibility_array)) {
      return GetExpectedEncodingType(encoding_possibility_array);
    }
  }

  if (encoding_possibility_array[EUC_JP] == 0) {
    if (DetectEUC_JP(content, content_size, encoding_possibility_array)) {
      return EUC_JP;
    }
  }

  if (encoding_possibility_array[SHIFT_JIS] == 0) {
    if (DetectShiftJIS(content, content_size, encoding_possibility_array)) {
      return SHIFT_JIS;
    }
  }

  if (DetectUtf8(content, content_size)) {
      return UTF8;
  }


  // none of the detecting function successed
  return GetExpectedEncodingType(encoding_possibility_array);
}


Encoding YREncodingDetector::GetMetaCharset(const uint8* content) {
  if (content == NULL) {
    return INVALID_ENCODING;
  }

  const char* charset =
    strcasestr(reinterpret_cast<const char*>(content), "Content-Type");
  if (charset == NULL) {
    return INVALID_ENCODING;
  }
  const char* meta_end = strstr(charset, ">");
  if (meta_end == NULL) {
    return INVALID_ENCODING;
  }

  charset = strcasestr(charset, "charset");
  if (charset == NULL || charset >= meta_end) {
    return INVALID_ENCODING;
  }
  charset = strstr(charset, "=");
  if (charset == NULL || charset >= meta_end) {
    return INVALID_ENCODING;
  }

  charset += strlen("=");
  while ((*charset == ' ' || *charset == '\t') && charset < meta_end) {
    ++charset;
  }

  return EncodingMapping(charset);
}

Encoding YREncodingDetector::EncodingMapping(const char* charset) {
  if (charset == NULL) {
    return INVALID_ENCODING;
  }

  static const char* EncodingName[] =
  {"INVALID_ENCODING", "ascii", "utf-8", "gb2312", "gbk",
    "big5", "euc-jp", "shift_jis", "euc-kr"};

  for (int index = 0; index < sizeof(EncodingName)/sizeof(charset); index++) {
    if (strncasecmp(charset, EncodingName[index],
          strlen(EncodingName[index])) == 0) {
      return static_cast<Encoding>(index);
    }
  }

  return INVALID_ENCODING;
}

Encoding YREncodingDetector::GetExpectedEncodingType(
    int64 encoding_possibility_array[]) {
  int pivot_index = INVALID_ENCODING;
  int pivot = encoding_possibility_array[pivot_index];

  if (encoding_possibility_array[GBK] != 0) {
    encoding_possibility_array[GBK] += encoding_possibility_array[GB2312];
  }

  for (int index = 0; index < ENCODING_TYPE_COUNT; index++) {
    if (encoding_possibility_array[index] > pivot) {
      pivot_index = index;
      pivot = encoding_possibility_array[index];
    }
  }

  // All charactors are 0~127
  if (encoding_possibility_array[pivot_index] == 0) {
    pivot_index = ASCII;
  } else if (encoding_possibility_array[pivot_index] <
      ENCODING_JUDGE_LOW_THRESHOLD) {
    pivot_index = INVALID_ENCODING;
  }

  return static_cast<Encoding>(pivot_index);
}

}  // namespace i18n
