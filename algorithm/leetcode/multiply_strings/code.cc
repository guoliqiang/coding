// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 13:39:44
// File  : code.cc
// Brief :

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

int main(int argc, char** argv) {
  std::string s1;
  std::string s2;
  s1 = "0";
  s2 = "0";
  LOG(INFO) << Multiply(s1, s2);
  return 0;
}
