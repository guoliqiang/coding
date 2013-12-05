// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-22 23:11:50
// File  : code.cc
// Brief : http://blog.csdn.net/lwbaptx/article/details/7297840

/*
 * floyd + 状态压缩　
 *
 * 一个图的最小生成树即这个图上所有点（设集合为G）生成的边权值和最小的树，我们可
 * 以知道这个树上任意两点是可到达的，这个可以用prime或者kruskal实现。下面，假如我
 * 们只要求G的一个真子集里面的所有点连通，那么我们发现一些边权是没有用的、可以去掉，
 * 这样，将指定点集合中的所有点连通，且总边权值和最小的生成树称谓MinimalSteinerTree
 * （最小斯坦纳树），可以看出，最小生成树是最小斯坦纳树的一种特殊情况。 (最小树形图是有向图的MST)
 * 
 * 所謂的Steiner tree problem是指在一无向图G(V,E)中, 给定一组V的子集合S, 我们要在其中找
 * 到一个 minimum cost tree, 这个tree 必需包含S中所有的点, 另外也可包含一些非S中的点。
 * 这些非S的点我们称之为 Steiner nodes, S中的点我们称之为 terminals。
 * 
 * 时间复杂度 O(V^3+V*2^A*(2^A+V))
 * poj3123
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

#define INF 0x3f3f3f3f
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = { {0} };
int visited[MAX] = {0};
const int MaxS = 8;
int S = 0;

int dis[MAX][MAX] = { {0} };
int dp[1 << MaxS][MAX] = { {0} };
// dp[i][j]表示点j到联通的集合i的最短距离 (i为状态压缩后表示的点集)
int id[MAX] = {0};

int Steiner() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) dis[i][j] = 0;
      else if (matrix[i][j] > 0) {
        dis[i][j] = matrix[i][j];
      } else {
        dis[i][j] = INF;
      }
    }
  }

  int top = 1 << S;
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0;j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) {
          dis[i][j] = dis[i][k] + dis[k][j];
        }
      }
    }
  }  // floyd

  for (int i = 0; i < top; i++) {
    for (int j = 0; j < N; j++) dp[i][j] = INF;
  }
  for (int i = 0; i < S; i++) {
    for (int j = 0; j < N; j++) {
      dp[1 << i][j] = dis[id[i]][j];
    }
  }
  // dp[i][j] 的值由两种情况产生
  // １，j直接和ｉ集合相连
  // ２，ｊ通过另一个点ｋ直接和ｉ集合相连
  for (int i = 0; i < top; i++) {
     if ((i & (i - 1)) == 0) continue;  // ｉ集合只含有一个点
     memset(visited, 0, sizeof(visited));
     for (int k = 0; k < N; k++) {  // init
       for (int j = 0; j < i; j++) {
         if ((i | j) == i && dp[i][k] > dp[j][k] + dp[i - j][k]) {
           dp[i][k] = dp[j][k] + dp[i - j][k];
           // LOG(INFO) << "dp[" << i << "][" << k << "]:" << dp[i][k];
         }
       }
     }
     // 因为c有可能和i集合不联通
     // 修正dp[i][k]
     for (int j = 0; j < N; j++) {  // update
       // 每次选择最小值更新
       int c = 0;
       int min = INF;
       for (int k = 0; k < N; k++) {
         if (dp[i][k] <= min && visited[k] == 0) {
           min = dp[i][k];
           c = k;
         }
       }
       visited[c] = 1;
       for (int k = 0; k < N; k++) {
         if (dp[i][c] > dp[i][k] + dis[k][c]) {
           dp[i][c] = dp[i][k] + dis[k][c];
         }
       }
     }
  }

  int rs = INF;
  for (int i = 0; i < N; i++) {
    rs = std::min(rs, dp[top - 1][i]);
  }
  return rs;
}

void Read(std::vector<std::vector<int> > & v, std::vector<int> & ids) {
  N = v.size();
  S = ids.size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
  for (int i = 0; i < S; i++) {
    id[i] = ids[i];
  }
}

}  // namespace algorithm

using namespace algorithm;  // NOLINT


int main(int argc, char** argv) {
  int n = 4;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = v[1][0] = 10;
  v[0][2] = v[2][0] = 10;
  v[1][2] = v[2][1] = 10;
  v[3][0] = v[0][3] = 2;
  v[3][1] = v[1][3] = 2;
  v[3][2] = v[2][3] = 2;
  
  std::vector<int> ids;
  ids.push_back(0);
  ids.push_back(1);
  ids.push_back(2);
  Read(v, ids);
  LOG(INFO) << Steiner();
  return 0;
}
