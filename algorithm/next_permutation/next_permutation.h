// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 15:25:04
// File  : next_permutaion.h
// Brief :
// http://blog.csdn.net/aipb2008/article/details/2227490

#ifndef  __NEXT_PERMUTAION_H_
#define  __NEXT_PERMUTAION_H_

#include "base/public/logging.h"
#include <stdlib.h>

namespace algorithm {
bool NextPermutation(std::string & t) {
  if (t.size() == 1) return false;
  int i = t.size() - 1;
  int j = t.size() - 2;
  while (j >= 0) {
    if (t[j] < t[i]) break;
    j--;
    i--;
  }
  if (j < 0) return false;
  i = t.size() - 1;
  while (t[i] <= t[j]) i--;
  std::swap(t[i], t[j]);
  for (int i = j + 1; i < (t.size() - j - 1) / 2 + 1; i++) {
    std::swap(t[i], t[t.size() - 1 - (i - j - 1)]);
  }
  return true;
}

bool PrePermutation (std::string & t) {
  if (t.size() == 1) return false;
  int i =  t.size() - 1;
  int j =  t.size() - 2;
  while (j >= 0) {
    if (t[j] > t[i]) break;
    j--;
    i--;
  }
  if (j < 0) return false;
  i = t.size() - 1;
  while (t[i] >= t[j]) i--;
  std::swap(t[i], t[j]);
  for (int i = j + 1; i < (t.size() - j - 1) / 2 + 1; i++) {
    std::swap(t[i], t[t.size() - 1 - (i - j - 1)]);
  }
  i = 0;
  while (t[i] == '0') i++;
  t = t.substr(i);
  return true;
}

}  // namespace algorithm

#endif  // __NEXT_PERMUTAION_H_
