// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "util/i18n/fast_language_detector.h"

#include <string>
#include <algorithm>

#include "base/basictypes.h"
#include "base/yr.h"
#include "base/logging.h"
#include "base/utf_string_conversions.h"
#include "util/i18n/mozilla_encoding_detector.h"
#include "util/i18n/encoding.h"
#include "util/release/release_path.h"
#include "util/translator/chinese_translator_static.h"
#include "third_party/icu/uchar.h"

namespace i18n {
using translator::kTraToSimData;

bool FastLanguageDetector::traditional_chs_[65536];

void FastLanguageDetector::InitModule() {
  for (size_t c = 0; c <= 65535; ++c) {
    traditional_chs_[c] = false;
  }
  const size_t len = sizeof(kTraToSimData) / sizeof(kTraToSimData[0]);
  for (size_t i = 0; i < len; ++i) {
    FastLanguageDetector::traditional_chs_[kTraToSimData[i].from] = true;
  }
  // exceptions
  traditional_chs_[26296] = false;  // 26296 暸
  traditional_chs_[33879] = false;  // 33879 著
  traditional_chs_[20094] = false;  // 33879 乾
  traditional_chs_[34249] = false;  // 34249 藉
}

REGISTER_MODULE_INITIALIZER(fast_lang_detect, \
    FastLanguageDetector::InitModule());

bool FastLanguageDetector::IsJapanese(base_icu::UChar32 cp) {
  UBlockCode c = ublock_getCode(cp);
  switch (c) {
  case UBLOCK_HIRAGANA:
  case UBLOCK_KATAKANA:
  case UBLOCK_KATAKANA_PHONETIC_EXTENSIONS:
      return true;
    default:
      return false;
  }
}

bool FastLanguageDetector::IsKorean(base_icu::UChar32 cp) {
  UBlockCode c = ublock_getCode(cp);
  switch (c) {
  case UBLOCK_HANGUL_JAMO:
  case UBLOCK_HANGUL_COMPATIBILITY_JAMO:
  case UBLOCK_HANGUL_SYLLABLES:  // 11184 字
      return true;
    default:
      return false;
  }
}

bool FastLanguageDetector::IsTradictionalChs(base_icu::UChar32 c) {
  if (c >= 0xFFFF) return false;
  return traditional_chs_[c];
}

FastLanguageDetector::FastLanguageDetector() {
}

FastLanguageDetector::~FastLanguageDetector() {
}


i18n::Language FastLanguageDetector::DetectLanguage(const uint8* content,
    int64 content_size) {
  string text = string(reinterpret_cast<const char*>(content), content_size);
  i18n::MozillaEncodingDetector chardet;
  i18n::Encoding encoding = chardet.DetectEncoding(text);

  if (text.empty()) {
    return i18n::UNKNOWN_LANGUAGE;
  }
  if (encoding == i18n::GB18030 || encoding == i18n::GBK || encoding
      == i18n::GB2312) {
    return i18n::ZH_CN;
  } else if (encoding == i18n::EUC_KR) {
    return i18n::KO;
  } else if (encoding == i18n::BIG5) {
    return i18n::ZH_TW;
  } else if (encoding == i18n::SHIFT_JIS || encoding == i18n::EUC_JP) {
    return i18n::JA;
  } else if (encoding == i18n::ASCII) {
    return i18n::EN;
  } else if (encoding == i18n::UTF8) {
    return DetectLanguageForUTF8(text);
  }
  return i18n::UNKNOWN_LANGUAGE;
}

static bool IsASCII(UChar32 c) {
  return c < 0xFF;
}
static bool IsHanzi(UChar32 ch) {
  return ch >= 0x4E00 && ch <= 0x9FCB;
}

// TODO(james): other language
i18n::Language FastLanguageDetector::DetectLanguageForUTF8(
    const string &content) const {
  if (content.empty()) {
    return i18n::UNKNOWN_LANGUAGE;
  }

  string16 s16 = UTF8ToUTF16(content);
  if (std::find_if(s16.begin(), s16.end(), FastLanguageDetector::IsJapanese) !=
      s16.end()) {
    return i18n::JA;
  }
  if (std::find_if(s16.begin(), s16.end(), FastLanguageDetector::IsKorean) !=
      s16.end()) {
    return i18n::KO;
  }
  if (std::find_if(s16.begin(), s16.end(),
                   FastLanguageDetector::IsTradictionalChs) != s16.end()) {
    return i18n::ZH_TW;
  }
  if (std::find_if(s16.begin(), s16.end(), IsHanzi) !=
      s16.end()) {
    return i18n::ZH_CN;
  }
  if (std::count_if(s16.begin(), s16.end(), IsASCII) ==
      s16.size()) {
    return i18n::EN;
  }
  return i18n::UNKNOWN_LANGUAGE;
}

}  // namespace i18n
