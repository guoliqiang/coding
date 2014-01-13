// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-03 15:39:43
// File  : code.cc
// Brief :

// http://blog.csdn.net/maqingli20/article/details/7361057
#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100;
int N;
const int MAXK = 100;
int start[MAXN];
int end[MAXN];
int top[MAXK];
int K;
int min = 7;

void Build() {
  for (int i = 1; i <= K; i++) {
    top[i] = 0;
    for (int j = 1; j <= N; j++) {
      if (start[j] == i) {
        top[i] = j;
        break;
      }
    }
  }
}

bool Ok() {
  for (int i = 1; i <= N; i++) {
    if (start[i] != end[i]) return false;
  }
  return true;
}

void Trace(int depth, std::vector<std::pair<int, int> > & path, std::vector<std::pair<int, int> > & rs) {
  if (depth > min) return;
  for (int i = 1; i <= K; i++) {
    for (int j = 1; j <= K; j++) {
      if (top[j] > top[i]) {
        start[top[i]] = j;
        path.push_back(std::make_pair(i, j));
        if (Ok() && (rs.size() == 0 || rs.size() > path.size())) {
          rs = path;
          min = rs.size();
        }
        Trace(depth + 1, path, rs);
        path.pop_back();
        start[top[j]] = i;
      }
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
