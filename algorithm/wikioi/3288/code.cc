// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-20 16:03:19
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100005;
int N;
int h[MAXN];

// TLE
int dp[MAXN];
int dp2[MAXN];
int Dp() {
  int rs = 0;
  while (true) {
    memset(dp, 0, sizeof(dp));
    memset(dp2, 0, sizeof(dp2));
    int max = 0;
    dp[0] = h[0] == 0 ? 0 : 1;
    dp2[0] = h[0];
    for (int i = 0; i < N; i++) {
      if (h[i] == 0) {
        dp[i] = 0;
        dp2[i] = 0;
      } else {
        dp[i] = dp[i - 1] + 1;
        dp2[i] = dp[i] == 1 ? h[i] : std::min(h[i], dp2[i - 1]);
      }
      if (dp[i] > dp[max]) max = i;
    }
    if (h[max] == 0) break;
    rs += dp2[max];
    for (int i = 0; i < dp[max]; i++) {
      h[max - i] -= dp2[max];
    }
  }
  return rs;
}

// AC
struct Node {
  Node () {}
  Node (int i, int l, int m) : index(i), len(l), min(m){}
  int index;
  int len;
  int min;
};

struct Cmp {
 bool operator () (const Node & x, const Node & y) const {
   return x.len < y.len;
 }
};

int Solve() {
  int rs = 0;
  std::priority_queue<Node, std::vector<Node>, Cmp> queue;
  int len = 0;
  int min = INF;
  for (int i = 0; i < N; i++) {
    if (h[i] == 0) {
      if (len != 0) queue.push(Node(i - 1, len, min));
      len = 0;
      min = INF;
    } else {
      len += 1;
      min = std::min(min, h[i]);
    }
  }
  if (len != 0) queue.push(Node(N - 1, len, min));
  while (!queue.empty()) {
    Node t = queue.top();
    queue.pop();
    rs += t.min;
    len = 0;
    min = INF;
    for (int i = t.index - t.len + 1; i <= t.index; i++) {
      if (i < 0) return 0;
      h[i] -= t.min;
      if (h[i] == 0) {
        if (len != 0) {
          queue.push(Node(i - 1, len, min));
        }
        len = 0;
        min = INF;
      } else {
        len += 1;
        min = std::min(min, h[i]);
      }
    }
    if (len != 0) queue.push(Node(t.index, len, min));
  }
  return rs;
}

void Read() {
  while (scanf("%d", &N) != EOF) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      h[i] = t;
    }
    getchar();
    int t = Solve();
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
