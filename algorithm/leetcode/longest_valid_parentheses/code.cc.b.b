// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 10:12:00
// File  : code.cc
// Brief :
#include <algorithm>
#include <vector>
#include "base/public/logging.h"

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
    if (left_count >= right_count) {
      // LOG(INFO) << begin << " " << end;
      // if (end - begin + 1 > max) max = end - begin + 1;
      if (2 * right_count > max) max = 2 * right_count;
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
  LOG(INFO) << "max:" << max  << " begin:" << begin;
  if (right_count < left_count && right_count > 0) {
    end = str.size() - 1;
    left_count = 0;
    right_count = 0;
    while (str[end] == '(') end--;
    int new_end = end;
    
    while (end >= begin) {
      if (str[end] == '(') left_count++;
      if (str[end] == ')') right_count++;
      if (left_count == right_count) {
        if (new_end - end + 1> max) max = new_end - end + 1;
      }
      if (left_count > right_count) break;
      end--;
    }
  }
  return max;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = ")(((((()())()()))()(()))(";
  LOG(INFO) << LongestValidParentheses(str);
  return 0;
}
