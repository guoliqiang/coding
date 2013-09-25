// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 19:38:06
// File  : code.cc
// Brief :
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

int main(int argc, char** argv) {
  LOG(INFO) << CountAndSay(3);
  LOG(INFO) << CountAndSay(5);
  LOG(INFO) << CountAndSay(50);
  return 0;
}
