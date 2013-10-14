// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 11:25:35
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 21/21 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 10/10 test cases passed.
 *
 * */

#include <vector>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {

/*
 * 一行或一列处理n - 1个，循环好写
 * 注意处理边界情况
 *
 * n  <= 0 与  n =1
 * */

void SpiralOrder(std::vector<std::vector<int> > & v, int b, int n,
                 std::vector<int> * rs) {
  if (n <= 0) return;
  if (n == 1) {
    rs->push_back(v[b][b]);
    return;
  }
  int size = n - 1;
  for (int i = 0; i < n - 1; i++) rs->push_back(v[b][b + i]);
  for (int i = 0; i < n - 1; i++) rs->push_back(v[b + i][b + size]);
  for (int i = 0; i < n - 1; i++) rs->push_back(v[b + size][b + size - i]);
  for (int i = 0; i < n - 1; i++) rs->push_back(v[b + size - i][b]);
}

void SpiralOrder(std::vector<std::vector<int> > & v, int b, int m, int n,
                 std::vector<int> * rs) {
  if (n == 0 || m == 0) return;  // bug used much time to fix.
  if (m == 1) {
    for (int i = 0; i< n; i++) rs->push_back(v[b][b + i]);
    return;
  }
  if (n == 1) {
    for (int i = 0; i< m; i++) rs->push_back(v[b + i][b]);
    return;
  }

  int msize = m - 1;
  int nsize = n - 1;

  for (int i = 0; i < n - 1; i++) rs->push_back(v[b][b + i]);
  for (int i = 0; i < m - 1; i++) rs->push_back(v[b + i][b + nsize]);
  for (int i = 0; i < n - 1; i++) rs->push_back(v[b + msize][b + nsize - i]);
  for (int i = 0; i < m - 1; i++) rs->push_back(v[b + msize - i][b]);
}

std::vector<int>  SpiralOrder(std::vector<std::vector<int> > & v) {
  std::vector<int> rs;
  if (v.size() == 0) return rs;
  int m = v.size();
  int n = v[0].size();
  int k = std::min(m, n);
  for (int i = 0; i <= k / 2; i++) {
    SpiralOrder(v, i, m, n, &rs);
    n -= 2;
    m -= 2;
  }
  return rs;
}

void Out(std::vector<std::vector<int> > & v) {
  for (int i = 0; i< v.size(); i++) {
    for (int j = 0; j < v[i].size(); j++) {
      std::cout << v[i][j] << "";
    }   
    std::cout << std::endl;
  }
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
using namespace std;    
    vector<int> SpiralOrder(vector<vector<int> > &matrix) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        std::vector<int> rs;
        if (matrix.size() == 0) return rs;
        int bx = 0;
        int ex = matrix.size() - 1;
        int by = 0;
        int ey = matrix[0].size() - 1;
        while (bx <= ex && by <= ey) {
            for (int i = by; i < ey; i++) rs.push_back(matrix[bx][i]);
            for (int i = bx; i < ex; i++) rs.push_back(matrix[i][ey]);
            if (bx == ex) {
                rs.push_back(matrix[ex][ey]);
                break;
            }
            if (by == ey) {
                rs.push_back(matrix[ex][by]);
                break;
            }  // 只有一行或一列时特殊处理
            for (int i = ey; i > by; i--) rs.push_back(matrix[ex][i]);
            for (int i = ex; i > bx; i--) rs.push_back(matrix[i][by]);
            bx++;
            ex--;
            by++;
            ey--;
        }
        return rs;
    }
}  // namespace twice

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<int> foo;
  int n = 2;
  for (int i = 0 ; i < n; i++) {
    foo.push_back(i);
  }
  for (int i = 0; i < 1; i++) {
    matrix.push_back(foo);
  }
  /*
  matrix.clear();
  foo.clear();
  foo.push_back(1);
  matrix.push_back(foo);

  foo.clear();
  foo.push_back(2);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(3);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(4);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(5);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(6);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(7);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(8);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(9);
  matrix.push_back(foo);
  
  foo.clear();
  foo.push_back(10);
  matrix.push_back(foo);
  Out(matrix);
  */
  std::vector<int> rs = twice::SpiralOrder(matrix);
  LOG(INFO) << JoinVector(rs);
  return 0;
}
