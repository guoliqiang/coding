// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 16:40:57
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int SumNei(std::vector<std::vector<int> > & board, int i, int j) {
  int m = board.size();
  int n = board[0].size();

  int ans = 0;
  if (i - 1 >= 0) {
    ans += (0x1 & board[i - 1][j]);
    if (j + 1 < n) ans += (0x1 & board[i - 1][j + 1]);
    if (j - 1 >= 0) ans += (0x1 & board[i - 1][j - 1]);
  }
  if (i + 1 < m) {
    ans += (0x1 & board[i + 1][j]);
    if (j + 1 < n) ans += (0x1 & board[i + 1][j + 1]);
    if (j - 1 >= 0) ans += (0x1 & board[i + 1][j - 1]);
  }
  if (j - 1 >= 0) ans += (0x1 & board[i][j - 1]);
  if (j + 1 < n) ans += (0x1 & board[i][j + 1]);
  return ans;
}

void GameOfLife(std::vector<std::vector<int> > & board) {
  int m = board.size();
  if (m == 0) return;
  int n = board[0].size();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      int sum = SumNei(board, i, j);
      if (board[i][j] & 0x1) {
        if (sum == 2 || sum == 3) {
          board[i][j] |= 0x2;
        }
      } else {
        if (sum == 3) {
          board[i][j] |= 0x2;
        }
      }
    }
  }

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      board[i][j] = (board[i][j] >> 1);
    }
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
