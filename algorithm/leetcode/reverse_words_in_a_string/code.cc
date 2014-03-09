// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-03-10 02:18:54
// File  : code.cc
// Brief :

/*

22 / 22 test cases passed.
Status: Accepted
Runtime: 44 ms
Submitted: 0 minutes ago

*/

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string Reverse(const std::string & str) {
  std::string rs = "";
  int b = 0;
  while (b < str.size()) {
    while (str[b] == ' ' || str[b] == '\t') b++;
    if (b == str.size()) break;
    int e = b;
    while (str[e] != ' ' && str[e] != '\t' && e < str.size()) e++;
    if (rs.size() != 0) rs.insert(rs.begin(), 1, ' ');
    rs.insert(rs.begin(), &(str[b]), &(str[e]));
    b = e;
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  std::string str = "   the    sky is blue  ";
  LOG(INFO) << str;
  str = Reverse(str);
  LOG(INFO) << str;
  return 0;
}
