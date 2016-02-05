// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-20 23:21:53
// File  : code.h
// Brief :

/*
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this:
(you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number of rows:

string convert(string text, int nRows);
convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR".
*/

#ifndef  __CODE_H_
#define  __CODE_H_

#include <vector>
#include "base/public/logging.h"

namespace algorithm {
std::string zigzag(std::string text, int n) {
  if (n == 1) return text;
  std::vector<std::string> foo(n, std::string(""));
  int index = 0;
  int bar = 0;
  bool flag = false;
  for (int i = 0; i < text.size(); i++) {
    if (flag) {
      foo[n - 2 - bar].append(&text[i], 1);
    } else {
      foo[bar].append(&text[i], 1);
    }
    index = (index + 1) % (2 * n - 2);
    bar = index % n;
    if (n > 2 && bar == 0) flag = (!flag);
  }
  std::string rs = "";
  for (int i = 0; i < foo.size(); i++) {
    rs += foo[i];
  }
  return rs;
}
}  // namespace algorithm

namespace twice {

std::string Convert(std::string s, int row) {
  if (row <= 1) return s;  // 这个必须特殊处理
  int idx = 0;
  bool flag = true;
  std::vector<std::string> v(row, std::string(""));
  for (int i = 0; i < s.size(); i++) {
    v[idx].append(&s[i], 1);
    if (flag) {
      idx++;
      if (idx == row) {
        idx -= 2;
        flag = !flag;
      }
    } else {
      idx--;
      if (idx == -1) {
        idx += 2;
        flag = !flag;
      }
    }
  }
  std::string rs = "";
  for (int i = 0; i < row; i++) rs += v[i];
  return rs;
}
}  // namespace twice

namespace NB {
using namespace std;

string convert(string s, int numRows) {
    if (numRows <= 1) return s;
    
    std::vector<std::string> vec(numRows, "");
    int idx = 0;
    bool add = true;
    for (int i = 0; i < s.size(); i++) {
        vec[idx].push_back(s[i]);
        
        if (idx == numRows - 1) add = false;
        if (idx == 0) add = true;
        
        if (add) idx += 1;
        else idx -= 1;
    }
    std::string ans;
    for (int i = 0; i < numRows; i++) ans += vec[i];
    return ans;
}
}  // namespace NB

namespace NB2 {
using namespace std;

string convert(string s, int numRows) {
    std::vector<std::string> vec(numRows, "");
    int idx = 0;
    while (idx < s.size()) {
        for (int i = 0; i < numRows && idx < s.size(); i++) vec[i].push_back(s[idx++]);
        for (int i = numRows - 2; i > 0 && idx < s.size(); i--) vec[i].push_back(s[idx++]);
    }
    std::string ans;
    for (int i = 0; i < numRows; i++) {
        ans += vec[i];
    }
    return ans;
}
}  // namespace NB2

#endif  //__CODE_H_
