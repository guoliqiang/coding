// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianma@yunrang.com (Ma Jian)

#ifndef UTIL_PRODUCTION_PRODUCTION_SAFETY_CHECKER_H_
#define UTIL_PRODUCTION_PRODUCTION_SAFETY_CHECKER_H_

#include <string>
namespace util {

class ProductionSafetyChecker {
 public:
  static bool CheckUser();
  static bool CheckRegisteredkNSPath(const std::string& full_path);
  static bool IsProduction();
  static std::string GetLoginName();
  static std::string GetUserName();
  static std::string GetEffectiveUserName();
  static std::string GetGroupName();
  static std::string GetEffectiveGroupName();
};

}  // namespace util

#endif  // UTIL_PRODUCTION_PRODUCTION_SAFETY_CHECKER_H_
