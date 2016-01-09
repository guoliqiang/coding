// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 22:59:20
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::string IntToString(int v) {
  char buff[100] = {0};
  sprintf(buff, "%d", v);
  return std::string(buff);
}

int IntToString(const std::string & str) {
  return atoi(str.c_str());
}

class Codec {
 public:
  // Encodes a list of strings to a single string.
  std::string encode(std::vector<std::string>& strs) {
    std::string ans;
    for (int i = 0; i < strs.size(); i++) {
      ans.append(IntToString(strs[i].size()));
      ans.append(1, '\0');
      ans.append(strs[i]);
    }
    return ans;
  }

  // Decodes a single string to a list of strings.
  std::vector<std::string> decode(std::string s) {
    std::vector<std::string> ans;
    std::string cur;
    int idx = 0;
    while (idx < s.size()) {
      if (s[idx] == '\0') {
        int len = StringToInt(cur);
        cur.clear();
        ans.push_back(s.substr(idx + 1, len));
        idx += 1 + len;
      } else {
        cur.push_back(s[idx]);
        idx++;
      }
    }
    return ans;
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> vec;
  vec.push_back("a");
  vec.push_back("bc");
  vec.push_back("m");
  vec.push_back("jklleousw");
  Codec foo;
  std::string str = foo.encode(vec);
  LOG(INFO) << JoinVector(foo.decode(str));
  return 0;
}
