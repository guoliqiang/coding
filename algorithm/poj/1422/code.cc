// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-14 03:57:09
// File  : code.cc
// Brief :

/* Accepted  156k  0MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 150;
int N = 0;
int matrix[MAXN][MAXN];
int link[MAXN];
int visited[MAXN];

bool Search(int k) {
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0 && visited[i] == 0) {
      visited[i] = 1;
      if (link[i] == -1 || Search(link[i])) {
        link[i] = k;
        return true;
      }
    }
  }
  return false;
}

int MaxMatch() {
  int rs = 0;
  for (int i = 0; i < N; i++)  {
    memset(visited, 0, sizeof(visited));
    if (Search(i)) rs++;
  }
  return rs;
}

void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int k = 0; k < c; k++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    N = n;
    memset(matrix, 0, sizeof(matrix));
    memset(link, -1, sizeof(link));
    int m = 0;
    scanf("%d", &m);  // 输入一定要注意  getchar 要不要用呢，开始多了一个getchar() 就一直WA
    getchar();
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      getchar();
      u--;
      v--;
      matrix[u][v] = 1;
    }
    int t = MaxMatch();
    printf("%d\n", N - t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
