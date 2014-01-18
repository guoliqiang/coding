// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 01:42:09
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// 保持原有类的结构，新加的操作都在visitor中，一般这些搜是些和之前类关系不是非常紧密的操作
// 如输出debug信息
namespace algorithm {
class Element;

class Visitor {
 public:
  // 如果不希望用RTTI，使用如下两个函数
  // virtual void VisitConcreatElementA(ConcreatElementA * ptr) {};
  // virtual void VisitConcreatElementB(ConcreatElementB * ptr) {};
  virtual void VisitElement(Element * ptr) = 0;
  virtual ~Visitor() {}
};

class Element {
 public:
  virtual void CallVisitor(Visitor * ptr) = 0;
  virtual ~Element() {}
};

class ConcreatElementA : public Element {
 public:
  void CallVisitor(Visitor * ptr) {
    ptr->VisitElement(this);
  }
  void Out() {
    LOG(INFO) << "A";
  }
};

class ConcreatElementB : public Element {
 public:
  void CallVisitor(Visitor * ptr) {
    ptr->VisitElement(this);
  }
  void Print() {
    LOG(INFO) << "B";
  }
};

class VisitorElementA {
 public:
  void VisitElement(Element * ptr) {
    ConcreatElementA * p = dynamic_cast<ConcreatElementA*>(p);
    p->Out();
  }
};

class VisitorElementB {
 public:
  void VisitElement(Element * ptr) {
    ConcreatElementB * p = dynamic_cast<ConcreatElementB*>(p);
    p->Print();
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
