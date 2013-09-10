// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 11:49:33
// File  : last_numner.h
// Brief :

#ifndef  __LAST_NUMNER_H_
#define  __LAST_NUMNER_H_

#include <list>
#include "base/public/logging.h"

namespace algorithm {

int LastNum(int n, int m) {
  if (n == 1) return 0;
  return (LastNum(n - 1, m) + m) % n;
}

int LastNum2(int n, int m) {
  std::list<int> l;
  for (int i = 0; i < n; i++) {
    l.push_back(i);
  }
  std::list<int>::iterator it = l.begin();
  while (l.size() > 1) {
    for (int i = 0; i < m - 1; i++)  {
      it++;
      if (it == l.end()) it = l.begin();
    }
    it = l.erase(it);
    if (it == l.end()) it = l.begin();
  }
  return l.front();
}

}  // namespace algorithm


#endif  //__LAST_NUMNER_H_
