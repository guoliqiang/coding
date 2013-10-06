// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-17 14:44:49
// File  : p1.cc
// Brief :
#include "base/public/logging.h"
#include <vector>
#include <algorithm>

struct Node {
  int value;
  int index;
};
static bool Cmp(const Node & a, const Node & b) {
  return a.value < b.value;
}


int main(int argc, char** argv) {
  std::vector<Node> foo;
  for (int i = 0; i< 10; i++) {
    Node n;
    n.index = i;
    n.value = i + 10;
    foo.push_back(n);
  }
  sort(foo.begin(), foo.end(), Cmp);

  for (int i = 0; i< foo.size(); i++) {
    LOG(INFO) << foo[i].index << " " << foo[i].value;
  }
  return 0;
}
