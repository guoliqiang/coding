// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 15:00:06
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace wrong {

void SetZero(std::vector<std::vector<int> > & v, int b, int e) {
  LOG(INFO) << "b:" << b << " e: " << e;
  if (b > e) return;
  if (b == e) {
    int i = 0;
    for (i = 0; i < v[b].size(); i++) {
      if (v[b][i] == 0) break;
    }
    if (i != v[b].size()) {
      for (i = 0; i < v[b].size(); i++) v[b][i] = 0;
    }
  } else {
    int mid = b + (e - b) / 2;
    SetZero(v, b, mid);
    SetZero(v, mid + 1, e);
    if (e - b + 1 > 2) {
      for (int i = 0; i < v[mid].size(); i++) {
        int tb = 0;
        int te = 0;
        if (v[mid][i] == 0 && v[mid + 1][i] != 0) {
          tb = mid + 1;
          te = e;
        }
        if (v[mid][i] != 0 && v[mid + 1][i] == 0) {
          tb = b;
          te = mid;
        }
        for (int j = tb; j <= te; j++) {
          v[j][i] = 0;
        }
      }
    }
  }
}

void SetZero(std::vector<std::vector<int> > & v) {
  SetZero(v, 0, v.size() - 1);
}

}  // namespace wrong

/*
 *
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 376 milli secs
 * Progress: 154/154 test cases passed.
 *
 * the first row used to mark if the coloum is needed to set zero?
 * the first coloum used to mark if the row is needed to set zer0?
 *
 * */

namespace algorithm {
void SetZero(std::vector<std::vector<int> > & v) {
  int m = v.size();
  int n = m > 0 ? v[0].size() : 0;
  int row0 = -1;
  int col0 = -1;

  for (int i = 0; i < v.size(); i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] != 0) continue;
      if (i == 0) row0 = 0;
      if (j == 0) col0 = 0;
      if (i == 0 || j == 0) continue;
      v[i][0] = 0;
      v[0][j] = 0;
    }
  }
  for (int i = 1; i < m; i++) {
    if (v[i][0] == 0) {
      for (int j = 1; j < n; j++) v[i][j] = 0;
    }
  }
  for (int i = 1; i < n; i++) {
    if (v[0][i] == 0) {
      for (int j = 1; j < m; j++) v[j][i] = 0;
    }
  }
  if (row0 == 0) {
    for (int i = 0; i < n; i++) v[0][i] = 0;
  }
  if (col0 == 0) {
    for (int i = 0; i < m; i++) v[i][0] = 0;
  }
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<int> > matrix(3, std::vector<int>(5, 1));
  // matrix[2][3] = 0;
  matrix[0][2] = 0;
  matrix[1][2] = 0;

  LOG(INFO) << JoinMatrix(&matrix);
  SetZero(matrix);
  LOG(INFO) << JoinMatrix(&matrix);
  return 0;
}
