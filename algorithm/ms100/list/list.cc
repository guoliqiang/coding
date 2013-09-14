// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 17:13:26
// File  : list.cc
// Brief :
#include "list.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  Node<int> * root = BuildList(foo);
  ReverseOut(root);
  HERE(INFO);
  ReverseOut2(root);
  Delete(root);
  return 0;
}
