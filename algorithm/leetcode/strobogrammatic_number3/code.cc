// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 16:06:45
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::vector<std::string> Get(int m, int n) {
  std::vector<std::string> ans;
  if (m == 0) {
    ans.push_back("");
  } else if (m == 1) {
    ans.push_back("0");
    ans.push_back("1");
    ans.push_back("8");
  } else {
    std::vector<std::string> t = Get(m - 2, n);
    for (int i = 0; i < t.size(); i++) {
      ans.push_back("8" + t[i] + "8");
      ans.push_back("1" + t[i] + "1");
      ans.push_back("6" + t[i] + "9");
      ans.push_back("9" + t[i] + "6");
      if (m != n) ans.push_back("0" + t[i] + "0");
    }
  }
  return ans;
}

int StrobogrammaticInRange(std::string low, std::string high) {
  std::vector<std::string> ans;
  for (int i = low.size(); i <= high.size(); i++) {
    std::vector<std::string> tmp = Get(i, i);
    ans.insert(ans.end(), tmp.begin(), tmp.end());
  }
  int cnt = ans.size();
  for (int i = 0; i < ans.size(); i++) {
    if (ans[i].size() > low.size()) break;
    if (ans[i] < low) cnt--;
  }

  for (int i = ans.size() - 1; i >= 0; i--) {
    if (ans[i].size() < high.size()) break;
    if (ans[i] > high) cnt--;
  }
  return cnt;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << JoinVector(Get(3, 3));
  LOG(INFO) << StrobogrammaticInRange("609", "888");
  return 0;
}
