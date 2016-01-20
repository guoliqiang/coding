// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-12 17:34:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string ToBase62(int v) {
  std::string ans;
  std::map<int, char> m;
  for (int i = 0; i < 62; i++) {
    if (i < 10) m[i] = '0' + i;
    else if (i < 36) m[i] = 'a' + (i - 10);
    else {
      m[i] = 'A' + (i - 36);
    }
  }

  while (v > 0) {
    ans.append(1, m[v % 62]);
    v /= 62;
  }
  while(ans.size() < 6) ans.push_back('0');
  std::reverse(ans.begin(), ans.end());
  return ans;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << ToBase62(898989);
  return 0;
}
