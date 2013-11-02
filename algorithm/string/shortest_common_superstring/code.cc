// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-01 05:10:09
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

void Next(std::string & pattern, std::vector<int> & next) {
  int n = pattern.size();
  next.resize(n, 0);
  next[0] = -1;
  if (n == 1) return;
  next[1] = 0;
  int k = next[1];
  for (int i = 1; i < n; i++) {
    while (k >= 0 && pattern[i] != pattern[k]) k = next[k];
    i++;
    k++;
    if (pattern[i] == pattern[k]) next[i] = next[k];
    else next[i] = k;
  }
}

bool KMP(int & i, int & j, std::string & pattern, std::string & str) {
  int n = pattern.size();
  int m = str.size();

  std::vector<int> next;
  Next(pattern, next);
  i = 0;
  j = 0;
  while (j < m) {
    if (pattern[i] == str[j]) {
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

int ShortestCommon(std::string & str1, std::string & str2) {
  int len1 = str1.size();
  int len2 = str2.size();
  int i = 0;
  int j = 0;

  int t1 = 0;
  int t2 = 0;
  // find str2 in str1
  if (KMP(i, j, str2, str1)) {
    t1 = len2;
    // LOG(INFO) << "find";
  } else {
    if (i > 0) t1 = i;
  }

  // find str1 in str2;
  if (KMP(i, j, str1, str2)) {
    t2 = len1;
    // LOG(INFO) << "find";
  } else {
    if (i > 0) t2 = i;
  }
  LOG(INFO) << "t1:" << t1 << " t2:" << t2;
  return len1 + len2 - std::max(t1, t2);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str1;
  std::string str2;
  str1 = "mmab";
  str2 = "abc";
  LOG(INFO) << ShortestCommon(str1, str2);
  return 0;
}
