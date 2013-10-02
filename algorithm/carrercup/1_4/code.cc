// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 17:38:27
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

bool IsAnagram(std::string s1, std::string s2) {
  if (s1.size() != s2.size()) return false;
  std::sort(s1.begin(), s1.end());
  std::sort(s2.begin(), s2.end());
  return s1 == s2;
}

// ASCII 0 ~ 127
// 所以可以使用 (int)(s[i])
// 否则(int)((unsigned char)s[i])
bool IsAnagram(const std::string & s1, const std::string &s2) {
  int * p = new int [256];
  memset(p, 0, sizeof(int) * 0xff);
  for (int i = 0; i < s1.size(); i++) p[(int)s1[i]]++;
  for (int i = 0; i < s2.size(); i++) p[(int)s2[i]]--;
  for (int i = 0; i < 256; i++) {
    if (p[i] != 0) return false;
  }
  return true;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  return 0;
}

