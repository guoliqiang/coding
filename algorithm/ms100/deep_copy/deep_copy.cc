// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 12:45:22
// File  : deep_copy.cc
// Brief :

#include "./deep_copy.h"

using namespace algorithm;  // NOLINT


int main(int argc, char** argv) {
  Foo<double> a(10);
  Foo<double> a1(0);
  Foo<double> b(a);
  Foo<double> b1(a1);

  Foo<double> c(10);
  Foo<double> c1(0);
  c = a;
  c = a1;

  c1 = a;
  c1 = a1;

  HERE(INFO);

  double * ptr = new double(10);
  Bar<double> bar(ptr);
  Bar<double> bar2 = bar;
  bar2 = bar;
  Bar<double> bar3;
  bar3 = bar2;
  return 0;
}
