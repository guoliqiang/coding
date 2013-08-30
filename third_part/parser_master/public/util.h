// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jiannanli@yunrang.com (li jiannan)

#ifndef UTIL_PARSE_MASTER2_UTIL_UTIL_H_
#define UTIL_PARSE_MASTER2_UTIL_UTIL_H_

#include <string>


namespace parse_master {
class DomNode;

bool GetTrimedTextOfNode(const DomNode& node, std::string* out);
void GetCDATASectionText(const std::string& in, std::string* out);

}  // namespace parse_master

#endif  // UTIL_PARSE_MASTER2_UTIL_UTIL_H_
