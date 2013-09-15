// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 09:51:18
// File  : sub_tree.cc
// Brief :
#include "sub_tree.h"
#include <vector>
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  std::vector<int> foo2;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  foo2.push_back(1);
  foo2.push_back(2);
  foo2.push_back(3);
  foo2.push_back(4);
  Node<int> * root = BuildBST(foo);
  Node<int> * root2 = BuildBST(foo2);
  MiddleRootIterate(root);
  HERE(INFO);
  LOG(INFO) << "result:" << SubTree(root, root2);
  HERE(INFO);
  for (int i = 0; i < 10; i++) {
    Node<int> * bar = RandomNode(root);
    MiddleRootIterate(bar);
    LOG(INFO) << "result:" << SubTree(root, bar);
  }
  return 0;
}
