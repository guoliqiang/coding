// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 22:11:00
// File  : kmp.h
// Brief :
// Building next is O(m)
// Mathch is O(m + n)

#ifndef  __KMP_H_
#define  __KMP_H_

#include <vector>
#include "base/public/logging.h"

namespace util {

template <typename stype, typename ctype>
class KMP {
 public:
  KMP(stype pattern) : pattern_(pattern) {
    next_.assign(pattern_.size(), 0);
    BuildNext();
  }

  int Match(stype str) {
    int size = pattern_.size();
    int j = 0;
    int i = 0;
    while (i < str.size() - pattern_.size()) {
      while (j < size && str[i] == pattern_[j]) {
        j++;
        i++;
      }
      if (j == size) {
        return i - size;
      } else {
        j = next_[j];
        if (j < 0) {
          j++;
          i++;
        }
      }
    }  // for
    return -1;
  }
 private:
  void BuildNext() {
    int size = pattern_.size();
    next_[0] = -1;
    if (size > 1) {
      next_[1] = 0;
      for (int i = 2; i < size; i++) {
        int j = next_[i - 1];
        while (j > 0 && pattern_[i - 1] != pattern_[j]) {
          j = next_[j];
        }
        j++;
        if (pattern_[i] == pattern_[j]) {
          next_[i] = next_[j];
        } else {
          next_[i] = j;
        }
      }
    }
  }
 
 private:
  stype pattern_;
  std::vector<int> next_;
};

}  // namespace util

#endif  //  __KMP_H_
