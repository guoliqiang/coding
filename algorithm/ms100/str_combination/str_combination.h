// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 20:03:28
// File  : str_combination.h
// Brief :

#ifndef  __STR_COMBINATION_H_
#define  __STR_COMBINATION_H_

#include <iostream>
#include <vector>
#include "base/public/logging.h"

namespace algorithm {
void CombinationSub(std::string & str, int n, std::vector<int> & flag) {
  if (n == str.size()) {
    for (int i = 0; i< str.size(); i++) {
      if (flag[i]) std::cout << str[i];
    }
    std::cout << std::endl;
  } else {
    for (int i = 0; i < 2; i++) {
      flag[n] = i;
      CombinationSub(str, n + 1, flag);
    }
  }
}

void Combination(std::string & str) {
  std::vector<int> flag(str.size(), 0);
  CombinationSub(str, 0, flag);
}

}  // namespace algorithm


#endif  //  __STR_COMBINATION_H_
