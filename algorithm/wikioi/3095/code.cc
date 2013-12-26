// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-22 11:06:23
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXCN = 30;
class BInt {
 public:
 char v_[MAXCN];
 int l;
 
 BInt(char * str = NULL) {
   l = 0;
   if (str != NULL) {
     int len = strlen(str);
     for (int i = len - 1; i >= 0; i--) {
       v_[l++] = str[i];
     }
   }
   while (l > 0 && v_[l - 1] == '0') l--;
 }

 bool operator < (const BInt & a) const {
   if (l != a.l) return l < a.l;
   for (int i = l - 1; i >= 0; i--) {
     if (v_[i] != a.v_[i]) return v_[i] < a.v_[i];
   }
   return false;
 }

 bool operator <= (const BInt & a) const {
   return !(a < (*this));
 }

 BInt operator + (const BInt & a) {
   BInt rs;
   int i = 0;
   int j = 0;
   int c = 0;
   while (i < l || j < a.l || c > 0) {
     int t = c;
     if (i < l) t += v_[i++] - '0';
     if (j < a.l) t += a.v_[j++] - '0';
     c = t / 10;
     t = t % 10;
     rs.v_[rs.l++] = '0' + t;
   }
   while (rs.l > 0 && rs.v_[rs.l - 1] == '0') rs.l--;
   return rs;
 }

 void Out() {
   for (int i = l - 1; i >= 0; i--) printf("%d", v_[i] - '0');
   printf("\n");
 }
};

const int MAXN = 505;
int N;
struct Node {
  BInt a;
  BInt b;
  BInt c;
} data[MAXN];

BInt dp[MAXN];

bool Cmp(const Node & x, const Node & y) {
  return x.a < y.a;
}

BInt Dp() {
  BInt rs;
  std::sort(data, data + N, Cmp);
  for (int i = 0; i < N; i++) {
    dp[i] = data[i].c;
    for (int j = i - 1; j >= 0; j--) {
      if (data[j].b <= data[i].a) dp[i] = std::max(dp[i], dp[j] + data[i].c);
    }
    rs = std::max(rs, dp[i]);
  }
  return rs;
}

void Read() {
  char str[30];
  while (scanf("%s%d", str, &N) != EOF) {
    getchar();
    char stra[30];
    char strb[30];
    char strc[30];
    for (int i = 0; i < N; i++) {
      scanf("%s%s%s", stra, strb, strc);
      getchar();
      data[i].a = BInt(stra);
      data[i].b = BInt(strb);
      data[i].c = BInt(strc);
    }
    BInt t = Dp();
    t.Out();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
