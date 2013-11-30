// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-29 16:08:05
// File  : code.cc
// Brief :

/*
Evaluate the value of an arithmetic expression in Reverse Polish Notation.
Valid operators are +, -, *, /. Each operand may be an integer or another expression.

Some examples:
  ["2", "1", "+", "3", "*"] -> ((2 + 1) * 3) -> 9
  ["4", "13", "5", "/", "+"] -> (4 + (13 / 5)) -> 6

另一道题：生成波兰逆式
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
void GetOp(std::stack<int> & stack, int & x, int & y) {
  x = stack.top();
  stack.pop();
  y = stack.top();
  stack.pop();
}

int ToInt(std::string & str) {
  int rs = 0;
  int n = str.size();
  bool minus = false;
  for (int i = 0; i < n; i++) {
    if (str[i] == '-') {
      minus = true;
      continue;
    }
    if (str[i] == '+') continue;
    rs = rs * 10 + (str[i] - '0');
  }
  return minus ? -rs : rs;
}

int PRN(std::vector<std::string> & token) {
  int x = 0;
  int y = 0;
  std::stack<int> stack;
  for (int i = 0; i < token.size(); i++) {
    if (token[i] == "+") {
      GetOp(stack, x, y);
      stack.push(x + y);
    } else if (token[i] == "-") {
      GetOp(stack, x, y);
      stack.push(y - x);
    } else if (token[i] == "*") {
      GetOp(stack, x, y);
      stack.push(x * y);
    } else if (token[i] == "/") {
      GetOp(stack, x, y);
      stack.push(y / x);
    } else {
      stack.push(ToInt(token[i]));
    }
  }
  return stack.top();
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::string> data;
  data.push_back("2");
  data.push_back("1");
  data.push_back("+");
  data.push_back("3");
  data.push_back("*");
  LOG(INFO) << PRN(data);
  data.clear();
  data.push_back("4");
  data.push_back("13");
  data.push_back("5");
  data.push_back("/");
  data.push_back("+");
  LOG(INFO) << PRN(data);
  return 0;
}
