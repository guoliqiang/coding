// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-26 22:43:37
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct TrieNode {
  int cnt;
  TrieNode * fail;
  std::map<char, TrieNode *> child;
};
}  // namespace algorithm

int main(int argc, char** argv) {
  return 0;
}
