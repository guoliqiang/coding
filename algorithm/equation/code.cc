// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 15:12:44
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
std::map<std::string, int> priority;
void Init() {
  priority["+"] = 0;
  priority["-"] = 0;
  priority["*"] = 1;
  priority["/"] = 1;
}

// 波兰逆式
std::vector<std::string> Polish(std::vector<std::string> & v) {
  std::vector<std::string> rs;
  std::stack<std::string> stack;
  for (int i = 0; i < v.size(); i++) {
    std::string s = v[i];
    if (s == "(") stack.push(s);
    else if (s == ")") {
      while (stack.top() != "(") {
        rs.push_back(stack.top());
        stack.pop();
      }
      stack.pop();
    } else if (s == "+" || s == "-" || s == "*" || s == "/") {
      while(true) {
        if (!stack.empty() && stack.top() != "(" && priority[stack.top()] > priority[s]) {
          rs.push_back(stack.top());
          stack.pop();
        } else break;
      }
      stack.push(s);
    } else {
      rs.push_back(s);
    }
  }
  while (!stack.empty()) {
    rs.push_back(stack.top());
    stack.pop();
  }
  return rs;
}

int ToInt(std::string & str) {
  int rs = 0;
  for (int i = 0; i < str.size(); i++) {
    rs = rs * 10 + (str[i] - '0');
  }
  return rs;
}

// 计算波兰逆式
double Eval(std::vector<std::string> & v) {
  std::stack<double> stack;
  for (int i = 0; i < v.size(); i++) {
    if (v[i] == "+") {
      double op1 = stack.top();
      stack.pop();
      double op2 = stack.top();
      stack.pop();
      stack.push(op1 + op2);
    } else if (v[i] == "-") {
      double op1 = stack.top();
      stack.pop();
      double op2 = stack.top();
      stack.pop();
      stack.push(op2 - op1);
    } else if (v[i] == "*") {
      double op1 = stack.top();
      stack.pop();
      double op2 = stack.top();
      stack.pop();
      stack.push(op1 * op2);
    } else if (v[i] == "/") {
      double op1 = stack.top();
      stack.pop();
      double op2 = stack.top();
      stack.pop();
      stack.push(op2 / op1);
    } else stack.push(ToInt(v[i]));
  }
  return stack.top();
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Init();
  std::vector<std::string> v;
  v.push_back("1");
  v.push_back("*");
  v.push_back("(");
  v.push_back("2");
  v.push_back("+");
  v.push_back("1");
  v.push_back(")");
  v.push_back("/");
  v.push_back("5");
  std::vector<std::string> pl = Polish(v);
  LOG(INFO) << JoinVector(pl);
  LOG(INFO) << Eval(pl);
  return 0;
}
