// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 18:45:49
// File  : code.cc
// Brief :

/*
 * 树状数组，以log(n)的复杂度维护一个A[1] ~ A[k]的查询与修改
 * 额外空间为n，如果使用完全二叉树维护的话，额外空间的消耗为2*n
 * http://www.java3z.com/cwbwebhome/article/article1/1369.html?id=4804
 *
 * 注意数组下边要从1开始
 * */

#include "base/public/common_head.h"

// 一维树状数组
namespace algorithm {

int const MAX = 1000;
int N = 0;
int data[MAX] = {0};
int c[MAX] = {0};

// k表示成二进制形式后，其后0的个数的2次密
int Low(int k) {
  int t = k & (k ^ (k - 1));
  return t;
}

int GetSum(int k) {
  if (k > N) return -1;
  int rs = 0;
  while (k > 0) {
    rs += c[k];
    k -= Low(k);
  }
  return rs;
}

void Add(int k, int v) {
  data[k] += v;
  while (k <= N) {
    c[k] += v;
    k += Low(k);
  }
}

void Read(std::vector<int> & v) {
  N = v.size();
  for (int i = 0; i < N; i++) {
    data[i + 1] = v[i];
  }
  // c[i]表示data[i - 2^k + 1]到data[i]的和，而k则是i在二进制时末尾0的个数
  // 下标从1开始
  for (int i = 1; i <= N; i++) {
    int t = 0;
    for (int j = i - Low(j) + 1; j <= i; j++) {
      t += data[j];
    }
    c[i] = t;
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(5);
  v.push_back(2);
  v.push_back(0);
  v.push_back(4);
  Read(v);
  LOG(INFO) << GetSum(3);
  Add(2, -3);
  LOG(INFO) << GetSum(3);
  return 0;
}
