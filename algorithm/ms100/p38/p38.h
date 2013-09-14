// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 15:44:40
// File  : p38.h
// Brief :

#ifndef  __P38_H_
#define  __P38_H_

#include <iostream>
#include <vector>
#include <math.h>
#include "base/public/logging.h"

namespace algorithm {

void OutN(std::vector<unsigned char> & foo) {
  int i = foo.size() - 1;
  while (static_cast<int>(foo[i]) == 0) i--;
  while (i >= 0) {
    std::cout << static_cast<int>(foo[i]);
    i--;
  }
  std::cout << std::endl;
}

void OutN(int n) {
  n = pow(10, n) - 1;
  std::vector<unsigned char> foo;
  foo.assign(n, 0);
  while (n) {
    int i = 0;
    while (static_cast<int>(foo[i]) + 1 == 10) {
      foo[i] = 0;
      i++;
    }
    foo[i] = foo[i] + 1;
    OutN(foo);
    n--;
  }
}

}  // naemspace algorithm


#endif  //__P38_H_
