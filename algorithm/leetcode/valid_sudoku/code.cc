// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-24 17:00:53
// File  : code.cc
// Brief :

#include <vector>
#include <iostream>
#include "base/public/logging.h"

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 34/34 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 72 milli secs
 * Progress: 500/500 test cases passed.
 *
 *
 * 这样valid的数独并不一定有解，如test中的数独
 * 就是没有解的 
 *
 * */

namespace algorithm {
bool IsValidSudoku(std::vector<std::vector<char> > & v) {
  std::vector<std::vector<bool> > row(9, std::vector<bool>(9, false));
  std::vector<std::vector<bool> > col(9, std::vector<bool>(9, false));
  std::vector<std::vector<bool> > grid(9, std::vector<bool>(9, false));
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if(v[i][j] == '.') continue;
      int foo = v[i][j] - '1';
      int bar = (i / 3) * 3 + (j / 3);
      if (row[i][foo] || col[j][foo] || grid[bar][foo]) {
        LOG(INFO) << i << " "<< j << " "<< foo << " " << bar;
        return false;
      }
      row[i][foo] = true;
      col[j][foo] = true;
      grid[bar][foo] = true;
    }
  }
  return true;
}

}  // namespace algorithm

namespace other {

using namespace std;

bool isValidSudoku(vector<vector<char> > &board) {
  vector<vector<bool> > rows(9, vector<bool>(9, false));
  vector<vector<bool> > cols(9, vector<bool>(9, false));
  vector<vector<bool> > blocks(9, vector<bool>(9, false));

  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (board[i][j] == '.') continue;
      int c = board[i][j] - '1';
      if (rows[i][c] || cols[j][c] || blocks[i - i % 3 + j / 3][c])
        return false;
      rows[i][c] = cols[j][c] = blocks[i - i % 3 + j / 3][c] = true;
    }
  }
  return true;
}
void Out(std::vector<std::vector<char> > & board) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      // LOG(INFO) << "i :" << i << " j:" << j << " (i - i % 3 + j / 3):" << i - i % 3 + j / 3;
      // LOG(INFO) << (i / 3) * 3 + j/3;
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
}  // namepsace other

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<char> > board;
  std::vector<char> row1(9, '.');
  row1[0] = '1'; row1[1] = '2';  row1[2] = '.';  row1[3] = '4';  row1[4] = '5';  row1[5] = '6';  row1[6] = '7';  row1[7] = '8';  row1[8] = '.';
  board.push_back(row1);

  std::vector<char> row2(9, '.');
  row2[2] = '3';
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
  row9[8] = '3';
  board.push_back(row9);

  LOG(INFO) << other::isValidSudoku(board);
  LOG(INFO) << IsValidSudoku(board);
  other::Out(board);
  return 0;
}
