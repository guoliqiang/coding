// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-16 16:25:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int db[1024 * 1024] = {0};
std::map<char, int> m;

std::vector<std::string> FindRepeatedDnaSequences(const std::string & s) {
  m['A'] = 0;
  m['C'] = 1;
  m['G'] = 2;
  m['T'] = 3;
  memset(db, 0, sizeof(db));

  std::vector<std::string> ans;
  int hash = 0;
  for (int i = 0; i < s.size(); i++) {
    hash = ((hash << 2) & 0xFFFFF) | m[s[i]];
    if (i < 9) continue;
    db[hash]++;
    if (db[hash] == 2) ans.push_back(s.substr(i - 9, 10));
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) <<
    JoinVector(FindRepeatedDnaSequences("AAAAAAAAAAAA"));
  return 0;
}
