// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 20:32:17
// File  : code.cc
// Brief :

/*
Given two binary strings, return their sum (also a binary string).
For example,
a = "11"
b = "1"
Return "100".

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 8/8 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 20 milli secs
 * Progress: 294/294 test cases passed.
 *
 * */

#include "base/public/logging.h"

namespace algorithm {

std::string AddBinarySimple(std::string & a, std::string & b) {
  std::string rs = "";

  int carry = 0;
  int i = a.size() - 1;
  int j = b.size() - 1;
  while (i >= 0 || j >= 0 || carry) {
    int vi = i >= 0 ? a[i] - '0' : 0;
    int vj = j >= 0 ? b[j] - '0' : 0;
    int sum = vi + vj + carry;
    carry = sum / 2;
    sum %= 2;
    rs.insert(rs.begin(), char('0' + sum));
    i = (i >= 0 ? i - 1 : i);
    j = (j >= 0 ? j - 1 : j);
  }
  return rs;
  
}

// 最初写的好复杂
std::string AddBinary(std::string & a, std::string & b) {
  std::string rs = "";

  int carry = 0;
  int a_p = a.size() - 1;
  int b_p = b.size() - 1;
  while (a_p >= 0 && b_p >= 0) {
    int tmp = (a[a_p] - '0') + (b[b_p] - '0') + carry;
    char ch = '0' + tmp % 2;
    carry = tmp / 2;
    rs.insert(rs.begin(), ch);
    a_p--;
    b_p--;
  }

  int last = 0;
  std::string * ptr = NULL;
  if (a_p >= 0) {
    last = a_p;
    ptr = &a;
  }
  if (b_p >= 0) {
    ptr = &b;
    last = b_p;
  }

  while (last >= 0) {
    int tmp = ((*ptr)[last] - '0') + carry;
    char ch = '0' + tmp % 2;
    carry = tmp / 2;
    rs.insert(rs.begin(), ch);
    last--;
  }

  while (carry > 0) {
    char ch = '0' + carry % 2;
    carry /= 2;
    rs.insert(rs.begin(), ch);
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string a = "11";
  std::string b = "1";
  LOG(INFO) << AddBinary(a, b);
  LOG(INFO) << AddBinarySimple(a, b);
  return 0;
}
