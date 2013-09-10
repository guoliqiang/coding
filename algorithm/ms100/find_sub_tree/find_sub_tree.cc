// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 17:08:20
// File  : find_sub_tree.cc
// Brief :
#include "./find_sub_tree.h"
using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  int bar = 0;
  for (int i = 0; i< 10; i++) {
    foo.push_back(i);
    bar += i;
  }
  Node<int> * root = BuildBST(foo, 0, foo.size());
  MiddleRootIterate(root);
  for (int i = 0; i< bar; i++) {
    FindPath(root, i);
  }
  DeleteBST(root);
  return 0;
}
