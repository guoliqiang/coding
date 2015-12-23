// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-19 22:54:40
// File  : code.cc
// Brief :

// 9ms  2020kb

// 如果构思出来的dp比较复杂，一定要思考是不是可以分解成多次dp
#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 210;
char data[MAXN];
char word[6][MAXN];
int w = 0;
int k = 0;
int dp1[MAXN][MAXN];
int dp2[MAXN][MAXN];

bool Match(const char * l, const char * s) {
  while (*s != '\0') {
    if (*s != *l) return false;
    s++;
    l++;
  }
  return true;
}

// 如果词比较多的话可以用AC自动机优化
void Dp1() {
  memset(dp1, 0, sizeof(dp1));
  int n = strlen(data);

  for (int i = 0; i < w; i++) {
    int m = strlen(word[i]);
    for (int j = 0; j < n - m + 1; j++) {
      if (Match(&(data[j]), word[i])) {
        for (int b = 0; b <= j; b++) {
          for (int e = j + m - 1; e < n; e++) dp1[b][e]++;
        }
      }
    }
  }
}

int Dp2() {
  memset(dp2, 0, sizeof(dp2));
  int n = strlen(data);

  for (int i = 0; i < k; i++) {
    for (int j = i; j < n; j++) {
      if (i == 0) dp2[i][j] = dp1[0][j];
      else {
        for (int l = i - 1; l < j; l++) {
          dp2[i][j] = std::max(dp2[i][j], dp2[i - 1][l] + dp1[l + 1][j]);
        }
      }
    }
  }
  return dp2[k - 1][n - 1];
}



void Read() {
  int ca = 0;
  scanf("%d", &ca);
  getchar();
  for (int l = 0; l < ca; l++) {
    w = 0;
    k = 0;
    int p = 0;
    scanf("%d%d", &p, &k);
    getchar();
    memset(data, 0, sizeof(data));
    memset(word, 0, sizeof(word));
    for (int i = 0; i < p; i++) {
      scanf("%s", &(data[i * 20]));
      getchar();
    }
    scanf("%d", &p);
    getchar();
    for (int i = 0; i < p; i++) {
      scanf("%s", word[w]);
      getchar();
      int n = strlen(word[w]);
      int j = 0;
      for (j = 0; j < w; j++) {
        int m = strlen(word[j]);
        if (m >= n && Match(word[j], word[w])) {
          strcpy(word[j], word[w]);
          break;
        } else if (n >= m && Match(word[w], word[j])) break;
      }
      if (j == w) w++;
    }
    Dp1();
    int t = Dp2();
    printf("%d\n", t);
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
