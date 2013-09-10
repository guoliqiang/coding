// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 15:03:44
// File  : bst2dq.cc
// Brief :
#include "./bst.h"


using namespace algorithm;  // NOLINT
using namespace std;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(random() % 100);
  }
  sort(foo.begin(), foo.end());
  Node<int> *  root = BuildBST(foo, 0, foo.size());
  HERE(INFO);
  MiddleRootIterate(root);
  std::pair<Node<int> *, Node<int> *> bar = BST2DQ(root);
  HERE(INFO);
  IterateDQ(bar.first);
  HERE(INFO);
  ReverseIterateDQ(bar.second);
  DeleteDQ(bar.first);
  return 0;
}
