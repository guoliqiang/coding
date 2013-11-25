// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-06 22:00:52
// File  : code.cc
// Brief :

/*
 * Write a method to print the last K lines of an input file using C++.
 *
 * */

#include "base/public/common_head.h"

/*
 * 开始是求链表倒数第k个node的思路
 * 没有这个方法直接
 * */

namespace NB {

void OutLastK(const std::string & path, int k) {
  std::vector<std::string> lines(k, std::string());
  std::ifstream in(path.c_str());
  if (in == NULL) return;
  std::string line;
  int line_count = 0;
  while (std::getline(in, line)) {
    lines[line_count % k] = line;
    line_count++;
  }
  LOG(INFO) << JoinVector(lines);
}

}  // namespace NB

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string path = "test.txt";
  NB::OutLastK(path, 2);
  return 0;
}
