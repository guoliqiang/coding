// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-09 20:30:30
// File  : code.cc
// Brief :

/*
 * Given an NxN matrix of positive and negative integers,
 * write code to find the submatrix with the largest possible sum. 
 * */

#include "base/public/common_head.h"

namespace algorithm {

int MaxSub(std::vector<int> & v) {
  std::vector<int> dp(v.size(), 0);
  dp[0] = v[0];
  int max = dp[0];
  for (int i = 1; i < v.size(); i++) {
    dp[i] = std::max(v[i], v[i] + dp[i - 1]);
    max = std::max(dp[i], max);
  }
  return max;
}

int MaxMatrix(std::vector<std::vector<int> > & v) {
  int max = 0;
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  for (int i = 0; i < m; i++) {
    for (int j = i; j < m; j++) {
      std::vector<int> tmp;
      for (int k = 0; k < n; k++) {
        int t = 0;
        for (int l = i; l <= j; l++) t+= v[l][k];
        tmp.push_back(t);
      }
      max = std::max(max, MaxSub(tmp));
    }
  }
  return max;
}

// TODO: guoliqiang
// 用树形数组计算A[0][0]~A[i][j]的值 log(n), 但是没有下面的方法快

}  // namespace algorithm

namespace NB {
// sum[i][j] = sum[i-1][j] + sum[i][j-1] - sum[i-1][j-1] + v[i][j]
// sum[i][j]:表示左上角是(0, 0)， 右下角是(i, j)的矩阵中元素的和
// sum(D) = p[x2][y2] - p[x1 - 1][y2] - p[x2][y2 - 1] + p[x1 - 1][y1 - 1]

void Util(std::vector<std::vector<int> > & matrix, std::vector<std::vector<int> > & v) {
  int m = matrix.size();
  int n = (m == 0) ? 0 : matrix[0].size();
  v.resize(m + 1, std::vector<int>(n + 1, 0));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      v[i + 1][j + 1] = matrix[i][j];
    }
  }
}

void MakeSum(std::vector<std::vector<int> > & v,
             std::vector<std::vector<int> > & sum) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  sum.resize(m, std::vector<int>(n, 0));
  for (int i = 0; i < m; i++) sum[i][0] = 0;
  for (int i = 0; i < n; i++) sum[0][i] = 0;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] + v[i][j];
    }
  }
}



int MaxMattrix(std::vector<std::vector<int> > & matrix) {
  std::vector<std::vector<int> > v;
  Util(matrix, v);
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  std::vector<std::vector<int> > sum;
  MakeSum(v, sum);
  int max = 0;
  for (int i = 1; i < m; i++) {
    for (int j = i; j < m; j++) {
      int t = 0;
      for (int k = 1; k < n; k++) {
        int foo =  sum[j][k] - sum[j][k - 1] - sum[i - 1][k] + sum[i - 1][k - 1];
        if (k == 1) t = foo;
        else t = std::max(t + foo, foo);
        max = std::max(t, max);
      }
    }
  }
  return max;
}

}  //
using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
