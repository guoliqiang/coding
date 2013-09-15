// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 17:04:40
// File  : bm.h
// Brief :
// http://blog.chinaunix.net/uid-23390992-id-3320412.html
// http://blog.csdn.net/sealyao/article/details/4568167#

#ifndef  __BM_H_
#define  __BM_H_

#include <map>
#include <vector>
#include "base/public/logging.h"

namespace util {
template <typename stype, typename ctype>
class BM {
 public:
  BM(stype & str) : pattern_(str) {
    bmGs_.resize(pattern_.size());
    suffix_.resize(pattern_.size());
    BuildBMBC();
    BuildSuffix();
    BuildBMGS();
  }

  int Match(stype str) {
    int j = 0;
    int size = pattern_.size();
    while (j < str.size() - size) {
      int i = size - 1;
      while (i >= 0 && pattern_[i] == str[i + j]) {
        i--;
      }
      if (i < 0) return j;
      else {
        int BcMove = size - (size - 1 - i);
        if (bmBc_.count(str[j + i])) {
          BcMove = bmBc_[str[j + i]] - (size - 1 - i);
        }
        j += std::max(bmGs_[i], BcMove);
      }
    }
    return -1;
  }

 private:
  void BuildBMBC() {
    for (int i = 0; i < pattern_.size(); i++) {
      bmBc_.insert(std::make_pair(pattern_[i], pattern_.size() - i - 1));
    }
  }

  void BuildSuffix() {
    int size = pattern_.size();
    suffix_[size - 1] = size;
    for (int i = size - 2; i >= 0; i--) {
      int m = 0;
      while (pattern_[i - m] == pattern_[size - 1 - m]) {
        m++;
      }
      suffix_[i] = m;
    }
  }

  // must be called after BuildSuffix
  void BuildBMGS() {
    int size = pattern_.size();
    for (int i = 0; i < size; i++) {
      bmGs_[i] = size;
    }

    int j = 0;
    for (int i = 0; i < size; i++) {
      if (suffix_[i] == i + 1) {
        for (; j < size - 1 - i; j++) {
          if (bmGs_[j] == size) bmGs_[j] = size - i - 1;
        }
      }
    }

    //          A B
    //  ---[]------[]
    //          |
    //          ---[]------[]
    // this is not resonable, becase at point B , can not match
    for (int i = 0; i < size -1; i++) {
      bmGs_[size - 1 - suffix_[i]] = size - 1 - i;
    }
  }

 private:
  stype pattern_;
  std::map<ctype, int> bmBc_;
  std::vector<int> bmGs_;
  std::vector<int> suffix_;

};
}  // namespace util

#endif  //  __BM_H_
