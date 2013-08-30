// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yaopengzeng@yunrang.com (YaoPeng Zeng)

#include <vector>

#include "base/flags.h"
#include "base/logging.h"
#include "util/i18n/yrencoding_dictionary.h"
#include "file/file.h"
#include "base/string_util.h"
#include "util/release/release_path.h"

DEFINE_string(big5_file,
              "big5_cmm.txt",
              "big5 word stock file");
DEFINE_string(euc_jp_file,
              "euc_jp_cmm.txt",
              "euc_jp word stock file");
DEFINE_string(gbk_file,
              "gbk_cmm.txt",
              "gbk word stock file");
DEFINE_string(shift_jis_file,
              "shift_jis_cmm.txt",
              "shift_jis word stock file");
DEFINE_string(unicode_simplify_chinese_file,
              "unicode_simplify_chinese_cmm.txt",
              "unicode zh-cn word stock file");
DEFINE_string(unicode_traditional_chinese_file,
              "unicode_traditional_chinese_cmm.txt",
              "unicode zh-tw word stock file");
DEFINE_string(dict_data_path,
              "util/i18n/",
              "data file folder path");

namespace i18n {


int32 YREncodingDictionary::big5_array_[TWO_BYTES_DICT_SIZE] = {0};
int32 YREncodingDictionary::euc_jp_array_[TWO_BYTES_DICT_SIZE] = {0};
int32 YREncodingDictionary::shift_jis_array_[TWO_BYTES_DICT_SIZE] = {0};
int32 YREncodingDictionary::gbk_array_[TWO_BYTES_DICT_SIZE] = {0};
int32 YREncodingDictionary::unicode_simplify_chinese_array_[TWO_BYTES_DICT_SIZE] = {0};
int32 YREncodingDictionary::unicode_traditional_chinese_array_[TWO_BYTES_DICT_SIZE] = {0};
bool YREncodingDictionary::initialized_ = false;

YREncodingDictionary::YREncodingDictionary() {
  CHECK(Initialize());
}

YREncodingDictionary::~YREncodingDictionary() {
}

bool YREncodingDictionary::Initialize() {
  if (initialized_) {
    return true;
  }

  std::string base_path = release::GetReleaseYrDataPath(FLAGS_dict_data_path);

  if (!InitCodecArray(big5_array_, base_path+FLAGS_big5_file)) {
    return false;
  }

  if (!InitCodecArray(euc_jp_array_, base_path+FLAGS_euc_jp_file)) {
    return false;
  }

  if (!InitCodecArray(shift_jis_array_, base_path+FLAGS_shift_jis_file)) {
    return false;
  }

  if (!InitCodecArray(gbk_array_, base_path+FLAGS_gbk_file)) {
    return false;
  }

  if (!InitCodecArray(unicode_simplify_chinese_array_,
                      base_path+FLAGS_unicode_simplify_chinese_file)) {
    return false;
  }

  if (!InitCodecArray(unicode_traditional_chinese_array_,
                      base_path+FLAGS_unicode_traditional_chinese_file)) {
    return false;
  }

  initialized_ = true;
  return true;
}

bool YREncodingDictionary::InitCodecArray(int32 array[],
                                        const std::string& file_path) {
  std::string data = "";
  if (!file::File::ReadFileToString(file_path, &data)) {
    LOG(ERROR) << "Failed to read dictionary file:" << file_path;
    return false;
  }

  std::vector<std::string> lines;
  SplitString(data, '\n', &lines);
  if (lines.size() == 0) {
    LOG(ERROR) << "invalid dictionary file content." << file_path;
    return false;
  }

  std::vector<std::string>::const_iterator itr = lines.begin();
  std::vector<std::string> pare;
  int binary_value = 0;
  bool res = false;
  int index = 0;
  while (itr != lines.end()) {
    index++;
    if (index >= lines.size() &&
        (*itr).length() == 0) {
      break;
    }

    pare.clear();
    SplitString(*itr, ' ', &pare);

    if (pare.size() != 2) {
      LOG(ERROR) << "invalid line. file:" << file_path
                 << ", line:" << *itr << ", index:" << index;
      return false;
    }

    res = HexStringToInt(pare[0], &binary_value);
    if (!res || binary_value <= 0 || binary_value > TWO_BYTES_DICT_SIZE) {
      LOG(ERROR) << "invalid encoding value. file:" << file_path
                 << ", line:" << *itr << ",value:" << binary_value;
      return false;
    }

    array[binary_value] = 1;
    itr++;
  }

  return true;
}

bool YREncodingDictionary::IsInDictionary(Encoding codec, int32 binary_value) {
  if (binary_value < 0 || binary_value > TWO_BYTES_DICT_SIZE) {
    LOG(ERROR) << "Invalid binary value(should in [0x00,0xff])" << binary_value;
    return false;
  }

  if (codec == BIG5) {
    return (big5_array_[binary_value] != 0);
  } else if (codec == EUC_JP) {
    return (euc_jp_array_[binary_value] != 0);
  } else if (codec == SHIFT_JIS) {
    return (shift_jis_array_[binary_value] != 0);
  } else if (codec == GBK) {
    return (gbk_array_[binary_value] != 0);
  } else {
    LOG(ERROR) << "Invalid codec type:" << codec;
    return false;
  }
}

bool YREncodingDictionary::IsInSimplifyChineseUnicodeDict(int32 binary_value) {
  if (binary_value < 0 || binary_value > TWO_BYTES_DICT_SIZE) {
    LOG(ERROR) << "Invalid binary value(should in [0x00,0xff])" << binary_value;
    return false;
  }

  return (unicode_simplify_chinese_array_[binary_value] != 0);
}

bool YREncodingDictionary::IsInTraditionalChineseUnicodeDict(int32 binary_value) {
  if (binary_value < 0 || binary_value > TWO_BYTES_DICT_SIZE) {
    LOG(ERROR) << "Invalid binary value(should in [0x00,0xff])" << binary_value;
    return false;
  }

  return (unicode_traditional_chinese_array_[binary_value] != 0);
}

}  // namespace i18n
