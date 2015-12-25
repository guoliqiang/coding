// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-24 19:07:54
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string Add(const std::string & a, const std::string & b) {
  std::string ans;
  int i = a.size() - 1;
  int j = b.size() - 1;
  int c = 0;
  while (i >= 0 || j >= 0 || c > 0) {
    if (i >= 0) c += (a[i--] - '0');
    if (j >= 0) c += (b[j--] - '0');
    ans.push_back(c % 10 + '0');
    c /= 10;
  }
  std::reverse(ans.begin(), ans.end());
  return ans;
}

bool IsValid(const std::string & num, int i, int j, int k) {
  while (k < num.size()) {
    std::string a = num.substr(i, j - i);
    std::string b = num.substr(j, k - j);
    if (a.size() > 1 && a[0] == '0') return false;
    if (b.size() > 1 && b[0] == '0') return false;
    std::string sum = Add(a, b);
    std::string left = num.substr(k);
    if (sum.size() > left.size()) return false;
    if (left.substr(0, sum.size()) != sum) {
      return false;
    }
    i = j;
    j = k;
    k = j + sum.size();
  }
  return true;
}

bool IsAdditiveNumber(const std::string & num) {
  if (num.size() < 3) return false;
  int n = num.size();
  for (int i = 1; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (IsValid(num, 0, i, j)) return true;
    }
  }
  return false;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << IsAdditiveNumber("0235813");
  LOG(INFO) << IsAdditiveNumber("199100199");
  return 0;
}
