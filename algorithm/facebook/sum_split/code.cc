// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 21:15:06
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
void Trace(std::vector<std::vector<int> > & rs, std::vector<int> & path, int cur, int t) {
  if (cur == t) rs.push_back(path);
  if (cur >= t) return;
  for (int i = 1; i <= t; i++) {
    path.push_back(i);
    Trace(rs, path, cur + i, t);
    path.pop_back();
  }
}

std::vector<std::vector<int> > Trace(int t) {
  std::vector<std::vector<int> > rs;
  std::vector<int> path;
  Trace(rs, path, 0, t);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > rs = Trace(4);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
