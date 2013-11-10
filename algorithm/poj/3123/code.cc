// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-09 23:01:01
// File  : code.cc
// Brief :

/*
 * Accepted 260K  63MS
 * 斯坦纳树
 * floyd+状态dp
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int INF = 0x3f3f3f3f;
int N = 0;
const int MAXN = 32;
const int MAXM = 1010;

int matrix[MAXN][MAXN];
int dis[MAXN][MAXN];
int dp[256][MAXN];
int visited[MAXN];
int id[8];
std::map<std::string, int> v_id;
std::map<int, std::string> id_v;

int Stainer() {
  int n = N;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == j) dis[i][j] = 0;
      else if (matrix[i][j] > 0) dis[i][j] = matrix[i][j];
      else dis[i][j] = INF;
    }
  }
  
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
        }
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < n; j++) {
      dp[1 << i][j] = dis[j][id[i]];
    }
  }
  
  for (int i = 0; i < 256; i++) {
    if ((i & (i - 1)) == 0) continue;
    for (int j = 0; j < n; j++) {
      dp[i][j] = INF;
      for (int k = i; k; k = (k - 1) & i) {
        dp[i][j] = std::min(dp[i][j], dp[k][j] + dp[i - k][j]);
      }
    }

    memset(visited, 0, sizeof(visited));
    for(int j = 0; j < n; j++) {
      int min = INF;
      int idx = -1;
      for (int k = 0; k < n; k++) {
        if (dp[i][k] <= min && visited[k] == 0) {
          min = dp[i][k];
          idx = k;
        }
      }
      visited[idx] = 1;
      for (int k = 0; k < n; k++) {
        dp[i][k] = std::min(dp[i][k], dp[i][idx] + dis[idx][k]);
      }
    }
  }

  int rs = INF;
  
  // 最多可分成4组，分别用00,01,10,11表示
  for (int i = 0; i < 256; i++) {
    int t = 0;
    for (int j = 0; j < 4; j++) {
      int y = 0;
      int x = 0;
      for (int k = 0; k < 8; k+= 2) {
        if (((i >> k) & 3) == j) {
          y+= (3 << k);
          x = id[k];
        }
      }
      if (y != 0) t += dp[y][x];
    }
    rs = std::min(rs, t);
  }
  return rs;
}


void Read() {
  int n;
  int m;
  char s1[30];
  char s2[30];
  while (scanf("%d%d", &n, &m) != EOF && n) {
    getchar();
    N = n;
    for (int i = 0; i < n; i++) {
      scanf("%s", s1);
      getchar();
      v_id[std::string(s1)] = i;
      id_v[i] = std::string(s1);
    }
    memset(matrix, 0, sizeof(matrix));
    for (int i = 0; i < m; i++) {
      int c = 0;
      scanf("%s%s%d", s1, s2, &c);
      getchar();
      int id1 = v_id[std::string(s1)];
      int id2 = v_id[std::string(s2)];
      if (matrix[id1][id2] != 0 && c >= matrix[id1][id2]) continue;
      matrix[id1][id2] = matrix[id2][id1] = c;
    }
    for (int i = 0; i < 4; i++) {
      scanf("%s%s", s1, s2);
      getchar();
      id[i * 2] = v_id[std::string(s1)];
      id[i * 2 + 1] = v_id[std::string(s2)];
    }
    printf("%d\n", Stainer());
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
