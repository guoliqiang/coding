// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#ifndef UTIL_I18N_YRENCODING_DICTIONARY_H_
#define UTIL_I18N_YRENCODING_DICTIONARY_H_

#include <string>

#include "base/hash_tables.h"
#include "base/scoped_ptr.h"
#include "util/i18n/encoding.h"

namespace i18n {

class YREncodingDictionary {
 public:
  static const int64 TWO_BYTES_DICT_SIZE = 0x10000;

  YREncodingDictionary();
  ~YREncodingDictionary();

  bool IsInDictionary(Encoding codec, int32 binary_value);

  bool IsInSimplifyChineseUnicodeDict(int32 binary_value);
  bool IsInTraditionalChineseUnicodeDict(int32 binary_value);

 private:
  static bool InitCodecArray(int32 array[], const std::string& file_path);

  // load data file, initialize dicts
  static bool Initialize();

  static bool initialized_;

  // follows array stores binary value of frequently used charactors,
  // and only for two bytes encoding part
  static int32 big5_array_[TWO_BYTES_DICT_SIZE];
  static int32 euc_jp_array_[TWO_BYTES_DICT_SIZE];
  static int32 shift_jis_array_[TWO_BYTES_DICT_SIZE];
  static int32 gbk_array_[TWO_BYTES_DICT_SIZE];
  static int32 unicode_simplify_chinese_array_[TWO_BYTES_DICT_SIZE];
  static int32 unicode_traditional_chinese_array_[TWO_BYTES_DICT_SIZE];

  DISALLOW_COPY_AND_ASSIGN(YREncodingDictionary);
};

}  // namespace i18n

#endif  // UTIL_I18N_YRENCODING_DICTIONARY_H_
