// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-18 16:40:52
// File  : code.cc
// Brief :

// 备忘录模式,用于支持回撤操作

#include "base/public/common_ojhead.h"

namespace algorithm {
struct State {};

class Memento {
 public:
  friend class Originator;
  virtual ~Memento() {
    delete state;
  }

  // 只有Orignator可以访问
 private:
  Memento() : state(new State()) {}

  void SetState(State *s) {
    *state = *s;
  }

  State * GetState() {
    return state;
  }

 private:
  State * state;
};

class Originator {
 public:
  Originator() : state(new State()) {}

  virtual ~Originator() {
    delete state;
  }

  Memento * CreatMemento() {
    Memento * p = new Memento();
    p->SetState(state);
    return p;
  }

  void SetMemento(Memento * ptr) {
    *state = *(ptr->GetState());
  }

 private:
  State * state;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
