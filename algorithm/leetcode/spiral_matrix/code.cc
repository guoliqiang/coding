// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 11:25:35
// File  : code.cc
// Brief :

/*
Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.

For example,
Given the following matrix:

[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
You should return [1,2,3,6,9,8,7,4,5].
*/

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

namespace third {
using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int> >& matrix) {
        std::vector<int> ans;
        int n = matrix.size();
        if (n == 0) return ans;
        int m = matrix[0].size();
        int b_i = 0;
        int b_j = 0;
        for (int i = m, j = n; i > 0 && j > 0; i-= 2, j-= 2) {
            if (i == 1) {
                for (int k = 0; k < j; k++) ans.push_back(matrix[b_i][b_j + k]);
            } else if (j == 1) {
                for (int k = 0; k < i; k++) ans.push_back(matrix[b_i + k][b_j]);  
            } else {
                for (int k = 0; k < i - 1; k++) ans.push_back(matrix[b_i][b_j + k]);
                for (int k = 0; k < j - 1; k++) ans.push_back(matrix[b_i + k][b_j + i - 1]);
                for (int k = 0; k < i - 1; k++) ans.push_back(matrix[b_i + j - 1][b_j + i - 1 - k]);
                for (int k = 0; k < j - 1; k++) ans.push_back(matrix[b_i + j - 1 - k][b_j]);
            }
            b_i++;
            b_j++;
        }
        return ans;
    }
};
}  // namespace third

namespace clear {
std::vector<int> SpiralOrder(std::vector<std::vector<int> > & matrix) {
  std::vector<int> ans;
  while (matrix.size() > 0) {
    ans.insert(ans.end(), matrix[0].begin(), matrix[0].end());
    matrix.erase(matrix.begin());

    for (int i = 0; i < matrix.size(); i++) {
        ans.push_back(matrix[i].back());
        matrix[i].pop_back();
    }
    if (matrix.size() && matrix.back().size() == 0) matrix.clear();

    if (matrix.size()) {
        std::vector<int> t = matrix.back();
        std::reverse(t.begin(), t.end());
        ans.insert(ans.end(), t.begin(), t.end());
        matrix.pop_back();
    }

    for (int i = matrix.size() - 1; i >= 0; i--) {
        ans.push_back(*matrix[i].begin());
        matrix[i].erase(matrix[i].begin());
    }
    if (matrix.size() && matrix.back().size() == 0) matrix.clear();
  }
  return ans;
}
}  // namespace clear

int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<int> foo;
  foo.push_back(2);
  foo.push_back(5);
  foo.push_back(8);
  matrix.push_back(foo);
  foo.clear();
  third::Solution s;

  LOG(INFO) << JoinVector(s.spiralOrder(matrix));
  return 0;
}
