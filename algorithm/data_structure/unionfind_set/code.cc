// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-16 10:15:29
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// 不用另外设置cnt变量
const int MAXN = 1000;
int N;
int root[MAXN];

void Init() {
  memset(root, -1, sizeof(root));
}

int Find(int k) {
  if (root[k] < 0) return k;
  else return root[k] = Find(root[k]);
}

bool Union(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return false;
  if (abs(root[px]) > abs(root[py])) {
    root[py] += root[px];
    root[px] = py;
  } else {
    root[px] += root[py];
    root[py] = px;
  }
  return true;
}

}  // namespace algorithm

// 常用版本
namespace algorithm2 {
const int MAXN = 10000;
int N;
int root[MAXN];

void Init() {
  for (int i = 0; i < N; i++) root[i] = i;
}

int Find(int k) {
  if (root[k] == k) return k;
  return root[k] = Find(root[k]);
}

bool Union(int x, int y) {
  int px = Find(x);
  int py = Find(y);
  if (px == py) return false;
  root[px] = py;
  return true;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
