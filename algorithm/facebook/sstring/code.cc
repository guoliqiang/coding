// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 17:28:33
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int Cal(std::string str) {
  int n = str.size();
  std::vector<int> num(n + 1);
  num[0] = 1;
  for (int i = 1; i <= n; i++) {
    num[i] = (num[i - 1] * 25) % 1009;
  }
  int rs = 1;
  for (int i = 0; i < n; i++) {
    for (int j = 'a'; j < str[i]; j++) {
      if (i > 0 && j == str[i - 1]) continue;
      rs = (rs + num[n - i - 1]) % 1009;
    }
    if (i > 0 && str[i] == str[i - 1]) {
      rs--;
      break;
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << Cal("ccc");
  LOG(INFO) << Cal("ddd");
  LOG(INFO) << Cal("abbc");
  LOG(INFO) << Cal("zzzzz");
  return 0;
}
