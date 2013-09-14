// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 13:39:15
// File  : complex_list.cc
// Brief :
#include "complex_list.h"
using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i< 10; i++) {
    foo.push_back(random() % 100);
  }
  CNode<int> * head = BuildCList(foo);
  OutCList(head);
  CNode<int> * cp_head = CopyCList(head);
  OutCList(cp_head);
  Delete(head);
  Delete(cp_head);
  return 0;
}
