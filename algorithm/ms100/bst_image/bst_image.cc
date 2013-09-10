// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 20:38:11
// File  : bst_iamge.cc
// Brief :
#include "./bst_image.h"
#include "stdlib.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(random() % 100);
  }
  sort(foo.begin(), foo.end());
  Node<int> * root = BuildBST(foo);
  MiddleRootIterate(root);
  BSTImage(root);
  HERE(INFO);
  MiddleRootIterate(root);
  DeleteBST(root);
  return 0;
}
