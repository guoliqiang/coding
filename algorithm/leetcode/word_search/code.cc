// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-27 18:22:17
// File  : code.cc
// Brief :

/*
Given a 2D board and a word, find if the word exists in the grid.
The word can be constructed from letters of sequentially adjacent cell, where "adjacent" cells
are those horizontally or vertically neighboring. The same letter cell may not be used more than once.

For example,
Given board =
[
  ["ABCE"],
  ["SFCS"],
  ["ADEE"]
]
word = "ABCCED", -> returns true,
word = "SEE", -> returns true,
word = "ABCB", -> returns false.

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 30/30 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 60 milli secs
 * Progress: 76/76 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

bool ExistNew(std::vector<std::vector<char> > & v, std::string & word, int i, int j, int k) {
  if(v[i][j] != word[k]) return false;
  if (k >= word.size() - 1) return true;

  char ch = v[i][j];
  bool rs = false;
  v[i][j] = '#';
  // up
  if (i != 0 && v[i - 1][j] != '#')
    rs = ExistNew(v, word, i - 1, j, k + 1);
  // down
  if (!rs && i != v.size() - 1 && v[i + 1][j] != '#')
    rs = ExistNew(v, word, i + 1, j, k + 1);
  // left
  if (!rs && j != 0 && v[i][j - 1] != '#')
    rs = ExistNew(v, word, i, j - 1, k + 1);
  // right
  if (!rs && j != v[0].size() - 1 && v[i][j + 1] != '#')
    rs = ExistNew(v, word, i, j + 1, k + 1);

  v[i][j] = ch;
  return rs;
}

bool ExistNew(std::vector<std::vector<char> > & board,
              std::string & word) {
  int m = board.size();
  int n = (m == 0) ? 0 : board[0].size();
  if (m == 0 || n == 0 || word.size() == 0) return false;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (ExistNew(board, word, i, j, 0)) return true;
    }
  }
  return false;
}

// 这种后判断的写法，递归次数多，也会超时
bool Exist(std::vector<std::vector<char> > & v,
           std::string & word,
           int i, int j, int k,
           std::vector<std::vector<int> > & used) {
  if (k == word.size()) return true;
  else {
    if (v[i][j] != word[k]) return false;
    used[i][j] = 1;
    if (k == word.size() - 1) return true;
    if (!used[i - 1][j] && Exist(v, word, i - 1, j, k + 1, used)) return true;
    if (!used[i + 1][j] && Exist(v, word, i + 1, j, k + 1, used)) return true;
    if (!used[i][j - 1] && Exist(v, word, i, j - 1, k + 1, used)) return true;
    if (!used[i][j + 1] && Exist(v, word, i, j + 1, k + 1, used)) return true;
    used[i][j] = 0;
    return false;
  }
}

/*
 * 下面的写法由于拷贝数据次数太多，大数据会超时。
 *
 * */

bool NeedReverse(std::vector<std::vector<char> > & board, char first, char last) {
  if (first == last) return false;
  int count_first = 0;
  int count_last = 0;
  int m = board.size();
  int n = (m == 0) ? 0 : board[0].size();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == first) count_first++;
      if (board[i][j] == last) count_last++;
    }
  }
  if (count_last < count_first) return true;
  return false;
}

bool Exist(std::vector<std::vector<char> > & board,
           std::string & word) {
  int m = board.size();
  int n = (m == 0) ? 0 : board[0].size();
  if (m == 0 || n == 0 || word.size() == 0) return false;
  if (word.size() >= 2) {
    if (NeedReverse(board, word[0], word[word.size() - 1])) {
      std::string foo(word.rbegin(), word.rend());
      word = foo;
    }
  }
  std::vector<std::vector<char> > v;
  v.push_back(std::vector<char>(n + 2, ' '));
  for (int i = 0; i < m; i++) {
    std::vector<char> foo;
    foo.push_back(' ');
    foo.insert(foo.end(), board[i].begin(), board[i].end());
    foo.push_back(' ');
    v.push_back(foo);
  }
  v.push_back(std::vector<char>(n + 2, ' '));
  
  std::vector<std::vector<int> > used(m + 2, std::vector<int>(n + 2, 0));
  for (int i = 0; i < n + 2; i++) {
    used[0][i] = 1;
    used[m + 1][i] = 1;
  }
  for (int i = 0; i < m + 2; i++) {
    used[i][0] = 1;
    used[i][n + 1] = 1;
  }
  
  for (int i = 1; i < m + 1; i++) {
    for (int j = 1; j < n + 1; j++) {
      std::vector<std::vector<int> > tused = used;
      if (Exist(v, word, i, j, 0, tused)) return true;
    }
  }
  return false;
}

}  // namesapce algorithm

namespace third {
bool ExistT(std::vector<std::vector<char> > & v, std::string & word, int i, int j, int k) {
  if (k == word.size() - 1) return true;

  char ch = v[i][j];
  bool rs = false;
  v[i][j] = '#';
  // up
  if (i != 0 && v[i - 1][j] != '#' && v[i - 1][j] == word[k + 1])
    rs = ExistT(v, word, i - 1, j, k + 1); 
  // down
  if (!rs && i != v.size() - 1 && v[i + 1][j] != '#' && v[i + 1][j] == word[k + 1])
    rs = ExistT(v, word, i + 1, j, k + 1); 
  // left
  if (!rs && j != 0 && v[i][j - 1] != '#' && v[i][j - 1] == word[k + 1])
    rs = ExistT(v, word, i, j - 1, k + 1); 
  // right
  if (!rs && j != v[0].size() - 1 && v[i][j + 1] != '#' && v[i][j + 1] == word[k + 1])
    rs = ExistT(v, word, i, j + 1, k + 1); 

  v[i][j] = ch; 
  return rs; 
}
bool ExistT(std::vector<std::vector<char> > & board,
              std::string & word) {
  int m = board.size();
  int n = (m == 0) ? 0 : board[0].size();
  if (m == 0 || n == 0 || word.size() == 0) return false;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (board[i][j] == word[0] && ExistT(board, word, i, j, 0)) return true;
    }
  }
  return false;
}
}  // namespace third

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<std::vector<char> > v(3, std::vector<char>(4, ' '));
  v[0][0] = 'A'; v[0][1] = 'B'; v[0][2] = 'C'; v[0][3] = 'E';
  v[1][0] = 'S'; v[1][1] = 'F'; v[1][2] = 'C'; v[1][3] = 'S';
  v[2][0] = 'A'; v[2][1] = 'D'; v[2][2] = 'E'; v[2][3] = 'E';
  std::string word;
  word = "ABCCED";
  LOG(INFO) << Exist(v, word);
  LOG(INFO) << ExistNew(v, word);

  word = "SEE";
  LOG(INFO) << Exist(v, word);
  LOG(INFO) << ExistNew(v, word);
  word = "ABCB";
  LOG(INFO) << Exist(v, word);
  LOG(INFO) << ExistNew(v, word);
  return 0;
}
