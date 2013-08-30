// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#include "util/i18n/invalid_code_detector.h"

#include "base/yr.h"
#include "base/string_util.h"
#include "base/logging.h"
#include "file/file.h"

#define CHECK_MID(x) ((x & MID_M) >> 6 == MID_H)

namespace {
const uint32 ASC_M   = 0x80;
const uint32 TWO_M   = 0xE0;
const uint32 THREE_M = 0xF0;
const uint32 FOUR_M  = 0xF8;

const uint32 ASC_H   = 0x0;
const uint32 TWO_H   = 0x6;
const uint32 THREE_H = 0xE;
const uint32 FOUR_H  = 0x1E;

const uint32 MID_M   = 0xC0;
const uint32 MID_H   = 0x2;

const uint32 CJK_LB  = 0xe4b880;
const uint32 CJK_UB  = 0xe9BFBF;

const int kCommonChineseBound = 400;
const double kRatioToTotal = 0.25;
const double kRatioToChinese = 0.13;
const double kInvalidCodeDetectRatio = 0.01;
}  // namespace

namespace i18n {

bool InvalidCodeDetector::IsInRange(uint32 low_bound,
                                    uint32 up_bound,
                                    const uint8 *code) const {
  uint32 val;

  val = code[0] << 16 | code[1] << 8 | code[2];
  return val >= low_bound && val <= up_bound;
}

bool InvalidCodeDetector::IsValidCode(const string &content) const {
  size_t index = 0;
  size_t size = content.size();
  // Number of character that is out of utf-8 range.
  int32 invalid_num = 0;
  // Number of valid utf-8 character.
  int32 total_char_num = 0;
  // Number of uncommon used Chinese Character.
  int32 uncommon_chinese_num = 0;
  // Number of common used Chinese Character.
  int32 common_chinese_num = 0;

  if (content.empty()) {
    VLOG(3) << "Content is empty!";
    return true;
  }

  while (index < size) {
    // Jump over the comments in web page.
    if (content[index] == '<' && content[index+1] == '!') {
      index += 2;
      while (content[index] != '>' && index < size) {
        index++;
      }
      index++;
      continue;
    }
    if ((content[index] & ASC_M) >> 7 == ASC_H) {
      total_char_num++;
      index++;
    } else if ((content[index] & TWO_M) >> 5 == TWO_H) {
      if (index + 1 >= size) {
        invalid_num++;
        break;
      }
      if (!CHECK_MID(content[index + 1])) {
        invalid_num++;
        index++;
        continue;
      }
      total_char_num++;
      index += 2;
    } else if ((content[index] & THREE_M) >> 4 == THREE_H) {
      if (index + 2 >= size) {
        invalid_num++;
        break;
      }
      if (!(CHECK_MID(content[index + 1]) && CHECK_MID(content[index + 2]))) {
        invalid_num++;
        index++;
        continue;
      }
      const uint8 *code =
        reinterpret_cast<const uint8 *>(content.c_str() + index);
      if (IsInRange(CJK_LB, CJK_UB, code)) {
        if (!common_chinese_detector_.IsCommonChinese(code)) {
          uncommon_chinese_num++;
        } else {
          common_chinese_num++;
        }
      }
      total_char_num++;
      index += 3;
    } else if ((content[index] & FOUR_M) >> 3 == FOUR_H) {
      if (index + 3 >= size) {
        invalid_num++;
        break;
      }
      if (!(CHECK_MID(content[index + 1])
            && CHECK_MID(content[index + 2])
            && CHECK_MID(content[index + 3]))) {
        invalid_num++;
        index++;
        continue;
      }
      total_char_num++;
      index += 4;
    } else {
      invalid_num++;
      index++;
    }
  }

  if (invalid_num / static_cast<double>(size)
           > kInvalidCodeDetectRatio) {
    VLOG(1) << "Invalid char num ratio: "
            << invalid_num / static_cast<double>(size);
    return false;
  }

  if (uncommon_chinese_num > kCommonChineseBound &&
      (uncommon_chinese_num /
       static_cast<double>(uncommon_chinese_num + common_chinese_num))
         > kRatioToChinese) {
    VLOG(1) << "Uncommon chinese num ratio: "
            << uncommon_chinese_num /
               static_cast<double>(uncommon_chinese_num + common_chinese_num);
    return false;
  }

  if (uncommon_chinese_num > 0 &&
      (uncommon_chinese_num) /
       static_cast<double>(total_char_num) > kRatioToTotal) {
    VLOG(1) << "The ratio of uncommon chinese to valid character: "
            << ((uncommon_chinese_num + common_chinese_num) /
                static_cast<double>(total_char_num));
    return false;
  }

  return true;
}

}  // namespace i18n;
