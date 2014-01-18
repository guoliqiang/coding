// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 01:19:08
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class Document {
 public:
  virtual void Do() = 0;
  virtual ~Document() {}
};

class Application {
 public:
  virtual Document * CreatDoc() = 0;
  void Do() {  // 算法骨架共用部分，因此不需要是virtual,这也是模板方法的精髓 
    doc = CreatDoc();
    doc->Do();
  }
  virtual ~Application() {}
 protected:
  Document * doc;
};

class SimpleDocument : public Document {
 public:
  void Do() {
    LOG(INFO) << "Simple";
  }
};

class ComplexDocument : public Document {
 public:
  void Do() {
    LOG(INFO) << "complex";
  }
};

class SimpleApplication : public Application {
 public:
  Document * CreateDoc() {
    return new SimpleDocument();
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
