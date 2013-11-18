// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-25 22:39:38
// File  : code.cc
// Brief : http://blog.sina.com.cn/s/blog_71ded6bf0100tn3d.html
//         http://blog.sciencenet.cn/home.php?mod=space&uid=434885&do=blog&id=325398
// 2-sat (2 satisfiability) 问题
#include "base/public/common_head.h"

namespace algorithm {

const int MAX = 1000;
int N = 0;
int matrix[MAX][MAX] = {{0}};
int stack[MAX];
int dfn[MAX];
int low[MAX];
int id[MAX];
int tag = 0;
int cnt = 0;
int stop = 0;

int matrix2[MAX][MAX];
int queue[MAX];
int du[MAX];
int tail = 0;
int ans[MAX];
int ans2[MAX];
int other[MAX];

void Tarjan(int k) {
  dfn[k] = low[k] = cnt++;
  stack[stop++] = k;
  int min = N;
  for (int i = 0; i < N; i++) {
    if (matrix[k][i] > 0) {
      if (dfn[i] == -1) Tarjan(i);
      min = std::min(min, low[i]);
    }
  }
  if (low[k] > min) {
    low[k] = min;
  } else {
    stop--;
    while (stack[stop] != k) {
      id[stack[stop]] = tag;
      low[stack[stop]] = N;
      stop--;
    }
    id[k] = tag;
    low[k] = N;
    tag++;
  }
}

int Find() {
  memset(dfn, 0xff, sizeof(dfn));
  memset(low, 0xff, sizeof(low));
  memset(id, 0xff, sizeof(id));
  memset(stack, 0xff, sizeof(stack));
  tag = 0;
  stop = 0;
  cnt = 0;
  for (int i = 0; i < N; i++) {
    if (dfn[i] == -1) Tarjan(i);
  }
  return tag;
}

bool Check() {
  int n = N / 2;
  for (int i = 0; i < n; i++) {
    if (id[i] == id[n + i]) {
      return false;
    }
  }
  return true;
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

// 有问题
void answer() {
  memset(matrx2, 0, sizeof(matrix2));
  memset(du, 0, sizeof(du));
  memset(ans, 0, sizeof(ans));
  memset(ans2, 0, sizeof(ans2));
  memset(other, 0, sizeof(other));
  for (int i = 0; i < N / 2; i++) {
    other[id[i]] = id[N / 2 + i];
    other[id[N / 2 + i]] = id[i];
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (matrix[i][j] <= 0) continue;
      if(id[i] != id[j]) {
        matrix2[id[i]][id[j]] = 1;
        du[id[j]]++;
      }
    }
  }
  // toporder
  for (int i = 0; i < tag; i++) {
    if (du[i] == 0) queue[tail++] = i;
  }
  int k = 0;
  while (k < tail) {
    int t = queue[k++];
    if (ans2[t] == 0) {
      ans2[t] = 1;
      ans2[other[t]] = -1; // 删除点，因为是按拓扑排序遍历的，省去了删除前驱的步骤
    }
    for (int j = 0; j < tag; j++) {
      if (matrix2[t][j] > 0) {
        du[j]--;
        if (du[j] == 0) queue[tail++] = j;
      }
    }
  }

  for (int i = 0; i < N; i++) {
    if (ans2[id[i]] == 1) ans[i] = 1;
  }
}


// TODO 输出一个可行的结果
// 1.缩点
// 2.拓扑排序
// 3.从拓扑排序的逆序开始遍历
// 4.访问i，删除i'以及i'的前驱，直到队列中没有元素
// 5.访问过的元素即为一个可行的结果

}  // namesapce algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  int n = 3;
  std::vector<std::vector<int> > v(n * 2, std::vector<int>(n * 2, 0));
  v[0][1] = 1;
  v[3][4] = 1;
  // v[4][0] = 1;
  // v[1][3] = 1;
  Read(v);
  Find();
  LOG(INFO) << Check();
  return 0;
}
