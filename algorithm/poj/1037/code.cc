// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-23 10:33:37
// File  : code.cc
// Brief :

// http://hi.baidu.com/flower_mlh/item/b878d94cfae1aba7df2a9ff2
// http://blog.csdn.net/lencle/article/details/7211941
// Accepted 180K  32MS

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 30;
long long up[MAXN][MAXN];
long long down[MAXN][MAXN];
int path[MAXN];
int N = 0;
long long C = 0;

void Dp() {
  memset(up, 0, sizeof(up));
  memset(down, 0, sizeof(down));
  up[1][1] = 1;
  down[1][1] = 1;
  
  for (int i = 2; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      long long t = 0;
      for (int k = j - 1; k >= 1; k--) t += up[k][i - 1];
      down[j][i] = t;
      t = 0;
      for (int k = j; k <= i - 1; k++) t += down[k][i - 1];
      up[j][i] = t;
      // LOG(INFO) << "down[" << j << "][" << i << "]:" << down[j][i] << " up[" << j << "][" << i << "]:" << up[j][i];
    }
  }
}


void Trace(long long n, int k, bool f) {
  if (k > N) return;
  int i;
  int t = path[k - 1];
  
  if (f == true) {
    for (i = 1; i < t; i++) {
      if (n > up[i][N - k + 1]) n -= up[i][N - k + 1];
      else break;
    }
  } else {
    for (i = t; i <= N; i++) {
      if (n > down[i][N - k + 1]) n -= down[i][N - k + 1];
      else break;
    }
  }
  path[k] = i;
  Trace(n, k + 1, !f);
  
  for (int j = k + 1; j <= N; j++) {
    if (path[j] >= i) path[j]++;
  }
}

void Trace(long long n) {
  memset(path, 0, sizeof(path));
  int i;
  for (i = 1; i <= N; i++) {
    long long t = down[i][N] + up[i][N];
    if (n > t) n -= t;
    else break;
  }
  path[1] = i;
  if (n > down[i][N]) Trace(n - down[i][N], 2, false);
  else Trace(n, 2, true);

  for (int j = 2; j <= N; j++) {
    if (path[j] >= i) path[j]++;
  }
}

void Solve() {
  Trace(C);
  for (int i = 1; i <= N; i++) printf("%d ", path[i]);
  printf("\n");
}

void Read() {
  N = 22;
  Dp();
  
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    scanf("%d%lld", &N, &C);
    getchar();
    Solve();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
