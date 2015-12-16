// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 10:04:11
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::string ConvertToTitle(int n) {
  std::string title = "";
  while (n > 0) {
    n--;
    title.insert(title.begin(), 'A' + (n % 26));
    n /= 26;
  }
  return title;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << ConvertToTitle(26);
  return 0;
}
