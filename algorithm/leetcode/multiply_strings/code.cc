// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 13:39:44
// File  : code.cc
// Brief :

/*
Given two numbers represented as strings, return multiplication of the numbers as a string.
Note: The numbers can be arbitrarily large and are non-negative.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 11/11 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 40 milli secs
 * Progress: 311/311 test cases passed.
 *
 * */

#include "base/public/logging.h"

namespace algorithm {

std::string Multiply(std::string & s1, std::string &s2) {
  if (s1 == "0" || s2 == "0") return "0";
  std::string rs(s1.size() + s2.size() + 1, '0');
  int begin = rs.size() - 1;
  for (int i = s1.size() - 1; i >= 0; i--) {
    int carry = 0;
    int num = 0;
    for (int j = s2.size() - 1; j >= 0; j--) {
      int t = (s1[i] - '0') * (s2[j] - '0') + carry + (rs[begin - num] - '0');
      rs[begin - num] = '0' + t % 10;
      carry = t / 10;
      num++;
    }
    while (carry) {
      int t = rs[begin - num] - '0' + carry % 10;
      rs[begin - num] = '0' + t % 10;
      carry = t / 10;
      num++;
    }
    begin--;
  }
  return rs.substr(rs.find_first_not_of('0'));
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;
string multiply(string num1, string num2) {
  if (num1 == "0" || num2 == "0") return "0";
  int len1 = num1.size();
  int len2 = num2.size();
  std::string rs(len1 + len2 + 1, '0');
  int idx = rs.size() - 1;
  for (int i = len1 - 1; i >= 0; i--) {
    int carry = 0;
    int cur = idx;
    for (int j = len2 - 1; j >=  0; j--) {
      int t = (num2[j] - '0') * (num1[i] - '0') + carry + (rs[cur] - '0');
      carry = t / 10;
      t %= 10;
      rs[cur--] = '0' + t;
    }
    while (carry) {
      int t = carry + (rs[cur] - '0');
      carry = t / 10;
      t %= 10;
      rs[cur--] = '0' + t;
    }
    idx--;
  }
  int i = 0;
  while (rs[i] == '0') i++;
  return rs.substr(i);
}
}  // namespace  twice

namespace third {
std::string Multiply(std::string num1, std::string num2) {
  std::string rs(num1.size() + num2.size() + 1, '0');
  int idx = rs.size() - 1;
  for (int i = num1.size() - 1; i >= 0; i--) {
    int c = 0;
    int k = idx--;
    for (int j = num2.size() - 1; j >= 0 || c > 0; j--) {
      if (j >= 0) c += (num2[j] - '0') * (num1[i] - '0');
      c += rs[k] - '0';
      rs[k--] = c % 10 + '0';
      c /= 10;
    }
  }
  int i = 0;
  while (i < rs.size() - 1 && rs[i] == '0') i++;
  return rs.substr(i);
}
}  // namespace third

int main(int argc, char** argv) {
  std::string s1;
  std::string s2;
  s1 = "98";
  s2 = "9";
  // LOG(INFO) << Multiply(s1, s2);
  LOG(INFO) << twice::multiply(s1, s2);
  return 0;
}
