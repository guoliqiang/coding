// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-19 14:47:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 101;
int v[MAXN][MAXN];
struct Node {
 int x;
 int y;
} stack[MAXN * MAXN];

int index = 0;
int M;
int N;


bool Trace(int x, int y) {
  stack[index].x = x;
  stack[index].y = y;
  v[x][y] = 1;
  index++;
  if (index == M * N) return true;
  if (x - 1 >= 0 && v[x - 1][y] == 0 && Trace(x - 1, y)) return true;
  if (x + 1 < M && v[x + 1][y] == 0 && Trace(x + 1, y)) return true;
  if (y - 1 >= 0 && v[x][y - 1] == 0 && Trace(x, y - 1)) return true;
  if (y + 1 < N && v[x][y + 1] == 0 && Trace(x, y + 1)) return true;
  index--;
  v[x][y] = 0;
  return false;
}

void Read() {
  while (scanf("%d%d", &M, &N) != EOF) {
    getchar();
    index = 0;
    memset(v, 0, sizeof(v));
    int x, y;
    scanf("%d%d", &x, &y);
    getchar();
    if (!Trace(x - 1, y - 1)) printf("NO\n");
    else {
      for (int i = 0; i < M * N; i++) {
        printf("%d %d\n", stack[i].x + 1, stack[i].y + 1);
      }
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
