// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-25 00:24:05
// File  : code.cc
// Brief : http://wenku.baidu.com/view/07f4be196c175f0e7cd13784.html
// 弦图：所有长度大于3的环均有弦的图叫弦图。
// 单纯点(simplicial vertex)：设N(v)表示与点v相邻的点集。一个点称为单纯点当{v} + N(v)的诱导子图为一个团。
// 完美消除序列(perfect elimination ordering)
//        定义：一个点的序列(每个点出现且恰好出现一次)v1, v2, …, vn满足vi在{vi, vi+1,…,vn}的诱导子图中为一个单纯点。
// 最朴素的算法：
//     每次找一个单纯点v，加入到完美消除序列中。
//     将v以及相关的边从图中删掉。
//     重复以上过程直到所有点都被删除(图为弦图，得到了完美序列)或不存在单纯点v(图不是弦图)。
//
//
//  最大势算法(用O(m + n) 实现朴素算法思想)
//
//  第一步: 给节点编号 mcs(n)
//  设已编号的节点集合为A, 未编号的节点集合为B
//  开始时A为空, B包含所有节点.
//  for num=n-1 downto 0 do {
//  在B中找节点x, 使与x相邻的在A集合中的节点数最多,
//  将x编号为num, 并从B移入A.
//  }
//  第二步: 检查 peo(n)
//  for num=0 to n-1 do {
//  对编号为num的点x, 设所有编号>num且与x相邻的点集为C
//  在C中找出编号最小的节点y
//  若C中存在点z!=y, 使得y与z之间无边, 则此图不是弦图.
//  }
//  检查完了, 则此图是弦图.

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX];
int order[MAX]; // order[i] 顶点i在完美序列中排在第几位
int val[MAX];   // val[i]   完美序列中排在第i位的是哪个节点
int num[MAX];

void MSC() {
  memset(order, 0xff, sizeof(order));
  memset(val, 0xff, sizeof(val));
  memset(num, 0, sizeof(num));
  for (int i = N - 1; i >= 0; i--) {
    int j = 0;
    while (order[j] >= 0) j++;
    for (int k = j + 1; k < N; k++) {
      if (order[k] == -1 && num[k] > num[j]) j = k;
    }
    order[j] = i;
    val[i] = j;
    for (int k = 0; k < N; k++) {
      if (matrix[j][k] > 0) num[k]++;
    }
  }
}

int Check() {
  for (int i = N - 2; i >= 0; i--) {
    int min = N;
    int u = val[i];
    for (int j = i + 1; j < N; j++) {
      int v = val[j];
      if (matrix[u][v] > 0) {
        min = v;
        break;
      }
    }
    if (min == N) continue;
    for (int j = i + 1; j < N; j++) {
      int v = val[j];
      if (v == min || matrix[u][v] == 0) continue;
      if (matrix[min][v] == 0) return 0;
    }
  }
  return 1;
}

void Read(std::vector<std::vector<int> > & v) {
  memset(matrix, 0, sizeof(matrix));
  N = v.size();
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
  v[0][1] = v[1][0] = 1;
  v[0][3] = v[3][0] = 1;
  v[1][2] = v[2][1] = 1;
  v[1][3] = v[3][1] = 1;
  // v[3][2] = v[2][3] = 1;
  // v[1][4] = v[4][1] = 1;
  v[4][2] = v[2][4] = 1;
  v[4][3] = v[3][4] = 1;
  v[4][5] = v[5][4] = 1;
  Read(v);
  MSC();
  LOG(INFO) << Check();
  return 0;
}
