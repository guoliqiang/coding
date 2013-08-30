// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: huiqiangyang@yunrang.com (Huiqiang Yang)

#ifndef UTIL_I18N_COMMON_CHINESE_DETECTOR_H_
#define UTIL_I18N_COMMON_CHINESE_DETECTOR_H_

#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/logging.h"

namespace i18n {
class CommonChineseDetector {
 public:
  CommonChineseDetector();
  ~CommonChineseDetector() {}
  bool IsCommonChinese(const uint8 *code) const;
 private:
  static const int32 MAX_CHINESE_CODE = 0xE9BFBF;
  static bool LoadCharSet(std::string path);
  static std::set<int32> char_set_;
  DISALLOW_COPY_AND_ASSIGN(CommonChineseDetector);
};
}  // namespace i18n

#endif  // UTIL_I18N_COMMON_CHINESE_DETECTOR_H_
