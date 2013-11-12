// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-02 01:04:37
// File  : code.cc
// Brief :

/*
 *  旅行商问题，哈密顿回路(每个点最多经过1此)，状态压缩.
 *  前提是节点数目不能太多，这是状态压缩的前提要求，否则使用分支限界法
 *
 *  如果允许每个点可以经过多次，需要首先使用floyd计算出两个节点的最短距离，然后
 *  基于floyd结果构成的图上使用状态压缩。即虚拟任两个节点间都有连线，其长度为floyd算出
 *  来的结果。
 *
 * */
#include "base/public/common_head.h"

namespace algorithm {

#define INF 0x3f3f3f3f
const int MAX = 10;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int dis[MAX][MAX] = {{0}};
int dp[1 << MAX][MAX];

struct Info{
  int s;
  int i;
  Info(int ts = 0, int ti = 0) : s(ts), i(ti){}
};

int Hamilton() {
  memset(dp, 0, sizeof(dp));
  std::queue<Info> queue;
  for (int i = 0; i < N; i++) {
    if (dis[0][i] < INF) {
      dp[(1 << i) + 1][i] = dis[0][i];
      queue.push(Info((1 << i) + 1, i));
    }
  }
  while (!queue.empty()) {
    Info t = queue.front();
    queue.pop();
    for (int i = 0; i < N; i++) {
      if (t.s & (1 << i) || dis[t.i][i] == INF) continue;
      
      if (dp[t.s + (1 << i)][i] == 0) dp[t.s + (1 << i)][i] = dp[t.s][t.i] + dis[t.i][i];
      else dp[t.s + (1 << i)][i] = std::min(dp[t.s + (1 << i)][i], dp[t.s][t.i] + dis[t.i][i]);
      // LOG(INFO) << "dp[" << (t.s + (1 << i)) <<"][" << i << "]" << dp[t.s + (1 << i)][i];
      if (t.s + (1 << i) != (1 << N) - 1) queue.push(Info(t.s + (1 << i), i));
    }
  }
  int res = INF;
  for (int i = 1; i < N; i++) {
    res = std::min(res, dp[(1 << N) - 1][i] + dis[i][0]);
  }
  return res;
}

void Floyd() {
  memset(dis, 0, sizeof(dis));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) dis[i][j] = 0;
      else {
        dis[i][j] = matrix[i][j] == 0 ? INF : matrix[i][j];
      }
    }
  }
  for (int k = 0; k < N; k++) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (dis[i][j] > dis[i][k] + dis[k][j]) dis[i][j] = dis[i][k] + dis[k][j];
      }
    }
  }
}

void Read(std::vector<std::vector<int> > & v, bool repeate = false) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  memset(dis, 0, sizeof(dis));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
  if (repeate == false) {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i == j) dis[i][j] = 0;
        else {
          dis[i][j] = matrix[i][j] == 0 ? INF : matrix[i][j];
        }
      }
    }
  } else {
    Floyd();
  }

}

void Trace() {
  int road = INF;
  int index = -1;
  for (int i = 1; i < N; i++) {
    if (dp[(1 << N) - 1][i] + dis[i][0] < road) {
      road = dp[(1 << N) - 1][i] + dis[i][0];
      index = i;
    }
  }
  if (index == -1) return;
  road -= dis[index][0];

  LOG(INFO) << "path:";
  LOG(INFO) << index;
  int t = (1 << N) - 1;
  while (index != 0) {
    t = t - (1 << index);
    // LOG(INFO) << "t:" << t << " index:" << index << " road:" << road;
    for (int i = 0; i < N; i++) {
      // LOG(INFO) << "dp[" << t << "][" << i << "]:" << dp[t][i] << " " << "dis[" << i << "][" << index << "]:" << dis[i][index] << " road :" << road;
      if (dp[t][i] + dis[i][index] == road) {
        road -= dis[i][index];
        index = i;
        break;
      }
    }
    LOG(INFO) << index;
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 3;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  /*
  v[0][1] = v[1][0] = 1;
  v[0][2] = v[2][0] = 3;
  v[1][3] = v[3][1] = 2;
  v[2][3] = v[3][2] = 2;
  v[0][3] = v[3][0] = 2;
  v[1][2] = v[2][1] = 1;
  */
  v[0][1] = v[1][0] = 1;
  v[1][2] = v[2][1] = 1;
  v[0][2] = v[2][0] = 100;
  Read(v, true);
  LOG(INFO) << Hamilton();
  // Trace();
  return 0;
}
