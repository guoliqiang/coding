// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 21:07:37
// File  : leftrotate.h
// Brief :

#ifndef  __LEFTROTATE_H_
#define  __LEFTROTATE_H_

#include <string>
#include "../swap_string/swap_string.h"
#include "base/public/logging.h"

namespace algorithm {


void leftrotate(std::string & str, int n) {
  n = n % str.size();
  if (n == 0) return;
  char * b = &str[0];
  char * e = &str[n - 1];
  SwapStringSub(b, e);
  SwapStringSub(&str[n], &str[str.size() - 1]);
  SwapStringSub(&str[0], &str[str.size() - 1]);
}

int y(int i, int n, int size) {
  if (i < n) return size - n + i;
  else return i - n;
}

void leftrotate_v2(std::string & str, int n) {
  n = n % str.size();
  if (n == 0) return;
  if (str.size() % n == 0 ||
      str.size() % (str.size() - n) == 0) {
    int k = n < str.size() - n ? n : str.size() - n;
    for (int i = 0; i < k; i++) {
      int foo = i;
      char bar = str[foo];
      do {
        char tmp = str[y(foo, n, str.size())];
        str[y(foo, n, str.size())] = bar;
        // LOG(INFO) << y(foo, n, str.size()) << ":" << str[y(foo, n, str.size())];
        bar = tmp;
        foo = y(foo, n , str.size());
      } while (foo != i);
    }
  } else {
    int foo = 0;
    char bar = str[0];
    do {
      char tmp = str[y(foo, n, str.size())];
      str[y(foo, n, str.size())] = bar;
      bar = tmp;
      foo = y(foo, n , str.size());
    } while (foo != 0);
  }
}

}  // namespace algorithm

#endif  // __LEFTROTATE_H_
