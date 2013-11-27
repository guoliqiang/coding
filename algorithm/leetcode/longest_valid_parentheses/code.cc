// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 10:12:00
// File  : code.cc
// Brief :

/*
Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.
For "(()", the longest valid parentheses substring is "()", which has length = 2.
Another example is ")()())", where the longest valid parentheses substring is "()()", which has length = 4.

*/

#include <algorithm>
#include <vector>
#include <stack>
#include "base/public/logging.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 96 milli secs
 * Progress: 229/229 test cases passed.
 *
 * */

namespace algorithm {

int LongestValidParentheses(std::string & str) {
  int begin = 0;
  int end = 0;
  int max = 0;
  int left_count = 0;
  int right_count = 0;
  while (end < str.size()) {
    if (str[end] == '(') left_count++;
    if (str[end] == ')') right_count++;
    if (left_count >= right_count && str[end] == ')') {
      int t_e = end;
      int t_l = 0;
      int t_r = 0;
      while (t_e >= begin) {
        if (str[t_e] == '(') t_l++;
        if (str[t_e] == ')') t_r++;
        if (t_l == t_r) {
          if (end - t_e + 1> max) max = end - t_e + 1;
        }
        if (t_l > t_r) break;
        t_e--;
     }

    }
    if (right_count > left_count) {
      end++;
      begin = end;
      left_count = 0;
      right_count = 0;
    } else {
      end++;
    }
    // LOG(INFO) << "begin:" << begin << " end:" << end
    //           << " left:" << left_count
    //           << " right:" << right_count;
  }
  return max;
}
}  // namespace algorithm

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 40 milli secs
 * Progress: 229/229 test cases passed.
 * http://discuss.leetcode.com/questions/212/longest-valid-parentheses
 *
 * 这种题目解决方法一定是基于栈的
 *
 * */

namespace other {
int LongestValidParentheses(std::string & str) {
  std::stack<int> foo;
  int max = 0;
  int last = -1;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == '(') foo.push(i);
    else {
      if (foo.empty()) {
        last = i;
      } else {
        foo.pop();
        if (foo.empty()) max = std::max(max, i - last);
        else max = std::max(max, i - foo.top());
      }
    }
  }
  return max;
}
}

namespace twice {
int Longest(std::string s) {
  std::stack<int> stack;
  int max = 0;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == ')') {
      if (stack.empty() || s[stack.top()] != '(') stack.push(i);
      else {
        stack.pop();
        if (stack.empty()) max = std::max(max, i + 1);
        else max = std::max(max, i - stack.top());
      }
    } else {
      stack.push(i);
    }
  }
  return max;
}
}  // namesapce twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "(())()(()((";
  LOG(INFO) << LongestValidParentheses(str);
  LOG(INFO) << other::LongestValidParentheses(str);
  return 0;
}
