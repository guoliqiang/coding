// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-03 21:44:05
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

/*
http://www.careercup.com/question?id=12718665

Count the number of occurences of each letter in the input string [numA, numB, numC] 
If two of these counts are 0, then return string.length 
Else if (all counts are even) or (all counts are odd), then return 2 
Else, then return 1

*/
namespace algorithm {

struct Node {
  int len;
  char ch;
  Node(int l = 0, char c = '0') : len(l), ch(c){}
};

char Other(char x, char y) {
  if ((x == 'a' && y == 'b') || (x == 'b' && y == 'a')) return 'c';
  if ((x == 'a' && y == 'c') || (x == 'c' && y == 'a')) return 'b';
  if ((x == 'c' && y == 'b') || (x == 'b' && y == 'c')) return 'a';
  return '0';
}

int Dp(std::string str) {
  int n = str.size();
  std::vector<std::vector<Node> > dp(n, std::vector<Node>(n, Node()));
  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n - k; i++) {
      if (k == 0) {
        dp[i][i + k].len = 1;
        dp[i][i + k].ch = str[i];
      } else {
        int len = INT_MAX;
        char ch = '0';
        for (int j = i; j < i + k; j++) {
          if (dp[i][j].ch == dp[j + 1][i + k].ch) {
            len = dp[i][j].len + dp[j + 1][i + k].len;
            ch = dp[i][j].ch;
          } else {
            char c = dp[i][j].ch;
            int t = dp[j + 1][i + k].len;
            while (t) {
              c = Other(c, dp[j + 1][i + k].ch);
              t--;
            }
            t = dp[i][j].len;
            char pre = c;
            while (t && c != '0') {
              c = Other(c, dp[i][j].ch);
              t--;
            }
            if (c == '0') c = pre;
            if (t + 1 < len) {
              len = t + 1;
              ch = c;
            }

            c = dp[j + 1][i + k].ch;
            t = dp[i][j].len;
            while (t) {
              c = Other(c, dp[i][j].ch);
              t--;
            }
            t = dp[j + 1][ i + k].len;
            pre = c;
            while (t && c != '0') {
              c = Other(c, dp[j + 1][i + k].ch);
              t--;
            }
            if (c == '0') c = pre;
            if (t + 1 < len) {
              len = t + 1;
              ch = c;
            }
          }
          dp[i][i + k].len = len;
          dp[i][i + k].ch = ch;
        }
      }
    }
  }
  return dp[0][n - 1].len;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << Dp("cccc");
  return 0;
}
