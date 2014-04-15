// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-04-09 23:53:12
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Foo() {
   std::string str = "guoliqiang is a man";
   LOG(INFO) << str.length() << " " << str.size() << " " << sizeof(str);
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Foo();
  return 0;
}
