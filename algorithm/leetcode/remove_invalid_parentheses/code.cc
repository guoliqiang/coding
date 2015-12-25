// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-24 18:15:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// BFS
namespace algorithm {
bool IsValid(const std::string & str) {
  int left = 0;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == '(') left++;
    else if (str[i] == ')') {
      if (left <= 0) return false;
      else left--;
    }
  }
  return left == 0;
}
std::vector<std::string> RemoveInvalidParentheses(const std::string & s) {
  std::vector<std::string> ans;

  std::list<std::string> queue;
  std::set<std::string> visited;

  queue.push_back(s);
  bool flag = false;
  while (queue.size()) {
    std::string cur = queue.front();
    queue.pop_front();
    if (IsValid(cur)) {
      ans.push_back(cur);
      flag = true;
    }
    if (flag) continue;
    for (int i = 0; i < cur.size(); i++) {
      if (cur[i] == '(' || cur[i] == ')') {
        std::string tmp = cur.substr(0, i) + cur.substr(i + 1);
        if (!visited.count(tmp)) {
          queue.push_back(tmp);
          visited.insert(tmp);
        }
      }
    }
  }
  return ans;
}

}  // namespace algorithm

// DFS

namespace NB {

int InValidNum(const std::string & str) {
  int left = 0;
  int ans = 0;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == '(') left++;
    if (str[i] == ')') {
      if (left == 0) ans++;
      else left--;
    }
  }
  ans += left;
  return ans;
}

void Sub(const std::string & s, std::vector<std::string> & ans,
    std::set<std::string> & visited) {
  int num = InValidNum(s);
  if (num == 0) {
    ans.push_back(s);
    return;
  }

  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '(' || s[i] == ')') {
      std::string cur = s.substr(0, i) + s.substr(i + 1);
      if (!visited.count(cur) && InValidNum(cur) < num) {
        visited.insert(cur);
        Sub(cur, ans, visited);
      }
    }
  }
}

std::vector<std::string> RemoveInvalidParentheses(const std::string & s) {
  std::vector<std::string> ans;
  std::set<std::string> visited;
  Sub(s, ans, visited);
  return ans;
}

}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << JoinVector(RemoveInvalidParentheses("()())()"));
  LOG(INFO) << JoinVector(NB::RemoveInvalidParentheses("()())()"));
  LOG(INFO) << JoinVector(RemoveInvalidParentheses("(a)())()"));
  LOG(INFO) << JoinVector(NB::RemoveInvalidParentheses("(a)())()"));
  LOG(INFO) << JoinVector(RemoveInvalidParentheses(")("));
  LOG(INFO) << JoinVector(NB::RemoveInvalidParentheses(")("));
  return 0;
}
