// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 10:50:42
// File  : code.cc
// Brief :

/*
 * Write a program to find the longest word made of other words in a list of words.
 * EXAMPLE Input: test, tester, testertest, testing, testingtester Output: testingtester
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool CanForm(const std::string & word, std::set<std::string> & v) {
  if (word.size() == 0) return true;  // bug fixed , add 递归出口
  for (int i = 1; i <= word.size(); i++) {
    if (v.count(word.substr(0, i)) && CanForm(word.substr(i), v)) return true;
  }
  return false;
}

bool Cmp(const std::string & a, const std::string & b) {
  return a.size() > b.size();
}

std::string Find(std::vector<std::string> v) {
  std::sort(v.begin(), v.end(), Cmp);  // 这个排序可以起到剪枝的作用
  for (int i = 0; i < v.size(); i++) {
    std::set<std::string> t(v.begin() + i + 1, v.end());
    if (CanForm(v[i], t)) return v[i];
  }
  return "";
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::string> v;
  v.push_back("test");
  v.push_back("tester");
  v.push_back("testertest");
  v.push_back("testing");
  v.push_back("testingtester");
  LOG(INFO) << Find(v);
  return 0;
}
