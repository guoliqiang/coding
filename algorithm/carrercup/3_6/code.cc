// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 18:52:29
// File  : code.cc
// Brief :
//
// 这个思路比较有意思
// http://hawstein.com/posts/3.6.html
// 大家都能想到的是把所有的pop出来然后sort
// 再push进去

#include "base/public/common_head.h"

namespace algorithm {

template <typename type>
std::stack<type> SortStack(std::stack<type> & s) {
  std::stack<type> foo;
  while (!s.empty()) {
    type t= s.top();
    s.pop();
    while (!foo.empty() && foo.top() > t) {
      s.push(foo.top());
      foo.pop();
    }
    foo.push(t);
  }
  return foo;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::stack<int> foo;
  foo.push(5);
  foo.push(1);
  foo.push(9);
  foo.push(2);
  foo.push(3);
  foo = SortStack(foo);
  while (!foo.empty()) {
    LOG(INFO) << foo.top();
    foo.pop();
  }
  return 0;
}
