// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 12:06:46
// File  : code.cc
// Brief :

/*
 * Write a function to find the longest common prefix string amongst an array of strings.
 *
 * */

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 117/117 test cases passed.
 *
 * */

#include "base/public/logging.h"

namespace algorithm {

std::string LongestCommnPrefix(std::vector<std::string> & v) {
  if (v.size() == 0) return "";
  if (v.size() == 1) return v[0];

  std::vector<const char *> ptr;
  for (int i = 0; i < v.size(); i++) {
    ptr.push_back(v[i].c_str());
  }
  bool flag = true;
  int num = 0;
  while (flag) {
    for (int i = 1; i < ptr.size(); i++) {
      if (*(ptr[0]) != *(ptr[i]++)) {
        flag = false;
        break;
      }
    }
    ptr[0]++;
    if (*(ptr[0] - 1) == '\0') break;
    if (flag) num++;
  }
  // LOG(INFO) << num;
  return v[0].substr(0, num);
}

}// namespace algorithm

using namespace algorithm;

namespace twice {
class Solution {
 public:
  std::string longestCommonPrefix(std::vector<std::string> &strs) {
    if (strs.size() == 0) return "";
    std::vector<const char *> v;
    for (int i = 0; i < strs.size(); i++) {
      v.push_back(strs[i].c_str());
    }
    bool flag = true;
    while (flag) {
      for (int i = 1; i < v.size(); i++) {
        if (*(v[0]) != *(v[i]++)) {
          flag = false;
          break;
         }
      }
      v[0]++;
      if (*(v[0] - 1) == '\0') flag = false;
    } 
    int len = v[0] - strs[0].c_str() - 1;
    return strs[0].substr(0, len);
  }
};
}  // namespace twice


int main(int argc, char** argv) {
  std::vector<std::string> v;
  v.push_back("a");
  v.push_back("a");
  v.push_back("a");
  LOG(INFO) << LongestCommnPrefix(v);
  return 0;
}
