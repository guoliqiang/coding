// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 11:02:43
// File  : stack_sequence.h
// Brief :

#ifndef  __STACK_SEQUENCE_H_
#define  __STACK_SEQUENCE_H_

#include <stack>
#include "base/public/logging.h"

namespace algorithm {
bool is_stack_sequence(std::string input,
                       std::string output) {
  if (input.size() != output.size()) return false;
  std::stack<char> foo;
  for (int i = 0; i < output.size(); i++) {
    while (foo.empty() || foo.top() != output[i]) {
      if (input.size() == 0) return false;
      foo.push(input[0]);
      input.erase(input.begin());
    }
    foo.pop();
  }
  return true;
}
}  // namespace algorithm

#endif  //  __STACK_SEQUENCE_H_
