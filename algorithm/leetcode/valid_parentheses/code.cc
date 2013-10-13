// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-22 13:07:38
// File  : code.cc
// Brief :
/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 12/12 test cases passed.
 *
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 61/61 test cases passed.
 *
 * */
#include <stack>
#include "base/public/logging.h"

namespace algorithm {
bool IsValid(std::string s) {
  std::stack<char> foo;
  for (int i = 0; i< s.size(); i++) {
    if (s[i] == '(' || s[i] == '{' || s[i] == '[') {
      foo.push(s[i]);
    } else {
      if (foo.empty()) return false;
      else {
        char ch = foo.top();
        if (s[i] == ')' && ch != '(') return false;
        if (s[i] == ']' && ch != '[') return false;
        if (s[i] == '}' && ch != '{') return false;
        foo.pop();
      }
    }
  }
  return foo.empty();
}
}  // namespace algorithm

namespace twice {
bool isValid(string s) {
  std::stack<char> foo;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == ')' || s[i] == '}' || s[i] == ']') {
      if (foo.empty()) return false;
      char ch = foo.top();
      foo.pop();
      if (s[i] == ')' && ch != '(') return false;
      if (s[i] == '}' && ch != '{') return false;
      if (s[i] == ']' && ch != '[') return false;
    } else {
      foo.push(s[i]);
    }
  }
  return foo.empty();
}
}  // namespace twice

int main(int argc, char** argv) {

  return 0;
}
