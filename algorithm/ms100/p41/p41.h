// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 19:53:53
// File  : p41.h
// Brief :

#ifndef  __P41_H_
#define  __P41_H_

// see http://zhedahht.blog.163.com/blog/static/25411174200952174133707

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

bool Compare(const int & a, const int & b) {
  std::string ab = IntToString(a) + IntToString(b);
  std::string ba = IntToString(b) + IntToString(a);

  CHECK(ab.size() == ba.size());
  for (int i = 0; i< ab.size(); i++) {
    if (ab[i] < ba[i]) return true;
    if (ab[i] > ba[i]) return false;
  }
  return false;
}

void OutMin(std::vector<int> & a) {
  std::sort(a.begin(), a.end(), Compare);
  for (int i = 0; i < a.size(); i++) {
    std::cout << a[i];
  }
  std::cout << std::endl;
}

}  // namespace algorithm


#endif  //__P41_H_
