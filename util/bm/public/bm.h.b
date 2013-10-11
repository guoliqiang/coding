// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 17:04:40
// File  : bm.h
// Brief :
// http://blog.chinaunix.net/uid-23390992-id-3320412.html
// http://blog.csdn.net/sealyao/article/details/4568167#
// Usually, BM is faster 3~5 times than KMP.
// KMP is restricted by is matche squence (from left to right, so each move step 
// can not greatehr the number of matched characters).

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
      if (bmBc_.count(pattern[i])) {
        bmBc_[pattern_[i]] = pattern_.size() - i - 1;
      } else {
        bmBc_.insert(std::make_pair(pattern_[i], pattern_.size() - i - 1));
      }
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
    // 必须初始化成size
    // 比如　pattern : abc
    //       suffix    0 0 0
    //       gs[0] gs[1] 的值只能取到初始化的size
    for (int i = 0; i < size; i++) {
      bmGs_[i] = size;
    }

    int j = 0;
    for (int i = size - 1; i >= 0; i--) {
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
    // 注意看　bmGs_[size - 1] 与　bmGs_[0]的值
    //         bmGs_[size - 1] 等于最后一个suffix[i] = 0的ｉ
    //         这是为了使移动距离尽可能少，因为其它大的可能会导致错误
    // Note: suffix_[size - 1] = size;
    //       suffix [size - 2] 才可能等于 size - 1
    for (int i = 0; i < size - 1; i++) {
      bmGs_[size - 1 - suffix_[i]] = size - 1 - i;
    }
  }

 private:
  stype pattern_;
  std::map<ctype, int> bmBc_;  // 最后一个字符出现的位置距离字符串末尾的距离
  std::vector<int> bmGs_;      
  std::vector<int> suffix_;    // 从当前字符串算起，有多少个字符是和字符串的后缀匹配的

};
}  // namespace util

#endif  //  __BM_H_
