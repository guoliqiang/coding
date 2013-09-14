// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 16:17:10
// File  : p28.h
// Brief :

#ifndef  __P28_H_
#define  __P28_H_


#include "base/public/logging.h"
#include "../swap_string/swap_string.h"
#include <iostream>

namespace algorithm {

void  SubTree(std::string & str, int n, int * p) {
  if (n == str.size()) {
    for (int i = 0; i < str.size(); i++) {
      if (p[i]) std::cout << str[i];
    }
    std::cout << std::endl;
    return;
  }
  for (int i = 0; i < 2; i++) {
    p[n] = i;
    SubTree(str, n + 1, p);
  }
}

void SubTree(std::string & str) {
  int * p = new int[str.size()];
  SubTree(str, 0, p);
  delete [] p;
}

void ArrangeTree(std::string & str, int n) {
  if (n == str.size()) {
    LOG(INFO) << str;
    return;
  }
  for (int i = n; i < str.size(); i++) {
    swap(&str[i], &str[n]);
    ArrangeTree(str, n + 1);
    swap(&str[i], &str[n]);
  }
}

void ArrangeTree(std::string & str) {
  std::string foo = str;
  ArrangeTree(foo, 0);
}

}  // namespace algorithm

#endif  // __P28_H_
