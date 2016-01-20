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

namespace cpp {
int StringToInt(const std::string & str) {
    return atoi(str.c_str());
}

std::vector<int> Split(const std::string & str) {
    std::vector<int> rs;
    std::string cur;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '.') {
            if (cur.size()) rs.push_back(StringToInt(cur));
            cur.clear();
        } else {
            cur.push_back(str[i]);
        }
    }
    if (cur.size()) rs.push_back(StringToInt(cur));
    return rs;
}

class Solution {
public:
    int compareVersion(std::string version1, std::string version2) {
        std::vector<int> v1 = Split(version1);
        std::vector<int> v2 = Split(version2);
        int i = 0;
        int j = 0;
        while (i < v1.size() || j < v2.size()) {
            int t1 = i < v1.size() ? v1[i] : 0;
            int t2 = j < v2.size() ? v2[j] : 0;
            if (t1 < t2) return -1;
            else if (t1 > t2) return 1;
            i++;
            j++;
        }
        return 0;
    }
};
}  // namespace cpp

int main(int argc, char** argv) {
  LOG(INFO) << CompareVersion("0.1", "0.0.1");
  return 0;
}
