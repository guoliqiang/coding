// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 19:41:09
// File  : swap_string.h
// Brief :

#ifndef  __SWAP_STRING_H_
#define  __SWAP_STRING_H_
#include <string>
#include "base/public/logging.h"

namespace algorithm {

inline void swap(char * p1, char * p2) {
  char foo = *p1;
  *p1 = *p2;
  *p2 = foo;
}

inline void SwapStringSub(char * b, char *e) {
  while (b < e) {
    swap(b, e);
    b++;
    e--;
  }
}

void SwapString(std::string & s) {
  char * b = &s[0];
  char * e = &s[s.size() - 1];
  SwapStringSub(b, e);
  b = &s[0];
  e = &s[s.size() - 1];
  while (b != e) {
    while(*b == ' ' && b != e) b++;
    if (b == e) break;
    char * foo = b + 1;
    while (*foo != ' ' && foo != e) foo++;
    swap(b, foo - 1);
    b = foo;
  }
}

}  // namespace algorithm

#endif  //__SWAP_STRING_H_
