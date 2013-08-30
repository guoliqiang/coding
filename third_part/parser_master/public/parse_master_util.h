// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_UTIL_H_
#define UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_UTIL_H_

#include <map>
#include <string>
#include <vector>

#include "base/public/logging.h"
#include "parse_master.h"

namespace parse_master {
class TitleParser {
 public:
  explicit TitleParser(string* str) {
    title_ = str;
  }

  void OnTitleClose(const DomNode& titleNode) {
    titleNode.GetFirstTopContent(title_);
  }

  const string* GetTitle() {
    return title_;
  }

 private:
  string* title_;
};

class AnchorParser {
 public:
  explicit AnchorParser(vector<const DomNode*>* anchors) {
    anchors_ = anchors;
  }

  void OnAnchorOpen(const DomNode& anchorNode) {
    anchors_->push_back(&anchorNode);
  }
 private:
  vector<const DomNode*>* anchors_;
};

class ParseMasterUtil {
 public:
  static void GetTitle(const char* buff, const int len,
                       ParseMaster* pm, string* str) {
    pm->SetBuff(buff, len);

    TitleParser tp(str);
    pm->RegistParserOnTagClose(HtmlTag::TITLE, base::NewPermanentCallback(&tp,
        &TitleParser::OnTitleClose));
    pm->Parse();
    /*
    std::string seral_str;
    pm->SerializeToString(true, &seral_str);
    LOG(INFO) << "\n" << seral_str;
    */
  }

  static void GetAnchors(const char* buff, const int len,
                         ParseMaster* pm, vector<const DomNode*>* anchors) {
    pm->SetBuff(buff, len);

    AnchorParser ap(anchors);
    pm->RegistParserOnTagOpen(HtmlTag::A, base::NewPermanentCallback(&ap,
                              &AnchorParser::OnAnchorOpen));
    pm->Parse();
    /*
    std::string seral_str;
    pm->SerializeToString(true, &seral_str);
    LOG(INFO) << seral_str;
    */
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseMasterUtil);
};
}  // namespace parse_master

#endif  // UTIL_PARSE_MASTER2_PUBLIC_PARSE_MASTER_UTIL_H_
