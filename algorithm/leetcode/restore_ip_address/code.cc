// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 16:47:17
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 24/24 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 147/147 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool Bigger255(std::string & s) {
  if (s[0] > '2') return true;
  if (s[0] == '2' && s[1] > '5') return true;
  if (s[0] == '2' && s[1] == '5' && s[2] > '5') return true;
  return false;
}

void Trace(std::string & str, std::string path, int n, int len, std::vector<std::string> & rs) {
  if (n == 5) {
    // LOG(INFO) << len << " " << str.size() << " " << path;
    if (len == str.size()) {
      rs.push_back(path.substr(0, path.size() - 1));
    }
  } else {
    if (str.size() - len > (5 - n) * 3) return;
    for (int i = 0; i < 3; i++) {
      if (len + i + 1 > str.size()) continue;
      std::string foo = str.substr(len, i + 1);
      if (foo.size() > 1 && foo[0] == '0') continue;
      if (i == 2 && Bigger255(foo)) continue;
      Trace(str, path + foo + ".", n + 1, len + foo.size(), rs);
    }
  }
}

std::vector<std::string> RestoreIpAddress(std::string & str) {
  std::vector<std::string> rs;
  std::string path = "";
  Trace(str, path, 1, 0, rs);
  return rs;
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "010010";
  std::vector<std::string> rs = RestoreIpAddress(str);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
