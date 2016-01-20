// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 23:32:43
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int read4(char *buf) { return 4; }
int read(char * buf, int n) {
  int len = 0;
  while (len + 4 <= n) {
    int size = read4(buf);
    if (size != 4) return len + size;
    len += 4;
    buf += 4;
  }
  int left = n - len;
  char * t = new char[4];
  int size = read4(t);
  for (int i = 0; i < std::min(left, size); i++) {
    *buf = t[i];
    buf++;
  }
  delete [] t;
  return len + std::min(left, size);
}
}  // namespace algorithm

using namespace algorithm;

class Base {
 public:
  Base() { this->Fun(); }
  virtual void Fun() {
    LOG(INFO) << "Base";
  }
};

class Foo : public Base {
 public:
  virtual void Fun() {
    LOG(INFO) << "Fun";
  }
};

int main(int argc, char** argv) {
  Base * ptr = new Foo();
  ptr->Fun();
  return 0;
}
