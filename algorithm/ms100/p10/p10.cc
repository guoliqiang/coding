// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-09 20:23:15
// File  : p10.cc
// Brief :

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
template <typename type>
void Find(std::vector<type> & a, type f) {
  type * b = &a[0];
  type * e = &a[a.size() - 1];
  while (b != e) {
    if (*b + *e > f) {
      e--;
    } else if (*b + *e < f) {
      b++;
    } else {
      LOG(INFO) << *b << " " << *e;
      return;
    }
  }
}
}  // namespace algorithm


using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(4);
  foo.push_back(7);
  foo.push_back(11);
  foo.push_back(15);
  Find(foo, 15);
  return 0;
}
