// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 10:21:13
// File  : code.cc
// Brief :
// http://hawstein.com/posts/19.2.html

/*
 * Design an algorithm to figure out if someone has won in a game of tic-tac-toe.
 *
 * */

#include "base/public/common_head.h"

namespace NB {
// 对于井字游戏，每个格子可以是空，我的棋子和对方的棋子3种可能，总共有3^9 = 19683 种可能状态。我们可
// 以把每一种状态转换成一个整数， 预处理时把这个状态下是否有人赢得了井字游戏保存起来，每次检索时就
// 只需要O(1)时间。 比如每个格子上的3种状态为0(空)，1(我方棋子)，2(对方棋子)，棋盘从左到右， 从上到
// 下依次记为0到8，那么任何一个状态都可以写成一个3进制的数，再转成10进制即可。 比如，下面的状态：
//
// 1 2 2
// 2 1 1
// 2 0 1
// 可以写成:
// 122211201=1*3^8 + 2*3^7 +...+ 0*3^1 + 1*3^0
// 如果只需要返回是否有人赢，而不需要知道是我方还是对方。 那可以用一个二进制位来表示是否有人赢。
// 比如上面的状态，1赢， 就可以把那个状态转换成的

// 我想到的办法：如果是每走一步就需要调用一次这个函数
// 那完全可以增量式的判断，即仅仅判断当前放棋子的位置
// 有米有可能造成win即可
//
// 上面的方法适合任何时候想调用时的快速方法,需要一个hashmap辅助

}  // namespace NB

namespace algorithm {

// 想复杂了，这个游戏是在3 * 3的九宫格上玩的
// 这样写代码就简单了许多，尤其是在判断对角线的
// 情况下。
//
// begin with row = 0; col = -1
bool Next(std::vector<std::vector<int> > & v, int & row, int & col, int k) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  for (int i = col + 1; i < n; i++)  {
    if (v[row][i] == k) {
      col = i;
      return true;
    }
  }
  for (int i = row + 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (v[i][j] == k) {
        row = i;
        col = j;
        return true;
      }
    }
  }
  return false;
}

bool Check(std::vector<std::vector<int> > & v, int i, int j) {
  int m = v.size();
  int n = (m == 0) ? 0 : v[0].size();
  if (i - 1 >= 0 && i + 1 < m && v[i][j] == v[i - 1][j] && v[i][j] == v[i + 1][j]) return true;
  if (i - 2 >= 0 && v[i][j] == v[i - 1][j] && v[i][j] == v[i - 2][j]) return true;
  if (i + 2 < m && v[i][j] == v[i + 1][j] && v[i][j] == v[i + 2][j]) return true;

  if (j - 1 >= 0 && j + 1 < n && v[i][j] == v[i][j - 1] && v[i][j] == v[i][j + 1]) return true;
  if (j - 2 >= 0 && v[i][j] == v[i][j - 1] && v[i][j] == v[i][j - 2]) return true;
  if (j + 2 < n && v[i][j] == v[i][j + 1] && v[i][j] == v[i][j + 2]) return true;

  if (i - 2 >= 0 && j - 2 >= 0) {
    if(v[i][j] == v[i - 1][j - 1] && v[i][j] == v[i - 2][j - 2]) return true;
  }

  if (i + 2 < m && j + 2 < n) {
    if(v[i][j] == v[i + 1][j + 1] && v[i][j] == v[i + 2][j + 2]) return true;
  }

  if (i - 1 >= 0 && j - 1 >=0 && i + 1 < m && j + 1 < n) {
    if(v[i][j] == v[i - 1][j - 1] && v[i][j] == v[i + 1][j + 1]) return true;
  }

  if (i + 2 < m && j - 2 >= 0) {
    if(v[i][j] == v[i + 1][j - 1] && v[i][j] == v[i + 2][j - 2]) return true;
  }

  if (i - 2 >= 0 && j + 2 < n) {
    if(v[i][j] == v[i - 1][j + 1] && v[i][j] == v[i - 2][j + 2]) return true;
  }

  if (i - 1 >= 0 && j + 1 < n && i + 1 < m && j - 1 >= 0) {
    if(v[i][j] == v[i - 1][j + 1] && v[i][j] == v[i + 1][j - 1]) return true;
  }
  return false;
}

bool Win(std::vector<std::vector<int> > & v, int k) {
  int i = 0;
  int j = -1;
  while (Next(v, i, j, k)) {
    if (Check(v, i, j)) return true;
  }
  return false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
