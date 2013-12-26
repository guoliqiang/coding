// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-22 17:41:59
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 5004;
int dp[MAXN][MAXN];
int dp_nb1[MAXN][MAXN];
int dp_nb2[MAXN][MAXN];
int flag[MAXN][MAXN];

int num;
std::string str1;
std::string str2;

struct Node {
  Node() {}
  Node(int xi, int yi, int li) : x(xi), y(yi), l(li){}
  int x;
  int y;
  int l;
} store[MAXN * 2], print[MAXN * 2];
int s_top = 0;
int p_top = 0;

std::vector<char> path;
void Trace1(int x, int y) {
  path.push_back(str1[x]);
  if (dp[x][y] == 1) {
    LOG(INFO) << JoinVector(path);
    num = (num + 1) % 100000000;
  } else {
    for (int i = 1; i < x; i++) {
      for (int j = 1; j < y; j++) {
        if (str1[i] == str2[j] && dp[i][j] + 1 == dp[x][y]) Trace1(i, j);
      }
    }
  }
  path.pop_back();
}
void Trace1() {
  num = 0;
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str1[i] == str2[j] && dp[i][j] == dp[m - 1][n - 1]) {
        Trace1(i, j);
      }
    }
  }
}

int DpNB() {
  memset(dp_nb1, 0, sizeof(dp_nb1));
  memset(dp_nb2, 0, sizeof(dp_nb2));
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str2[j] == str1[i]) {
        dp_nb1[i][j] = dp_nb1[i - 1][j - 1] + 1;
        dp_nb2[i][j] = dp_nb2[i - 1][j - 1];
        if (dp_nb1[i - 1][j] > dp_nb1[i][j]) {
          dp_nb1[i][j] = dp_nb1[i - 1][j];
          dp_nb2[i][j] = dp_nb2[i - 1][j];
        } else if (dp_nb1[i - 1][j] == dp_nb1[i][j]) {
          dp_nb2[i][j] += dp_nb2[i - 1][j];
        } else if (dp_nb1[i][j - 1] > dp_nb1[i][j]) {
          dp_nb1[i][j] = dp_nb1[i][j - 1];
          dp_nb2[i][j] = dp_nb2[i][j - 1];
        } else if (dp_nb1[i][j - 1] == dp_nb1[i][j]) {
          dp_nb2[i][j] += dp_nb2[i][j - 1];
        }
      } else {
        dp_nb1[i][j] = dp_nb1[i - 1][j];
        dp_nb2[i][j] = dp_nb2[i - 1][j];

        if (dp_nb1[i][j - 1] > dp_nb1[i][j]) {
          dp_nb1[i][j] = dp_nb1[i][j - 1];
          dp_nb2[i][j] = dp_nb2[i][j - 1];
        } else if (dp_nb1[i][j - 1] == dp_nb1[i][j]) {
          dp_nb1[i][j] = dp_nb1[i][j - 1];
          dp_nb2[i][j] += dp_nb2[i][j - 1];
          if (dp_nb1[i - 1][j - 1] == dp_nb1[i][j]) dp_nb2[i][j] -= dp_nb2[i - 1][j - 1];
        }
      }
    }
  }
  num = dp_nb2[m - 1][n - 1];
  return dp_nb1[m - 1][n - 1];
}


int Dp() {
  memset(dp, 0, sizeof(dp));
  memset(flag, 0, sizeof(flag));
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  for (int i = 1; i < m; i++) {
    for (int j = 1; j < n; j++) {
      if (str2[j] == str1[i]) {
        dp[i][j] = dp[i - 1][j - 1] + 1;
        flag[i][j] = 1;
      } else {
        if (dp[i - 1][j] > dp[i][j - 1]) {
          dp[i][j] = dp[i - 1][j];
          flag[i][j] = 2;
        } else if (dp[i][j - 1] > dp[i - 1][j]) {
          dp[i][j] = dp[i][j - 1];
          flag[i][j] = 3;
        } else {
          dp[i][j] = dp[i - 1][j];
          flag[i][j] = 4;
        }
      }
      // LOG(INFO) << "dp[" << i << "][" << j << "]:" << dp[i][j] << " flag[" << i << "][" << j << "]:" << flag[i][j];
    }
  }
  return dp[m - 1][n - 1];
}

void Print() {
  num++;
  for (int i = p_top - 1; i > 0; i--) {
    printf("%c", str1[print[i].x]);
  }
  printf("\n");
}

void Search(int curx, int cury, int & rsx, int &rsy, int type) {
  // LOG(INFO) << "curx:" << curx << " cury:" << cury << " flag[" << curx << "][" << cury << "]:" << flag[curx][cury];
  if (curx < 0) exit(1);
  if (flag[curx][cury] == 1) {
    rsx = curx;
    rsy = cury;
  } else if (flag[curx][cury] == 2) {
    Search(curx - 1, cury, rsx, rsy, type);
  } else if (flag[curx][cury] == 3) {
    Search(curx, cury - 1, rsx, rsy, type);
  } else {
    if (type == 0) Search(curx - 1, cury, rsx, rsy, type);
    else Search(curx, cury - 1, rsx, rsy, type);
  }
}

void Trace() {
  s_top = 0;
  p_top = 0;
  num = 0;
  int m = str1.size() - 1;
  int n = str2.size() - 1;
  store[s_top++] = Node(m, n, dp[m - 1][n - 1] + 1);
  while (s_top > 0) {
    Node t = store[--s_top];
    if (t.x == 1 || t.y == 1 || dp[t.x][t.y] == 1) {
      print[p_top++] = t;
      Print();
      Node foo = store[s_top - 1];
      while (p_top > 0 && print[p_top - 1].l <= foo.l) p_top--;
    } else {
      print[p_top++] = t;
      int ex1, ey1, ex2, ey2;
      Search(t.x - 1, t.y - 1, ex1, ey1, 0);
      Search(t.x - 1, t.y - 1, ex2, ey2, 1);
      if (ex1 == ex2 && ey1 == ey2) {
        store[s_top++] = Node(ex1, ey1, dp[ex1][ey1]);
      } else {
        for (int i = ex1; i<= ex2; i++) {
          for (int j = ey2; j <= ey1; j++) {
            if (flag[i][j] == 1) {
              store[s_top++] = Node(i, j, dp[i][j]);
            }
          }
        }
      }
    }
  }
}

void Read() {
  char s1[MAXN];
  char s2[MAXN];
  while (scanf("%s", s1) != EOF) {
    LOG(INFO) << s1;
    getchar();
    scanf("%s", s2);
    getchar();
    LOG(INFO) << s2;
    str1 = " " + std::string(s1);
    str2 = " " + std::string(s2);
    int t = DpNB();
    // Trace1();
    // Trace();
    printf("%d\n%d\n", t, num);
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
