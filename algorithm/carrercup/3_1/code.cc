// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 16:22:25
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

/*
 * 这种方式可能会因为某个stack数据比较多而浪费空间
 *
 *
 * http://hawstein.com/posts/3.1.html
 * */

class Stack {
 public:
  Stack(std::vector<int> * v_i, int last_i) : v(v_i), last(last_i) {}
  
  void Push(int p) {
    if (last + 1 > v->size()) {
      int t = v->size() == 0 ? 10 : v->size() * 2;
      v->resize(t);
    }
    (*v)[last] = p;
    last += 3;
  }

  int Top() {
    return last - 3 >= 0 ? (*v)[last - 3] : 0;
  }

  void Pop() {
    last -= 3;
  }

  bool Empty() {
    return last < 3;
  }

  std::vector<int> * v;
  int last;
};


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> foo;
  Stack s1(&foo, 0);
  Stack s2(&foo, 1);
  Stack s3(&foo, 2);
  for (int i = 0; i < 10; i++) {
    s1.Push(i * 10 + i);
    s2.Push(i * 20 + i);
    s3.Push(i * 30 + i);
  }
  while (!s1.Empty()) {
    LOG(INFO) << s1.Top();
    s1.Pop();
  }

  while (!s2.Empty()) {
    LOG(INFO) << s2.Top();
    s2.Pop();
  }

  while (!s3.Empty()) {
    LOG(INFO) << s3.Top();
    s3.Pop();
  }
  return 0;
}

