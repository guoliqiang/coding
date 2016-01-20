// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-09 20:14:14
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
long long int StringToInt(const std::string & str) {
  return atoll(str.c_str());
}

void Get(const std::string & num, long long int target, long long int last, long long int cur, int idx,
         char op, std::vector<std::string> & ans, std::string path) {
  if (idx == num.size()) {
    if (cur == target) {
      ans.push_back(path);
    }
    return;
  }

  for (int i = idx; i < num.size(); i++) {
    if (i > idx && num[idx] == '0') continue;
    std::string s = num.substr(idx, i - idx + 1);
    long long int t = StringToInt(s);
    Get(num, target, t, cur + t, i + 1, '+', ans, path + "+" + s);
    Get(num, target, t, cur - t, i + 1, '-', ans, path + "-" + s);
    if (op == ' ') {
      Get(num, target, last * t, last * t, i + 1, '*', ans, path + "*" + s);
    } else if (op == '+') {
      Get(num, target, last * t, cur - last + last * t, i + 1, op, ans, path + "*" + s);
    } else if (op == '-') {
      Get(num, target, last * t, cur + last - last * t, i + 1, op, ans, path + "*" + s);
    } else {
      Get(num, target, last * t, cur * t, i + 1, '*', ans, path + "*" + s);
    }
  }
}

std::vector<std::string> AddOperators(std::string num, int target) {
  std::vector<std::string> ans;
  for (int i = 0; i < num.size(); i++) {
    if (i > 0 && num[0] == '0') continue;

    std::string s = num.substr(0, i + 1);
    long long int t = StringToInt(s);
    Get(num, target, t, t, i + 1, ' ', ans, s);
  }
  return ans;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "123";
  LOG(INFO) << JoinVector(AddOperators(str, 6));
  str = "232";
  LOG(INFO) << JoinVector(AddOperators(str, 8));
  str = "105";
  LOG(INFO) << JoinVector(AddOperators(str, 5));
  str = "00";
  LOG(INFO) << JoinVector(AddOperators(str, 0));
  str = "2147483648";
  LOG(INFO) << JoinVector(AddOperators(str, -2147483648));
  return 0;
}
