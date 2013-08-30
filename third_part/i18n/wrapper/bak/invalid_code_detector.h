// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#ifndef UTIL_I18N_INVALID_CODE_DETECTOR_H_
#define UTIL_I18N_INVALID_CODE_DETECTOR_H_

#include <string>

#include "base/basictypes.h"
#include "util/i18n/common_chinese_detector.h"

namespace i18n {
class CommonChineseDetector;

class InvalidCodeDetector {
 public:
  InvalidCodeDetector() {}
  ~InvalidCodeDetector() {}

  bool IsValidCode(const std::string &page_content) const;

 private:
  bool IsInRange(uint32 low_bound, uint32 up_bound, const uint8 *code) const;
  i18n::CommonChineseDetector common_chinese_detector_;
  DISALLOW_COPY_AND_ASSIGN(InvalidCodeDetector);
};
}  // namespace i18n

#endif  // UTIL_I18N_INVALID_CODE_DETECTOR_H_
