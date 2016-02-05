// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:47:53
// File  : code.cc
// Brief :
/*
You are given an n x n 2D matrix representing an image.
Rotate the image by 90 degrees (clockwise).
Follow up:
Could you do this in-place?

*/

/*
 *Run Status: Accepted!
 Program Runtime: 8 milli secs
 Progress: 3/3 test cases passed.
 Run Status: Accepted!
 Program Runtime: 28 milli secs
 Progress: 20/20 test cases passed.
 *
 * */

#include <vector>
#include <iostream>
#include "base/public/common_ojhead.h"


namespace algorithm {

void RotateSub(std::vector<std::vector<int> > & v, int idx, int n) {
  LOG(INFO) << "n:" << n;
  if (n == 1) return;
  int size = n - 1;
  for (int i = 0; i < n - 1; i++) {  // Note from 0 to n-2
    // LOG(INFO) << "n:" << n << " i :" << i << " idx:" << idx;
    int foo = v[idx + i][idx];
    v[idx + i][idx] = v[idx + size][idx + i];
    v[idx + size][idx + i] = v[idx + size - i][idx + size];
    v[idx + size - i][idx + size] = v[idx][idx + size - i];
    v[idx][idx + size - i] = foo;
  }
}

void Rotate(std::vector<std::vector<int> > & v) {
  int n = v.size();
  for (int i = 0; i < v.size() / 2; i++) {
     RotateSub(v, i, n);
     n -= 2;
  }
}

void Out(std::vector<std::vector<int> > & v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < v.size(); j++) {
      std::cout << v[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

}  // namesapce algorithm


// http://hawstein.com/posts/1.6.html

/*
 * 顺时针旋转90度方法：1. 交换主对角线元素
 *                     2. 交换第j列与第n-1-j列的元素
 * 
 * 逆时针旋转90度方法：1. 交换主对角线元素
 *                     2. 交换第i行与第n-1-i行的元素
 *
 * */

namespace NB {
using namespace std;
// clockwise
void Rotate(vector<vector<int> > &matrix) {
  int n = matrix.size();
  for (int i = 0; i < n/2; ++i) {
    for (int j = i; j < n-1-i; ++j){
      int t = matrix[i][j];
      matrix[i][j] = matrix[n-1-j][i];
      matrix[n-1-j][i] = matrix[n-1-i][n-1-j];
      matrix[n-1-i][n-1-j] = matrix[j][n-1-i];
      matrix[j][n-1-i] = t;
    }
  }
}

// unclockwise
void RotateR(std::vector<std::vector<int> > & v) {
  for (int i = 0; i < v.size(); i++) {
    for (int j = i + 1; j < v.size(); j++) {
      std::swap(v[i][j], v[j][i]);
    }
  }
  for (int i = 0; i < v.size() / 2; i++) {
    for (int j = 0; j < v.size(); j++) {
      std::swap(v[i][j], v[v.size() - 1 - i][j]);
    }
  }
}

}  // namespce NB

namespace NB2 {
using namespace std;

/*
 * clockwise rotate
 * first reverse up to down, then swap the symmetry 
 * 1 2 3     7 8 9     7 4 1
 * 4 5 6  => 4 5 6  => 8 5 2
 * 7 8 9     1 2 3     9 6 3
*/
void rotate(vector<vector<int> > &matrix) {
    std::reverse(matrix.begin(), matrix.end());
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = i + 1; j < matrix[i].size(); ++j)
            swap(matrix[i][j], matrix[j][i]);
    }
}

/*
 * anticlockwise rotate
 * first reverse left to right, then swap the symmetry
 * 1 2 3     3 2 1     3 6 9
 * 4 5 6  => 6 5 4  => 2 5 8
 * 7 8 9     9 8 7     1 4 7
*/
void anti_rotate(vector<vector<int> > &matrix) {
    for (int i = 0; i < matrix.size(); i++) reverse(matrix[i].begin(), matrix[i].end());
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = i + 1; j < matrix[i].size(); ++j)
            swap(matrix[i][j], matrix[j][i]);
    }
}
}  // namespace NB2

using namespace algorithm;

namespace clear {
void rotate(std::vector<std::vector<int> > & matrix) {
  int n = matrix.size();

  for (int i = 0; i < n; i++) {
      for (int j = 0; j < i; j++) {
          std::swap(matrix[i][j], matrix[j][i]);
      }
  }
  for (int i = 0; i < n; i++) {
      std::reverse(matrix[i].begin(), matrix[i].end());
  }
}
}  // namespace clear


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix;
  std::vector<int> foo;
  int n = 6;
  for (int i = 0; i < n; i++) {
    foo.push_back(i);
  }
  for (int i = 0; i < n; i++) {
    matrix.push_back(foo);
  }
  Out(matrix);
  HERE(INFO);
  NB::RotateR(matrix);
  Out(matrix);
  return 0;
}
