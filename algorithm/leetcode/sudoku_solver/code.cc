// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 17:51:07
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 780 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 1144 milli secs
 * Progress: 6/6 test cases passed.
 *
 * 可以进行剪枝，编码难度略大
 *
 * */

#include <vector>
#include "base/public/logging.h"
namespace algorithm {

std::pair<int, int> GridIndex(int n, int k) {
  int i = 3 * (n / 3);
  int j = 3 * (n % 3);
  i += k / 3;
  j += k % 3;
  return std::make_pair(i, j);
}

bool Check(std::vector<std::vector<char> > & v, int i, int j) {
  std::vector<int> row(9, 0);
  std::vector<int> col(9, 0);
  std::vector<int> grid(9, 0);
  int grid_id = (i / 3) * 3 + (j / 3);
  
  for (int k = 0; k < 9; k++) {
    char r_c = v[i][k];
    char c_c = v[k][j];
    std::pair<int, int> idx = GridIndex(grid_id, k);
    char g_c = v[idx.first][idx.second];
    
    if (r_c != '.') {
      int r_v = r_c - '1';
      if (row[r_v]) return false;
      else row[r_v] = 1;
    }
    
    if (c_c != '.') {
      int c_v = c_c - '1';
      if (col[c_v]) return false;
      else col[c_v] = 1;
    }
    
    if (g_c != '.') {
      int g_v = g_c - '1';
      if(grid[g_v]) return false;
      else grid[g_v] = 1;
    }
  }
  return true;
}

bool Next(std::vector<std::vector<char> > & v, int *row, int * col) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if(v[i][j] == '.') {
        *row = i;
        *col = j;
        return true;
      }
    }
  }
  return false;
}

bool SudokuSolver(std::vector<std::vector<char> > & v) {
  int now_i = 0;
  int now_j = 0;
  if (!Next(v, &now_i, &now_j)) return true;
  // LOG(INFO) << "now i:" << now_i << " j:" << now_j;
  for (int i = 0; i < 9; i++) {
    v[now_i][now_j] = '1' + i;
    if (Check(v, now_i, now_j)) {
      if (SudokuSolver(v)) return true;
    }
  }
  v[now_i][now_j] = '.';  // Note here
  return false;
}

}  //  namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  /*
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j ++) {
      std::pair<int, int> rs = GridIndex(i, j);
      LOG(INFO) << rs.first << " " << rs.second;
    }
  }
  */

  std::vector<std::vector<char> > board;

  std::vector<char> row1(9, '.');
  row1[0] = '1'; row1[1] = '2';  row1[2] = '.';  row1[3] = '4';  row1[4] = '5';  row1[5] = '6';  row1[6] = '7';  row1[7] = '8';  row1[8] = '.';
  board.push_back(row1);

  std::vector<char> row2(9, '.');
  board.push_back(row2);
  
  std::vector<char> row3(9, '.');
  board.push_back(row3);
  
  std::vector<char> row4(9, '.');
  board.push_back(row4);
  
  std::vector<char> row5(9, '.');
  board.push_back(row5);
  
  std::vector<char> row6(9, '.');
  board.push_back(row6);
  
  std::vector<char> row7(9, '.');
  board.push_back(row7);
  
  std::vector<char> row8(9, '.');
  board.push_back(row8);
  
  std::vector<char> row9(9, '.');
  board.push_back(row9);
  LOG(INFO) << SudokuSolver(board);
  return 0;
}
