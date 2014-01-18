// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 16:26:38
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// 为一个子系统提供一个简单的使用方式，同时并不隐藏子系统
// 的实现细节，可供高级用户使用

namespace algorithm {
class Scanner {
 public:
  void Do() {}
};

class Parser {
 public:
  void Do() {}
};

class CodeGenerator {
 public:
  void Do() {}
};

// facade
class Compiler {
 public:
  void Do() {
    s->Do();
    p->Do();
    c->Do();
  }

  void CallScanner() {
    s->Do();
  }

  void CallParser() {
    p->Do();
  }

  void CallCodeGenerator() {
    c->Do();
  }
 private:
  Scanner * s;
  Parser * p;
  CodeGenerator * c;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
