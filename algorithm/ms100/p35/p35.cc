// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 09:49:45
// File  : p35.cc
// Brief :
#include "p35.h"
#include "base/public/logging.h"

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  std::vector<int> bar;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  for (int j = 10; j < 20; j++) {
    bar.push_back(j);
  }
  Node<int> * h1 = BuildList(foo);
  Node<int> * h2 = BuildList(bar);
  Node<int> * t1 = Tail(h1);
  Node<int> * t2 = Tail(h2);
  CHECK(IsJoin(h1, h2) == false);
  Node<int> * middle = Move(h1, 5);
  t2->next = middle;
  CHECK(IsJoin(h1, h2) == true);
  CHECK(JoinPoint(h1, h2)->value == middle->value);
  t2->next = NULL;
  CHECK(HasCircle(h1) == false);
  CHECK(HasCircle(h2) == false);
  t2->next = h2;
  t1->next = h2;
  CHECK(HasCircle(h1) == true);
  LOG(INFO) << CircleLen(h1);
  CHECK(CircleLen(h1) == 10);
  CHECK(CirclePoint(h1)->value == 10);
  LOG(INFO) << CirclePoint(h1)->value;
  return 0;
}
