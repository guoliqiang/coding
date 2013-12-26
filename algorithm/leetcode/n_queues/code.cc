// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 10:17:36
// File  : code.cc
// Brief :

/*
The n-queens puzzle is the problem of placing n queens on an n×n chessboard such that no two queens attack each other.



Given an integer n, return all distinct solutions to the n-queens puzzle.

Each solution contains a distinct board configuration of the n-queens' placement, where 'Q' and '.' both indicate a queen and an empty space respectively.

For example,
There exist two distinct solutions to the 4-queens puzzle:

[
 [".Q..",  // Solution 1
  "...Q",
  "Q...",
  "..Q."],

  ["..Q.",  // Solution 2
   "Q...",
   "...Q",
   ".Q.."]
]
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 9/9 test cases passed.
 *
 * */

#include <vector>
#include <algorithm>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

// 八皇后问题的最优实现方式,迭代和check的
// 次数都是最少的
bool Check(std::vector<int> & num, int k) {
  for (int i = 0; i < k; i++) {
    if (num[i] == num[k]) return false;
    if (abs(num[i] - num[k]) == abs(i - k)) return false;
  }
  return true;
}

void Trace(std::vector<int> & num, int n,
           std::vector<std::vector<std::string> > * rs) {
  if (n >= num.size()) {
    std::vector<std::string> foo;
    for (int i = 0; i < num.size(); i++) {
      std::string bar;
      for (int j = 0; j < num.size(); j++) {
        if (j == num[i]) bar.push_back('Q');
        else bar.push_back('.');
      }
      foo.push_back(bar);
    }
    rs->push_back(foo);
  } else {
    for (int i = n; i < num.size(); i++) {
      std::swap(num[n], num[i]);
      if (Check(num, n)) Trace(num, n + 1, rs);
      std::swap(num[n], num[i]);
    }
  }
}

std::vector<std::vector<std::string> > NQueues(int n) {
  std::vector<int> num(n, 0);
  std::vector<std::vector<std::string> > rs;
  for (int i = 0; i < n; i++) {
    num[i] = i;
  }
  Trace(num, 0, &rs);
  return rs;
}

void Out(std::vector<std::vector<std::string> > & v) {
  for (int i = 0; i< v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      std::cout << v[i][j] << std::endl;
    }
    std::cout << std::endl;
  }
}

}  // namespace algorithm

// 非递归实现，fb的一道面试题目
namespace twice {
bool Next(std::vector<int> & v) {
  int i = v.size() - 1;
  while (i > 0 && v[i - 1] > v[i]) i--;
  if (i == 0) return false;
  
  int j = v.size() - 1;
  while (v[j] < v[i - 1]) j--;
  std::swap(v[j], v[i - 1]);
  j = v.size() - 1;
  while (i < j) {
    std::swap(v[i++], v[j--]);
  }
  return true;
}

bool Check(std::vector<int> & v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      if (abs(i - j) == abs(v[i] - v[j])) return false;
    }
  }
  return true;
}

std::vector<std::vector<std::string> > NQueues(int n) {
  std::vector<std::vector<std::string> > rs;
  std::vector<int> v(n, 0);
  for (int i = 0; i < n; i++) v[i] = i + 1;
  while (true) {
    if (Check(v)) {
      std::vector<std::string> foo(n, "");
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (j + 1 == v[i]) foo[i].push_back('Q');
          else foo[i].push_back('.');
        }
      }
      rs.push_back(foo);
    }
    if (Next(v) == false) break;
  }
  return rs;
}

}  // namspace twice

// 回溯版本的非递归实现
namespace third {
bool Check(std::vector<int> & v, int k) {
  for (int i = 0; i < k; i++) {
    if (abs(v[i] - v[k])  == abs(i - k) || v[i] == v[k]) return false;
  }
  return true;
}

std::vector<std::vector<std::string> > NQueues(int n) {
  std::vector<std::vector<std::string> > rs;
  std::vector<int> v(n, 0);
  int k = 0;
  while (k >= 0) {
    v[k]++;
    while(v[k]<= n && Check(v, k) == false) v[k]++;
    if (v[k] <= n) {
      if (k == n - 1) {
        std::vector<std::string> foo(n, "");
        for (int i = 0; i < n; i++) {
          for (int j = 0; j < n; j++) {
            if (v[i] == j + 1) foo[i].push_back('Q');
            else foo[i].push_back('.');
          }
        }
        rs.push_back(foo);
      } else {
        k++;
        v[k] = 0;
      }
    } else {
      k--;
    }
  }
  return rs;
}
}  // namespace third

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<std::string> > rs = third::NQueues(8);
  Out(rs);
  // LOG(INFO) << JoinMatrix(&rs);
  LOG(INFO) << rs.size();
  return 0;
}

