// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include "util/i18n/yrlanguage_detector.h"

#include "base/logging.h"
#include "util/i18n/yrencoding_detector.h"
#include "util/i18n/yrlanguage_detector_helper.h"

namespace i18n {

namespace {
static const uint64 LANGUAGE_JUDGE_THRESHOLD = 80;
static const uint64 LANGUAGE_JUDGE_THRESHOLD_UTF8 = 50;
}

YRLanguageDetector::YRLanguageDetector()
    : encoding_detector_(new YREncodingDetector()),
      char_detector_(new YRLanguageDetectorHelper()) {
}

YRLanguageDetector::~YRLanguageDetector() {
}


Language YRLanguageDetector::DetectLanguage(const uint8* content,
                                            int64 content_size) {
  if (content_size <= 0) {
    VLOG(2) << "Empty content can't be dectected.";
    return UNKNOWN_LANGUAGE;
  }

  Encoding codec =
      encoding_detector_->DetectEncoding(content, content_size);

  if (codec == UTF8) {
    return DetectLanguageForUTF8(content, content_size);
  } else if (codec == GBK) {
    return ZH_CN;
  } else if (codec == BIG5) {
    return ZH_TW;
  } else if (codec == EUC_JP || codec == SHIFT_JIS) {
    return  JA;
  } else if (codec == ASCII) {
    return EN;
  } else {
    return UNKNOWN_LANGUAGE;
  }
}

Language YRLanguageDetector::DetectLanguageForUTF8(
    const uint8* content, int64 content_size) {
  if (content_size == 0) {
    return UNKNOWN_LANGUAGE;
  }

  int64 language_possibility_array[LANGUAGE_TYPE_COUNT] = {0};
  int64 current_index = 0;
  int64 total = 0;
  while (current_index < content_size) {
    if (*(content+current_index) >= 0x00 && *(content+current_index) <= 0x7f) {
      ++current_index;
      continue;
    }

    char_detector_->GetLanguageForUtf8(
        content, content_size, &current_index, language_possibility_array);

    ++total;
  }

  if (total == 0) {
    return EN;
  }

  Language type = GetExpectedLanguageType(language_possibility_array);
  uint64 passed_percent = (language_possibility_array[type]*100)/total;
  if (passed_percent > LANGUAGE_JUDGE_THRESHOLD_UTF8) {
    return type;
  } else {
    return UNKNOWN_LANGUAGE;
  }
}

Language YRLanguageDetector::GetExpectedLanguageType(
    int64 language_possibility_array[]) {
  int pivot_index = UNKNOWN_LANGUAGE;
  int pivot = language_possibility_array[pivot_index];

  for (int index = 0; index < LANGUAGE_TYPE_COUNT; index++) {
    if (language_possibility_array[index] > pivot) {
      pivot_index = index;
      pivot = language_possibility_array[index];
    }
  }

  return static_cast<Language>(pivot_index);
}

}  // namespace i18n
