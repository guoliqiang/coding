// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 13:06:19
// File  : young_matrix.cc
// Brief :
#include "young_matrix.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 25; i++) {
    foo.push_back(i);
  }
  std::vector<std::vector<int> > v;
  BuildYoung(foo, 5, &v);
  OutYoung(v);
  CHECK(YoungFind(v, 15));
  return 0;
}
