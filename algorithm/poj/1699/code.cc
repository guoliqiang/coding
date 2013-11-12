// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-11 19:46:20
// File  : code.cc
// Brief :

/*
 * Accepted 368K  16MS  C++
 *
 *
 * leetcode 中的wordladder
 * 1. 在图中求指定两个点的最短路长度
 * 2. 列出图中指定两个点所有的最短路（因为最短路可能不只一条）
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 11;
const int MAXM = 21;
int N = 0;
std::string str[MAXN];
int matrix[MAXN][MAXN];
int dp[1 << MAXN][MAXN];
int flag[1 << MAXN][MAXN];
int next[MAXM];

void Next(const std::string & pattern) {
  memset(next, 0, sizeof(next));
  int n = pattern.size();
  if (n == 0) return;
  next[0] = -1;
  int i = 0;
  int k = next[i];
  while (i < n - 1) {
    while (k >= 0 && next[k] != pattern[i]) k = next[k];
    k++;
    i++;
    if (pattern[i] == pattern[k]) next[i] = next[k];
    else next[i] = k;
  }
}

int KMP(const std::string & str, const std::string & pattern) {
  Next(pattern);
  int i = 0;
  int j = 0;
  while (j < str.size()) {
    if (str[j] == pattern[i]) {
      i++;
      j++;
    } else {
      i = next[i];
    }
    if (i == pattern.size()) break;
    if (i == -1) {
      i++;
      j++;
    }
  }
  return i;
}

int TSP() {
  memset(dp, -1, sizeof(dp));
  memset(flag, 0, sizeof(flag));
  std::queue<std::pair<int, int> > queue;
  for (int i = 0; i < N; i++) {
    dp[1 << i][i] = str[i].size();
    queue.push(std::make_pair(1 << i, i));
    flag[1 << i][i] = 1;
  }
  while (!queue.empty()) {
    std::pair<int, int> t = queue.front();
    flag[t.first][t.second] = 0;
    queue.pop();
    for (int i = 0; i < N; i++) {
      if (((1 << i) & t.first) == 0) {
        int key = 1 << i | t.first;
        int cost = str[i].size() - matrix[t.second][i];
        // 不能不判断就直接加入队列中，这样虽然逻辑没有问题，但运行时间会
        // 变长, TLE
        if (dp[key][i] == -1) {
          dp[key][i] = dp[t.first][t.second] + cost;
          queue.push(std::make_pair(key, i));
          flag[key][i] = 1;
        } else if (dp[key][i] > dp[t.first][t.second] + cost) {
          dp[key][i] = std::min(dp[key][i], dp[t.first][t.second] + cost);
          if (flag[key][i] == 0) {
            queue.push(std::make_pair(key, i));
            flag[key][i] = 1;
          }
        }
      }
    }
  }
  int rs = INF;
  for (int i = 0; i < N; i++) {
    // LOG(INFO) << "dp[" << (1 << N - 1) << "][" << i << "]:" << dp[(1 << N) - 1][i];
    rs = std::min(rs, dp[(1 << N) - 1][i]);
  }
  return rs;
}


void Read() {
  int num = 0;
  scanf("%d", &num);
  getchar();
  for (int i = 0; i < num; i++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    N = n;
    char temp[25];
    for (int j = 0; j < n; j ++) {
      scanf("%s", temp);
      getchar();
      str[j] = std::string(temp);
    }
    memset(matrix, 0, sizeof(matrix));
    for (int k = 0; k < n; k++) {
      for (int j = 0; j < n; j++) {
        if (k != j) {
          int t = 0;
          int m1 = str[k].size();
          int m2 = str[j].size();
          if (m1 > m2) t = KMP(str[k].substr(m1 - m2), str[j]);
          else t = KMP(str[k], str[j]);
          // LOG(INFO) << str[k] << "~" << str[j] << " : " << t;
          matrix[k][j] = t;
        }
      }
    }
    //
    int t = TSP();
    printf("%d\n", t);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
