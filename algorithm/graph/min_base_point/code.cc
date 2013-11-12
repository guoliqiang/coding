// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-25 04:45:47
// File  : code.cc
// Brief :
// 实际问题：http://blog.csdn.net/tiaotiaoyly/article/details/3281677
//
// 1. 找出所有的极大强连通分支
// 2. 把所有的极大强连通分支想像成一个点（缩点）
// 3. 入度为0的极大强连通分支数，即为最小点基数，（因为从入度为0的极大强连通分支中任意取一点，其可以
//    把前代关系传到其有关系的连通分支中）
//
// poj 1236
//
// 一个非强连通有向图，转换成强连通图最少需要加 max(入度为0的点数, 出度为0的点数) 条边

/*
 * 点基B满足：
 *     对于任意一个顶点Vj，一定存在B中的一个Vi,使得Vi是Vj的前代(不一定是直接前代)。
 *     点基包含：顶点数最小的点基 权最小的点基(当所有的权为1时，权最小的点基变成了顶点数的最小的点基)
 * */

#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int cnt = 0;
int dfn[MAX];
int low[MAX];
int id[MAX];
int tag = 0;
int stack[MAX];
int stop = 0;

int valid[MAX];
int base_point[MAX];


void Tarjan(int k) {
  dfn[k] = low[k] = cnt++;
  stack[stop++] = k;
  int min = N;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfn[i] == -1) Tarjan(i);
      if (low[i] < min) min = low[i];
    }
  }
  if (min < low[k]) {
    low[k] = min;
  } else {
    LOG(INFO) << "component " << tag << " :";
    stop--;
    while (stack[stop] != k) {
      id[stack[stop]] = tag;
      LOG(INFO) << stack[stop];
      low[stack[stop]] = N;
      stop--;
    }
    LOG(INFO) << k;
    id[k] = tag;
    low[k] = N;
    tag++;
  }
}

int Find() {
  memset(stack, 0xff, sizeof(stack));
  memset(dfn, 0xff, sizeof(dfn));
  memset(low, 0xff, sizeof(low));
  memset(id, 0xff, sizeof(id));
  stop = 0;
  tag = 0;
  for (int i = 0; i < N; i++) {
    if (dfn[i] == -1) {
      LOG(INFO) << i;
      Tarjan(i);
    }
  }
  return tag;
}

int BasePoint() {
  memset(valid, 0, sizeof(valid));
  Find();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] > 0 && id[i] != id[j]) valid[id[j]] = -1;
    }
  }
  int rs = 0;
  for (int i = 0; i < N; i++) {
    if (valid[id[i]] != -1) {
      base_point[rs++] = i;
      valid[id[i]] = -1;
    }
  }
  return rs;
}

void Read(std::vector<std::vector<int> > & v) {
  N = v.size();
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      matrix[i][j] = v[i][j];
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 6;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  v[0][1] = 1;
  v[1][2] = 1;
  v[2][0] = 1;

  // v[2][3] = 1;
  v[3][4] = 1;
  v[4][5] = 1;
  // v[5][3] = 1;
  Read(v);
  int k = BasePoint();
  LOG(INFO) << "base points:";
  for (int i = 0; i < k; i++) LOG(INFO) << base_point[i];
  return 0;
}
