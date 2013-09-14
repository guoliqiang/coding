// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-14 19:54:21
// File  : unionfind_set_unittest.cc
// Brief :
#include "../../public/unionfind_set.h"


using namespace util;  // NOLINT

int main(int argc, char** argv) {
  UFSet<int> foo;
  CHECK(foo.Insert(1));
  CHECK(foo.Insert(2));
  CHECK(foo.Insert(4));
  CHECK(foo.Insert(5));
  CHECK(foo.Insert(6));
  CHECK(foo.Insert(6) == 0);
  int bar = 0;
  CHECK(foo.Find(5, &bar));
  CHECK(bar == 5);

  CHECK(foo.FindPC(6, &bar));
  CHECK(bar == 6);
  CHECK(foo.FindPC(7, &bar) == false);
  CHECK(foo.Find(7, &bar) == false);
  
  CHECK(foo.Union(1, 2));
  CHECK(foo.Find(2, &bar));
  LOG(INFO) << bar;
  CHECK(foo.Union(bar, 5));
  foo.Find(5, &bar);
  int foobar;
  foo.Find(1, &foobar);

  LOG(INFO) << bar << " " << foobar;
  CHECK(bar == foobar);

  return 0;
}
