// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-17 18:08:31
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1005;  // 这里不可以，不能开题目要求那么大的数组
int N;
int data[MAXN];
int dp[MAXN][MAXN];
LL Dp() {
  memset(dp, 0, sizeof(dp));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N - i; j++) {
      if (i == 0) dp[j][j + i] = data[j];
      else {
        LL min = 0;
        for (int k = j + i; k > i; k--) {
          LL t = dp[j][k - 1] + dp[k][j + i];
          min = min == 0 ? t : std::min(min, t);
        }
        dp[j][j + i] = min;
      }
    }
  }
  return dp[0][N - 1];
}

void Read() {
  while (scanf("%d", &N) != EOF && N) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      data[i] = t;
    }
    if (N == 1) printf("0\n");
    else {
      LL t = Dp();
      printf("%lld\n", t);
    }
  }
}
}  // namespace algorithm

//Accepted  368K  188MS
/*
本题中N 范围特别大，开不了这么大的二维数组。所以
动规算法只能处理小规模的情况。下面介绍第三种解法，Garsia-Wachs 算法。
假设我们只对3 堆石子a,b,c 进行合并, 先合并哪两堆, 能使得得分最小？有两种方案：
score1 = (a+b) + ((a+b)+c)
score2 = (b+c) + ((b+c)+a)
假设score1 <= score2, 可以推导出a <= c，反过来，只要a 和c 的关系确定，合并的顺序也确定。这
就是2-递减性质。
Garsia-Wachs 算法，就是基于这个性质，找出序列中满足A[i-1] <= A[i+1] 最小的i，合并temp
= A[i]+A[i-1]，接着往前面找是否有满足A[j] > temp，把temp 值插入A[j] 的后面(数组的右边)。循
环这个过程一直到只剩下一堆石子结束。
例如,
13 9 5 7 8 6 14
首先，找第一个满足A[i-1] <= A[i+1] 的三个连续点，本例中就是5 7 8，5 和7 合并得到12，12 不是
丢在原来的位ç9 得到21，将21 插入到适当位置，得到21 14 13 14
找到14 13 14，合并14 和13 得到27，将27 插入到适当位置，得到27 21 14
因为27<14，先合并21 和14，得到35，最后合并27 和35，得到62，就是最终答案。
为什么要将temp 插入A[j] 的后面, 可以理解为是为了保持2-递减性质。从A[j+1] 到A[i-2] 看成
一个整体A[mid]，现在A[j], A[mid], temp(A[i-1]+A[i])，因为temp < A[j]，因此不管怎样都是A[mid]
和temp 先合并, 所以将temp 值插入A[j] 的后面不会影响结果。

*/
using namespace algorithm;

namespace NB {  // GarsiaWachs算法
const int MAXN = 50005;
int N;
int num;
int rs;
int data[MAXN];

void Combine(int k) {
  int t = data[k - 1] + data[k];
  rs += t;
  for (int i = k + 1; i < num; i++) {
    data[i - 1] = data[i];
  }
  num--;
  int i = k - 1;
  while (i > 0 && data[i - 1] < t) {
    data[i] = data[i - 1];
    i--;
  }
  data[i] = t;
  while (i - 2 >= 0 && data[i] >= data[i - 2]) {
    int d = num - i;
    Combine(i - 1);
    i = num - d;
  }
}

void Read() {
  while (scanf("%d", &N) != EOF && N) {
    getchar();
    for (int i = 0; i < N; i++) {
      int t = 0;
      scanf("%d", &t);
      getchar();
      data[i] = t;
    }
    rs = 0;
    num = 0;
    for (int i = 0; i < N; i++) {
      data[num++] = data[i];
      while (num >= 3 && data[num - 3] <= data[num - 1]) Combine(num - 2);
    }
    while (num > 1) Combine(num - 1);
    printf("%d\n", rs);
  }
}
}  // namespace NB

int main(int argc, char** argv) {
  FROMFILE;
  NB::Read();
  return 0;
}
