// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 17:15:54
// File  : code.cc
// Brief :

/*
A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).

How many possible unique paths are there?


Above is a 3 x 7 grid. How many possible unique paths are there?

Not  m and n will be at most 100.
*/

#include "base/public/common_head.h"

namespace algorithm {
class BitSet {
 public:
  BitSet(int len) : len_(len) {
    ptr_ = new char[len_ / 8 + 1];
    memset(ptr_, 0, sizeof(char) * (len_ / 8 + 1));
  }

  bool IsSet(int i) {
    char * p = ptr_ + (i / 8) * 8;
    char foo = (1 << (i % 8));
    return (foo & (*p));
  }

  void Set(int i) {
    char * p = ptr_ + (i / 8) * 8;
    char foo = (1 << (i % 8));
    (*p) |= foo;
  }

  void Clear(int i) {
    char * p = ptr_ + (i / 8) * 8;
    char foo = ~(1 << (i % 8));
    (*p) &= foo;
  }

  ~BitSet() {
    delete [] ptr_;
  }

 private:
  int len_;
  char * ptr_;
 
 private:
  BitSet(const BitSet &) {}
  void operator = (const BitSet &) {}
};

void UniquePath(std::vector<BitSet *> & v, int i, int j, int m, int n, int &num) {
  if (i == m - 2 && j == n - 2) {
    num++;
    return;
  }

  if (!v[i]->IsSet(j + 1)) {
    v[i]->Set(j + 1);
    UniquePath(v, i, j + 1, m, n, num);
    v[i]->Clear(j + 1);
  }

  if (!v[i + 1]->IsSet(j)) {
    v[i + 1]->Set(j);
    UniquePath(v, i + 1, j, m, n, num);
    v[i + 1]->Clear(j);
  }
}

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 13/13 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 61/61 test cases passed.
 *
 * DP is much faster than recursive but with more memory.
 *
 * recursive will be T.
 *
 * */

void DP(std::vector<std::vector<int> > & dp, int m, int n) {
  for (int i = 0; i < n; i++) dp[0][i] = 1;
  for (int j = 0; j < m; j++) dp[j][0] = 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    }
  }
}

int UniquePathDP(int m, int n) {
  std::vector<std::vector<int> > dp(m, std::vector<int>(n, 0));
  DP(dp, m, n);
  return dp[m - 1][n - 1];
}

int UniquePath(int m, int n) {
  if (m <= 0 || n <= 0) return 0;
  int tm = m + 2;
  int tn = n + 2;
  std::vector<BitSet *> v;
  for (int i = 0; i < tm; i++) {
    v.push_back(new BitSet(tn));
  }
  for (int i = 0; i < tm; i++) {
    if (i == 0 || i == tm - 1) {
      for (int j = 0; j < tn; j++) v[i]->Set(j);
    } else {
      v[i]->Set(0);
      v[i]->Set(tn - 1);
    }
  }
  int num = 0;
  UniquePath(v, 1, 1, tm, tn, num);
  for (int i = 0; i < v.size(); i++) {
    delete v[i];
  }
  return num;
}

}  // namespace algorithm

namespace twice {

int DP(int m, int n) {
  if (m == 0 || n == 0) return 0;
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) dp[0][i] = 1;
  bool flag = false;
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j == 0) dp[!flag][j] = 1;
      else dp[!flag][j] = dp[flag][j] + dp[!flag][j - 1];
    }
    flag = !flag;
  }
  return dp[flag][n - 1];
}

}  // namespace twice

using namespace algorithm;

namespace NB {
int UniquePath(int m, int n) {
  double ans = 1;
  for (int i = 1; i < m; i++) {
      ans =  ans * (n + i - 1) / (i);
  }
  return ans;
}
}  // namespace NB


int main(int argc, char** argv) {
  /*
  m = 4;
  n = 3;
  LOG(INFO) << m << " " << n << " : " << UniquePath(m, n);
  
  m = 2;
  n = 2;
  LOG(INFO) << m << " " << n << " : " << UniquePath(m, n);
  */
  int m = 10;
  int n = 10;
  LOG(INFO) << m << " " << n << " : " << NB::UniquePath(m, n);
  LOG(INFO) << m << " " << n << " : " << UniquePathDP(m, n);
 
  // test for bitset
  /* 
  BitSet * ptr = new BitSet(10);
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << ptr->IsSet(i);
  }
  ptr->Set(1);
  ptr->Set(5);
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << ptr->IsSet(i);
  }

  for (int i = 0; i < 10; i++) {
    ptr->Clear(i);
  }

  for (int i = 0; i < 10; i++) {
    LOG(INFO) << ptr->IsSet(i);
  }

  for (int i = 0; i < 10; i++) {
    ptr->Set(i);
  }

  for (int i = 0; i < 10; i++) {
    LOG(INFO) << ptr->IsSet(i);
  }*/
  return 0;
}
