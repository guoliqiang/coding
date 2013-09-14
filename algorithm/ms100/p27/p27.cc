// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 16:10:51
// File  : p27.cc
// Brief :

#include "../bst_to_dq/bst.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  Node<int> * root = BuildBST(foo);
  LOG(INFO) << Deepth(root);
  return 0;
}
