// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 19:38:06
// File  : code.cc
// Brief :

/*
The count-and-say sequence is the sequence of integers beginning as follows:
1, 11, 21, 1211, 111221, ...

1 is read off as "one 1" or 11.
11 is read off as "two 1s" or 21.
21 is read off as "one 2, then one 1" or 1211.
Given an integer n, generate the nth sequence.

Note: The sequence of integers will be represented as a string.

*/

#include "stdio.h"
#include "base/public/logging.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 32 milli secs
 * Progress: 18/18 test cases passed.
 *
 * */

namespace algorithm {

std::string IntToString(int n) {
  char buf[20];
  sprintf(buf, "%d", n);
  return std::string(buf);
}

std::string CountAndSay(int n) {
  std::string pre= "1";
  if (n == 1) return pre;
  std::string rs = "";
  for (int i = 2; i <= n; i++) {
    rs.clear();
    char foo = pre[0];
    int count = 1;
    for (int i = 1; i < pre.size(); i++) {
      if (pre[i] == foo) count++;
      else {
        rs += IntToString(count) + foo;
        foo = pre[i];
        count = 1;
      }
    }
    rs += IntToString(count) + foo;
    pre = rs;
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;

void Say(int k, int n, std::string & str) {
  if (k == n) return;
  else {
    std::string foo = "";
    char ch = str[0];
    int count = 1;
    for (int i = 1; i < str.size(); i++) {
      if (str[i] == ch) count++;
      else {
        foo.push_back(count + '0');
        foo.push_back(ch);
        ch = str[i];
        count = 1;
      }
    }
    foo.push_back(count + '0');
    foo.push_back(ch);
    str = foo;
    Say(k + 1, n, str);
  }
}

std::string Say(int n) {
    std::string rs = "1";
    Say(1, n, rs);
    return rs;
}

class Solution {
public:
    string countAndSay(int n) {
      return Say(n);
    }
};
}  // namespace twice

int main(int argc, char** argv) {
  LOG(INFO) << CountAndSay(3);
  LOG(INFO) << CountAndSay(5);
  LOG(INFO) << CountAndSay(50);
  return 0;
}
