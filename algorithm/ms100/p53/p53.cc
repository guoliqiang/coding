// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 11:07:07
// File  : p53.cc
// Brief :
#include "base/public/logging.h"

struct Point3D{
  int x;
  int y;
  int z;
};

class A {
 public:
  A():n2(1), n1(n2 + 1) {}
  int n1;
  int n2;
};

int main(int argc, char** argv) {
  Point3D * point = NULL;
  long offset = reinterpret_cast<long>(&(point->z));
  LOG(INFO) << offset;
  return 0;
}

