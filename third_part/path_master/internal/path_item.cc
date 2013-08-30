// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "../public/path_item.h"
#include <algorithm>
#include "base/public/singleton.h"
#include "base/public/string_util.h"
#include "base/public/string_split.h"
#include "third_part/re2/include/re2/re2.h"

using std::string;

namespace path_master {

using parse_master::ParseMaster;
using parse_master::DomNode;
using parse_master::HtmlAttrIterator;

PathItem::PathItem(const string& path,
                   PathCallback callback,
                   CallBackBindInfo* bind_info): path_(path),
                                                 callback_(callback),
                                                 bad_xpath_(false),
                                                 bind_info_(bind_info) {}

PathItem::~PathItem() {
  delete callback_;
}

bool PathItem::ParseXPath() {
  parse_master::ParseMaster pm;
  pm.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(this, &PathItem::ParseTag));
  pm.RegistParserOnText(
      base::NewPermanentCallback(this, &PathItem::ParseIndex));
  pm.SetBuff(path_);
  pm.Parse();

  //  Save last tag info for ypath
  if (!current_tag_.name.empty()) {
    path_info_.tags.push_back(current_tag_);
    current_tag_.Clear();
  }
  //  PrintXPathInfo();
  return !bad_xpath_;
}

bool PathItem::RunCallBackFuntion(ParserParam* param) {
  return callback_->Run(param);
}

void PathItem::ParseTag(const parse_master::DomNode& node) {
  if (!current_tag_.name.empty()) {
    path_info_.tags.push_back(current_tag_);
    current_tag_.Clear();
  }
  current_tag_.name = node.GetLowerTagName();
  for (HtmlAttrIterator it = node.GetAttrIter(); it.NotEnd(); it.Next()) {
    const string* attr_key = it.GetName();
    const string* attr_value = it.GetValue();
    PathTagAttribute attr;
    //  If attribute value is reg(...), treat it as regx
    if (attr_value) {
      if (StartsWithASCII(*attr_value, "reg(", false) &&
          EndsWith(*attr_value,  ")", false)) {
        attr.value = attr_value->substr(strlen("reg("),
                                        attr_value->length() - 5);
        attr.is_regx = true;
      } else {
        attr.value = *attr_value;
        attr.is_regx = false;
      }
    }
    current_tag_.attributes.push_back(make_pair(*attr_key, attr));
  }
}

void PathItem::ParseIndex(const parse_master::DomNode& node) {
  const string& content = node.GetContent();
  int index = -1;
  //  Make it static so all YPathItem instance can share a re2
  //  object, and re2 is recompiled.
  static re2::RE2 extract_index_re2("\\[(\\d+)\\]");
  if (!re2::RE2::FullMatch(content, extract_index_re2, &index)) {
    LOG(ERROR) << "bad index info for ypath:"
               << path_ << ":" << content;
    bad_xpath_ = true;
    return;
  }
  VLOG(4) << content << ",index:" << index;
  CHECK_GE(index, 0) << "bad index for this path:" << path_;
  current_tag_.index = index;
}

void PathItem::PrintXPathInfo() {
  for (size_t i = 0; i < path_info_.tags.size(); ++i) {
    LOG(INFO) << "tag name:" << path_info_.tags[i].name;
    LOG(INFO) << "index:" << path_info_.tags[i].index;
    const std::vector<std::pair<std::string, PathTagAttribute> >& attrs =
        path_info_.tags[i].attributes;
    for (size_t i = 0; i <  attrs.size(); ++i) {
      LOG(INFO) << attrs[i].first << ":" << attrs[i].second.value
                << ", is regx:" << attrs[i].second.is_regx;
    }
  }
}
}  //  namespace
