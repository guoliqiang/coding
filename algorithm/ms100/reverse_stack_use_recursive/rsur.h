// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 16:21:58
// File  : rsur.h
// Brief :
// 翻转一个栈，要求基于递归

#ifndef  __RSUR_H_
#define  __RSUR_H_

#include <stack>
#include "base/public/logging.h"

namespace algorithm {

template <typename type>
void PushToEnd(std::stack<type> & s, type v) {
  if (s.empty()) s.push(v);
  else {
    type foo = s.top();
    s.pop();
    PushToEnd(s, v);
    s.push(foo);
  }
}

template <typename type>
void ReverseStack(std::stack<type> & s) {
  if (s.empty()) return;
  else {
    type foo = s.top();
    s.pop();
    ReverseStack(s);
    PushToEnd(s, foo);
  }
}


}  // namespace algorithm

#endif  //  __RSUR_H_
