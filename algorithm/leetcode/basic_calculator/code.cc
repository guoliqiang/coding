// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-22 12:00:03
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

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
    if (parts[i] == "(") {
      op.push(parts[i]);
    } else if (parts[i] == "+" || parts[i] == "-") {
      if (op.size() > 0 && op.top() != "(") {
        int v1 = data.top(); data.pop();
        int v2 = data.top(); data.pop();
        if (op.top() == "+") {
          data.push(v1 + v2);
        } else {
          data.push(v2 - v1);
        }
        op.pop();
      }
      op.push(parts[i]);
    } else if (parts[i] == ")") {
      while (op.top() != "(") {
        int v1 = data.top(); data.pop();
        int v2 = data.top(); data.pop();
        if (op.top() == "+") {
          data.push(v1 + v2);
        } else {
          data.push(v2 - v1);
        }
        op.pop();
      }
      op.pop();
    } else {
      data.push(ToInt(parts[i]));
    }
  }
  while (op.size()) {
    int v1 = data.top(); data.pop();
    int v2 = data.top(); data.pop();
    if (op.top() == "+") {
      data.push(v1 + v2);
    } else {
      data.push(v2 - v1);
    }
    op.pop();
  }
  return data.top();
}

int Calculate(const std::string & str) {
  std::vector<std::string> parts = Tokenize(str);
  LOG(INFO) << JoinVector(parts);
  return Calcu(parts);
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {

using namespace std;

std::vector<std::string> Split(const std::string & str) {
    std::string cur;
    std::vector<std::string> ans;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            if (cur.size()) ans.push_back(cur);
            cur.clear();
        } else if (str[i] == '(' || str[i] == ')' || str[i] == '+' || str[i] == '-') {
            if (cur.size()) ans.push_back(cur);
            cur.clear();
            ans.push_back(str.substr(i, 1));
        } else {
            cur.push_back(str[i]);
        }
    }
    if (cur.size()) ans.push_back(cur);
    return ans;
}

int ToInt(const std::string & str) {
    return atoi(str.c_str());
}

void One(std::stack<std::string> & op1, std::stack<int> & op2) {
    if (op1.top() == "+") {
        LOG(INFO) << op2.size();
        int t1 = op2.top(); op2.pop();
        int t2 = op2.top(); op2.pop();
        op2.push(t2 + t1);
    } else {
        int t1 = op2.top(); op2.pop();
        int t2 = op2.top(); op2.pop();
        op2.push(t2 - t1);
    }
    op1.pop();
}

int Calculate(string s) {
    std::stack<std::string> op1;
    std::stack<int> op2;
    std::vector<std::string> parts = Split(s);
    LOG(INFO) << JoinVector(parts);

    for (int i = 0; i < parts.size(); i++) {
        if (parts[i] == "(") {
            op1.push(parts[i]);
        } else if (parts[i] == ")") {
            while (op1.top() != "(") One(op1, op2);
            op1.pop();
        } else if (parts[i] == "+" || parts[i] == "-"){
            if (op1.size() == 0 || op1.top() == "(") {
                op1.push(parts[i]);
            } else {
                One(op1, op2);
                op1.push(parts[i]);
            }
        } else {
            LOG(INFO) << "push " << parts[i];
            op2.push(ToInt(parts[i]));
        }
    }
    while (op1.size()) {
        LOG(INFO) << op1.size()  << " op1 size";
        One(op1, op2);
    }
    return op2.top();
}
}  // namespace twice

int main(int argc, char** argv) {
  LOG(INFO) << twice::Calculate("1 + 1");
  LOG(INFO) << Calculate("(2-1) + 2 ");
  LOG(INFO) << Calculate("(1+(4+5+2)-3)+(6+8)");
  LOG(INFO) << Calculate("0");
  return 0;
}
