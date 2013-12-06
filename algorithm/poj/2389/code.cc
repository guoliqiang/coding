// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-06 15:18:52
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

/*
2389  Accepted  172K  16MS
*/
namespace algorithm {
const int MAXN = 100;
int base = 10;  // base = 1000 && num = 3 can not ac ??
int num = 1;

struct BInt {
 public:
  int v[MAXN];
  int ln;
  
  BInt() : ln(0){}
  
  BInt(const std::string & str) {
    ln = 0;
    if (str.size() == 0) return;
    int rs = 0;
    int count = 1;
    for (int i = str.size() - 1; i >= 0; i--) {
      rs = rs * 10 + str[i] - '0';
      if (count == num) {
        v[ln++] = rs;
        rs = 0;
        count = 1;
      } else count++;
    }
    if (rs != 0) v[ln++] = rs;
    while (ln >= 0 && v[ln - 1] == 0) ln--;
  }

  BInt operator * (const BInt & b) const {
    BInt rs;
    for (int i = 0; i < ln; i++) {
      int c = 0;
      for (int j = 0; j < b.ln || c > 0; j++) {
        if (j < b.ln) c += b.v[j] * v[i];
        if (i + j < rs.ln) c += rs.v[i + j];
        if (i + j < rs.ln) rs.v[i + j] = c % base;
        else rs.v[rs.ln++] = c % base;
        c /= base;
      }
    }
    while (rs.ln >= 0 && rs.v[rs.ln - 1] == 0) rs.ln--;
    return rs;
  }

  void Out() {
    for (int i = ln - 1; i >= 0; i--) {
      printf("%d", v[i]);
    }
    printf("\n");
  }
};

void Read() {
  char str1[50];
  char str2[50];
  while (scanf("%s%s", str1, str2) != EOF) {
    BInt a(str1);
    a.Out();
    BInt b(str2);
    b.Out();
    BInt rs = a * b;
    rs.Out();
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
