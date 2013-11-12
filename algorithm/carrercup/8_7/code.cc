// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 17:38:20
// File  : code.cc
// Brief :

/*
 * Given an infinite number of quarters (25 cents), dimes (10 cents), nickels (5 cents) and pennies (1 cent), 
 * write code to calculate the number of ways of representing n cents.
 * */

/*
 * dp[i][j]的定义与０－１背包问题不同
 * dp[i][j]:前ｊ个中价值正好等于ｉ
 * */

#include "base/public/common_head.h"

namespace NB {
// 有序选择
// 选完10后，不能选比10大的，只能选比10小的
// http://hawstein.com/posts/8.7.html
void Trace(int n, std::vector<int> & cent, std::vector<int> & path, int k) {
  if (n == 0) {
    std::map<int, int> tmap;
    for (int i = 0; i < path.size(); i++) {
      if (tmap.count(path[i])) tmap[path[i]]++;
      else tmap[path[i]] = 1;
    }
    LOG(INFO) << JoinKeysValues(&tmap);
  } else {
    for (int i = k; i >= 0; i--) {
      if (n >= cent[i]) {
        path.push_back(cent[i]);
        Trace(n - cent[i], cent, path, i);
        path.pop_back();
      }
    }
  }
}

void Trace(int n, std::vector<int> & cent) {
  std::sort(cent.begin(), cent.end());
  std::vector<int> path;
  Trace(n, cent, path, cent.size() - 1);
}

}  // namespace NB

namespace algorithm {

void Trace(int n, std::vector<int> & cent, std::vector<int> & path, std::set<std::vector<int> > & rs) {
  if (n == 0) {
    std::vector<int> foo = path;
    std::sort(foo.begin(), foo.end());
    rs.insert(foo);
  } else {
    for (int i = 0; i < cent.size(); i++) {
      if (n >= cent[i]) {
        path.push_back(cent[i]);
        Trace(n - cent[i], cent, path, rs);
        path.pop_back();
      }
    }
  }
}

void Trace(int n, std::vector<int> & cent) {
  std::vector<int> path;
  std::set<std::vector<int> > rs;
  Trace(n, cent, path, rs);
  
  for (std::set<std::vector<int> >::iterator i = rs.begin(); i != rs.end(); i++) {
    std::map<int, int> tmp;
    for (int j = 0; j < i->size(); j++) {
      if (tmp.count((*i)[j]))
        tmp[(*i)[j]]++;
      else
        tmp[(*i)[j]] = 1;
    }
    LOG(INFO) << JoinKeysValues(&tmp);
  }

}

struct Node {
  void Push(std::vector<int> & a) {
    v.push_back(a);
  }
  std::vector<std::vector<int> > v;
};

void DP(std::vector<int> & cent, std::vector<std::vector<Node> > & dp) {
  int m = dp.size();
  int n = cent.size();

  for (int i = 0 ; i < n; i ++)  {
    for (int j = 0; j < m; j++) {
      if (i != 0) dp[j][i] = dp[j][i - 1];
      if (j >= cent[i]) {
        for (int k = 0; k < j / cent[i]; k++) {
          std::vector<int> tmp;
          for (int l = 0; l <= k; l++) tmp.push_back(cent[i]);
          int sum = cent[i] * (k + 1);
          if (sum == j) {
            dp[j][i].Push(tmp);
            // LOG(INFO) << "k:" << k << " dp[" << j << "][" << i << "]:" <<JoinMatrix(&(dp[j][i].v));
          }
          if (sum < j && i - 1 >= 0 && dp[j - sum][i - 1].v.size() > 0) {
            std::vector<int> foo = tmp;
            for (int l = 0; l < dp[j - sum][i - 1].v.size(); l++) {
              foo.insert(foo.end(), dp[j - sum][i - 1].v[l].begin(), dp[j - sum][i - 1].v[l].end());
              // LOG(INFO) << "dp[" << j << "][" << i << "]:" << JoinVector(foo);
              dp[j][i].Push(foo);
            }
          }
        }  // for
      }  // if
    }  // for
  }  // for
}

void DP(std::vector<int> & cent, int k) {
  int n = cent.size();
  int m = k + 1;
  std::vector<std::vector<Node> > dp(m, std::vector<Node>(n, Node()));
  DP(cent, dp);
  std::vector<std::vector<int> > & tmp = dp[m - 1][n - 1].v;
  for (int i = 0; i < tmp.size(); i++) {
    std::map<int, int> tmap;
    for (int j = 0; j < tmp[i].size(); j++) {
      if (tmap.count(tmp[i][j])) tmap[tmp[i][j]]++;
      else tmap[tmp[i][j]] = 1;
    }
    LOG(INFO) << JoinKeysValues(&tmap);
  }

}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> cent;
  cent.push_back(1);
  cent.push_back(5);
  cent.push_back(10);
  // cent.push_back(25);
  NB::Trace(30, cent);
  HERE(INFO);
  Trace(30, cent);
  HERE(INFO);
  DP(cent, 30);
  return 0;
}
