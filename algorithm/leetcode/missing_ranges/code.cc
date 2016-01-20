// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 16:19:28
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string IntToString(int v) {
  char buff[100] = { 0 };
  sprintf(buff, "%d", v);
  return std::string(buff);
}

std::vector<std::string> FindMissingRanges(std::vector<int> & num, int lower, int upper) {
  int n = num.size();
  if (n == 0) return std::vector<std::string> ();

  std::vector<std::string> ans;
  int pre = lower;
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      if (num[i] - pre == 1) {
        ans.push_back(IntToString(num[i] - 1));
      } else if (num[i] - pre > 1) {
        ans.push_back(IntToString(pre) + "->" + IntToString(num[i] - 1));
      }
    } else {
      if (num[i] - pre == 2) {
        ans.push_back(IntToString(num[i] - 1));
      } else if (num[i] - pre > 2) {
        ans.push_back(IntToString(pre + 1) + "->" + IntToString(num[i] - 1));
      }
    }
    pre = num[i];
  }
  if (upper - pre == 1) {
    ans.push_back(IntToString(pre + 1));
  } else if (upper - pre > 1) {
    ans.push_back(IntToString(pre + 1) + "->" + IntToString(upper));
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(3);
  num.push_back(50);
  num.push_back(98);
  LOG(INFO) << JoinVector(FindMissingRanges(num, 0, 99));
  return 0;
}
