// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-01 03:18:48
// File  : code.cc
// Brief :

/*
 *  kmp algorithm
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {
void Next(std::string & pattern, std::vector<int> & next) {
  int n = pattern.size();
  next.resize(n, 0);
  next[0] = -1;
  int i = 0;
  int k = next[i];
  while (i < n - 1) {
  // for (int i = 1; i < n; i++) {  // bug
    while (k >= 0 && pattern[i] != pattern[k]) 
      k = next[k];
    i++;
    k++;
    if (pattern[i] == pattern[k]) next[i] = next[k];
    else next[i] = k;
  }
}

bool Match(std::string & pattern, std::string & str) {
  int n = pattern.size();
  int m = str.size();
  if (n == 0) return m == 0;
  std::vector<int> next;
  Next(pattern, next);
  int i = 0;
  int j = 0;
  while (j < m) {  // bug fixed
    if (str[j] == pattern[i]) {
      i++;
      j++;
    } else {
      i = next[i];
    }
    if (i == n) return true;
    if (i == -1) {
      i++;
      j++;
    }
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string pattern = "abc";
  std::string str = "dddddddmmabc";
  LOG(INFO) << Match(pattern, str);
  pattern = "bbf";
  LOG(INFO) << Match(pattern, str);
  return 0;
}
