// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 12:00:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::map<std::string, int> priority;

int ToInt(const std::string & str) {
  int ans = 0;
  for (int i = 0; i < str.size(); i++) {
    ans = ans * 10 + str[i] - '0';
  }
  return ans;
}

std::vector<std::string> Tokenize(const std::string & str) {
  std::vector<std::string> ans;
  std::string cur;
  for (int i = 0; i < str.size(); i++) {
    if (isdigit(str[i])) {
      cur.push_back(str[i]);
    } else {
      if (cur.size()) {
        ans.push_back(cur);
      }
      cur.clear();
      if (str[i] != ' ') {
        ans.push_back(std::string(1, str[i]));
      }
    }
  }
  if (cur.size()) ans.push_back(cur);
  return ans;
}

int Calcu(std::vector<std::string> & parts) {
  if (parts.size() == 0) return 0;
  if (parts.size() == 1) return ToInt(parts[0]);

  std::stack<std::string> op;
  std::stack<int> data;
  for (int i = 0; i < parts.size(); i++) {
    if (parts[i] == "+" || parts[i] == "-" ||
        parts[i] == "/" || parts[i] == "*") {
      while (op.size() > 0 && priority[op.top()] >= priority[parts[i]]) {
        int v1 = data.top(); data.pop();
        int v2 = data.top(); data.pop();
        if (op.top() == "+") {
          data.push(v1 + v2);
        } else if (op.top() == "-") {
          data.push(v2 - v1);
        } else if (op.top() == "*") {
          data.push(v2 * v1);
        } else {
          data.push(v2 / v1);
        }
        op.pop();
      }
      op.push(parts[i]);
    } else {
      data.push(ToInt(parts[i]));
    }
  }
  while (op.size()) {
    int v1 = data.top(); data.pop();
    int v2 = data.top(); data.pop();
    if (op.top() == "+") {
      data.push(v1 + v2);
    } else if (op.top() == "-") {
      data.push(v2 - v1);
    } else if (op.top() == "*") {
      data.push(v2 * v1);
    } else if (op.top() == "/") {
      data.push(v2 / v1);
    }
    op.pop();
  }
  return data.top();
}

int Calculate(const std::string & str) {
  priority["*"] = 2;
  priority["/"] = 2;
  priority["+"] = 1;
  priority["-"] = 1;
  std::vector<std::string> parts = Tokenize(str);
  LOG(INFO) << JoinVector(parts);
  return Calcu(parts);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << Calculate("1*2-3/4+5*6-7*8+9/10");
  return 0;
}
