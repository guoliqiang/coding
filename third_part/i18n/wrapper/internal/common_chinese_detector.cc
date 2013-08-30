// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "../public/common_chinese_detector.h"

#include <set>
#include <string>
#include <vector>

#include "base/public/string_util.h"
#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/release/public/release_path.h"

DEFINE_string(common_simplify_chinese_code_path,
              "third_part/i18n/utf8_simplify_chinese_cmm.txt",
              "specify common chinese code path");
DEFINE_string(common_traditional_chinese_code_path,
              "third_part/i18n/utf8_traditional_chinese_cmm.txt",
              "specify common chinese code path");
using std::string;

namespace i18n {

std::set<int32> CommonChineseDetector::char_set_;

CommonChineseDetector::CommonChineseDetector() {
  std::string common_simplify_chinese_code_path =
    release::GetReleaseDataPath(FLAGS_common_simplify_chinese_code_path);
  std::string common_traditional_chinese_code_path =
    release::GetReleaseDataPath(FLAGS_common_traditional_chinese_code_path);
  CHECK(LoadCharSet(common_simplify_chinese_code_path));
  CHECK(LoadCharSet(common_traditional_chinese_code_path));
}

bool CommonChineseDetector::LoadCharSet(string path) {
  string data;

  if (!file::File::ReadFileToString(path, &data)) {
    LOG(ERROR) << "Failed to read dictionary file:"
      << path;
    return false;
  }

  std::vector<string> lines;
  SplitString(data, '\n', &lines);
  if (lines.size() == 0) {
    LOG(ERROR) << "invalid dictionary file content."
      << path;
    return false;
  }

  std::vector<string>::const_iterator itr = lines.begin();
  std::vector<string> pair;
  int32 binary_value = 0;
  bool res = false;
  while (itr != lines.end() - 1) {
    pair.clear();
    SplitString(*itr, ' ', &pair);

    if (pair.size() != 2) {
      LOG(ERROR) << "invalid line. file:" << path
                 << ", line:" << itr - lines.begin();
      return false;
    }

    res = HexStringToInt(pair[0], &binary_value);
    if (!res || binary_value <= 0 || binary_value > MAX_CHINESE_CODE) {
      LOG(ERROR) << "invalid encoding value. file:"
                 << path
                 << ", line:" << itr - lines.begin()
                 << ",value:" << binary_value;
      return false;
    }

    char_set_.insert(binary_value);
    itr++;
  }

  return true;
}

bool CommonChineseDetector::IsCommonChinese(const uint8 *code) const {
  int32 val;

  val = code[0] << 16 | code[1] << 8 | code[2];

  return char_set_.count(val) > 0;
}
}  // namespace i18n
