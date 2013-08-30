// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_YPATH_MASTER2_INTERNAL_PARSER_H_
#define UTIL_YPATH_MASTER2_INTERNAL_PARSER_H_

#include <map>
#include <vector>
#include <string>
#include "base/public/callback.h"
#include "third_part/parser_master/public/parse_master.h"
#include "../public/callback_define.h"

namespace parser_master {
class ParseMaster;
}

namespace path_master {
class PathItem;

class Parser {
 public:
  Parser();
  ~Parser();

  bool AddPath(const std::string& xpath,
               PathCallback callback,
               CallBackBindInfo* bind_info);

  inline std::vector<PathItem*>& paths() { 
    return paths_;
  }

  inline std::string& parser_name() {
    return parser_name_;
  }

  inline void set_force_check_attrs(
         std::vector<std::string>& force_check_attrs) {
    force_check_attrs_ = force_check_attrs;
  }

  inline const std::vector<std::string>& force_check_attrs() {
    return force_check_attrs_;
  }

 private:
  std::string parser_name_;
  std::vector<PathItem*> paths_;
  std::vector<std::string> force_check_attrs_;
  DISALLOW_COPY_AND_ASSIGN(Parser);
};
}
#endif  // UTIL_YPATH_MASTER2_INTERNAL_PARSER_H_
