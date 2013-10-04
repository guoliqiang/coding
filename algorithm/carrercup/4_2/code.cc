// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 21:12:53
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
bool DFS(std::vector<std::vector<int> > & adja,
         std::set<int> & visited,
         int from, int to) {
  visited.insert(from);
  if (from == to) return true;
  int n = adja.size();
  for (int i = 0; i < n; i++) {
    if (adja[from][i] > 0 &&
        !visited.count(i) &&
        DFS(adja, visited, i, to)) return true; 
  }
  return false;
}

bool FindPath(std::vector<std::vector<int> > & adja, int from, int to) {
  std::set<int> visited;
  return DFS(adja, visited, from, to);
}

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<int> > adja(4, std::vector<int>(4, 0));
  adja[0][1] = 1;
  adja[1][0] = 2;
  adja[0][2] = 9;
  adja[2][3] = 10;
  LOG(INFO) << FindPath(adja, 0, 3);
  return 0;
}
