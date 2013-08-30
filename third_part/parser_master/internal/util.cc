// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jiannanli@yunrang.com (li jiannan)

#include "third_part/parser_master/public/util.h"
#include "base/public/string_util.h"
#include "third_part/string/public/utf8_util.h"
#include "third_part/parser_master/public/parse_master.h"
#include "third_part/parser_master/public/dom_node.h"

using parse_master::ParseMaster;
using parse_master::DomNode;
using parse_master::HtmlTag;
using std::string;

namespace parse_master {

bool GetTrimedTextOfNode(const DomNode& node, string* out) {
  string str;
  node.GetEscapedContent(&str);
  TrimWhitespace(str, TRIM_ALL, out);
  // TrimUTF8 will not set output if nothing trimed
  util::utf8_util::TrimUTF8(*out, " ", out);
  return !out->empty();
}

//  If text is in CDATA, return real text, if not , return original text
void GetCDATASectionText(const string& in, string* out) {
  //  CDATA section begins with "<![CDATA[", and ends with "]]>"
  const string CDATAPrefix = "<![CDATA[";
  const string CDATAPostfix = "]]>";
  if (StartsWithASCII(in, CDATAPrefix, true) &&
      EndsWith(in, CDATAPostfix, true)) {
    string text(in.begin() + CDATAPrefix.length(),
        in.end() - CDATAPostfix.length());
    TrimWhitespace(text, TRIM_ALL, out);
  } else {
    out->assign(in);
  }
}

}  // namespace parse_master
