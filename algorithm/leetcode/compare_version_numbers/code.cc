// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-15 22:24:50
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"


int CompareVersion(const std::string & version1, const std::string & version2) {
  int idx1 = 0;
  int idx2 = 0;

  while (idx1 < version1.size() || idx2 < version2.size()) {
    int value1 = 0;
    int value2 = 0;
    while (idx1 < version1.size() && version1[idx1] != '.') {
      value1 = value1 * 10 + version1[idx1] - '0';
      idx1++;
    }
    if (idx1 < version1.size() && version1[idx1] == '.') {
      idx1++;
    }

    while (idx2 < version2.size() && version2[idx2] != '.') {
      value2 = value2 * 10 + version2[idx2] - '0';
      idx2++;
    }
    if (idx2 < version2.size() && version2[idx2] == '.') {
      idx2++;
    }

    if (value1 > value2) return 1;
    else if (value1 < value2) return -1;
  }
  return 0;
}

int main(int argc, char** argv) {
  LOG(INFO) << CompareVersion("0.1", "0.0.1");
  return 0;
}
