// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 21:16:06
// File  : code.cc
// Brief :

/*
 * 原文：
 * Write a method to sort an array of strings so that all the anagrams are next to each other.
 * 译文：
 * 写一个函数对字符串数组排序，使得所有的变位词都相邻。
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool Cmpare(std::string v1, std::string v2) {
  if (v1.size() != v2.size()) return false;
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());
  return v1 == v2;
}

// 其实是分组
void Sort(std::vector<std::string> & v) {
  int n = v.size();
  int i = 0;
  bool flag = true;
  while (flag) {
    flag = false;
    for (int j = i + 1; j < n; j++) {
      if (Cmpare(v[i], v[j])) {
        i++;
        std::swap(v[i], v[j]);
        flag = true;
      }
    }
    i++;  // bug fixed, add this line
  }
}
}  // namespace algorithm

namespace NB {

// 不用担心出现"XMX" "YXX" "MXX"
// 因为"XMX" "MXX"与"YXX"的比较一定是相同的
// 所以其要么全在"YXX"的左侧，要么全在"YXX"的右侧
//
bool Cmp(std::string s1, std::string s2) {
  std::sort(s1.begin(), s1.end());
  std::sort(s2.begin(), s2.end());
  return s1 < s2;
}

// 按每个串对应的排序后的结果排序
void Sort(std::vector<std::string> & v) {
  std::sort(v.begin(), v.end(), Cmp);
}
}

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> foo;
  foo.push_back("axyz");
  foo.push_back("yzax");
  foo.push_back("bac");
  foo.push_back("zyxa");
  foo.push_back("a");
  foo.push_back("abc");
  Sort(foo);
  // NB::Sort(foo);
  LOG(INFO) << JoinVector(foo);
  return 0;
}
