// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_YPATH_MASTER2_INTERNAL_YPATH_ITEM_H_
#define UTIL_YPATH_MASTER2_INTERNAL_YPATH_ITEM_H_

#include <map>
#include <vector>
#include <string>

#include "base/public/scoped_ptr.h"
#include "base/public/callback.h"
#include "third_part/parser_master/public/parse_master.h"
#include "../public/callback_define.h"

namespace parser_master {
class ParseMaster;
class DomNode;
}

namespace path_master {

struct PathTagAttribute {
  //  If is regx, value is a regx, we can use re2 to match
  //  If not, value is expected value of a tag attribute,
  std::string value;
  bool is_regx;
  PathTagAttribute() : is_regx(false) {}
};

struct PathTag {
  std::string name;
  std::vector<std::pair<std::string, PathTagAttribute> > attributes;
  int index;

  PathTag() : index(-1) {}
  void Clear() {
    name.clear();
    attributes.clear();
    index = -1;
  }
};

struct PathInfo {
  std::vector<PathTag> tags;
};

//  path holder class
class PathItem {
 public:
  PathItem(const std::string& path,
           PathCallback callback,
           CallBackBindInfo* bind_info);

  ~PathItem();
  bool ParseXPath();
  bool RunCallBackFuntion(ParserParam* param);

  inline std::string& path() { 
    return path_;
  }

  CallBackBindInfo* bind_info() {
    return bind_info_;
  }

  const PathInfo& path_info() {
    return path_info_;
  }

 private:
  void ParseTag(const parse_master::DomNode& node);
  void ParseIndex(const parse_master::DomNode& node);
  //  For debugging
  void PrintXPathInfo();

 private:
  std::string path_;
  PathCallback callback_;
  PathInfo path_info_;  // std::vector<PathTag>
  PathTag current_tag_;
  bool bad_xpath_;
  CallBackBindInfo* bind_info_;
  DISALLOW_COPY_AND_ASSIGN(PathItem);
};
}
#endif  // UTIL_YPATH_MASTER2_INTERNAL_YPATH_ITEM_H_
