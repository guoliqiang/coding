// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-11 21:39:50
// File  : sunday.h
// Brief :
// http://blog.chinaunix.net/uid-25513153-id-225240.html
//
// BM 的改进算法
// 实际效果怎么样，感觉不好说

#ifndef  __SUNDAY_H_
#define  __SUNDAY_H_

#include <map>
#include "base/public/logging.h"

namespace util {
template <class T1, class T2>
class Sunday {
 public:
  Sunday() {}

  Sunday(T1 p) : pattern_(p) {
    for (int i = 0; i < p.size(); i++) {
      shift_[p[i]] = p.size() - i;
    }
  }

  bool Match(const T1 & str) {
    int i = 0;
    int j = 0;
    while (i < str.size() - pattern_.size()) {
      if (str[i] == pattern_[j]) {
        i++;
        j++;
      } else {
        char ch = str[i + pattern_.size() - j];
        i += shift_.count(ch) ? shift_[ch] : pattern_.size() - j;
        if (shift_.count(ch)) i -= j;
        j = 0;
      }
      if (j == pattern_.size()) return true;
    }
    return false;
  }

 private:
  T1 pattern_;
  std::map<T2, int> shift_;
};
}   // namespace util

#endif  //  __SUNDAY_H_
