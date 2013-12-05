// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-02 23:05:28
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 30;
int N = 0;
int M = 0;
int count[MAXN];
int queue[MAXN];
int matrix[MAXN][MAXN];

int TopoOrder() {
  memset(count, 0, sizeof(count));
  memset(queue, 0, sizeof(queue));
  int tail = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0) {
        count[j]++;
      }
    }
  }
  for (int i = 0; i < N; i++) {
    if (count[i] == 0) queue[tail++] = i;
  }
  bool flag = 0;
  int temp = 0;
  int k = 0;
  while (k < tail) {
    if (tail - temp > 1) flag = 1;
    temp = tail;
    int t = queue[k++];
    for (int i = 0; i < N; i++) {
      if (matrix[t][i] > 0) {
        count[i]--;
        if (count[i] == 0) queue[tail++] = i;
      }
    }
  }
  if (tail != N) return -1;
  else if (flag == 1) return 0;
  else return 1;
}

void Read() {
  int n, m;
  while(scanf("%d%d", &n, &m) != EOF && (n || m)) {
    getchar();
    N = n;
    M = m;
    memset(matrix, 0, sizeof(matrix));
    char a, b;
    bool flag = true;
    for (int j = 0; j < m; j++) {
      scanf("%c<%c", &a, &b);
      getchar();
      matrix[a - 'A'][b - 'A'] = 1;
      if (flag) {
        int t = TopoOrder();
        if (t == -1) {
          printf("Inconsistency found after %d relations.\n", j + 1);
          flag = false;
        } else if (t == 1) {
          printf("Sorted sequence determined after %d relations: ", j + 1);
          for (int k = 0; k < N; k++) printf("%c", queue[k] + 'A');
          printf(".\n");
          flag = false;
        }
      }
    }
    if (flag) printf("Sorted sequence cannot be determined.\n");
  }
}


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
