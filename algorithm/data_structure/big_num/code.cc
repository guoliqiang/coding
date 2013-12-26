// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-04 04:17:40
// File  : code.cc
// Brief :
// poj 2389
// wikioi 3113
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 100;
const int base = 10;

struct BInt{
  int ln;
  int v[MAX]; // 低位-->高位
  BInt(int r = 0) {
    memset(v, 0, sizeof(v));
    ln = 0;
    while (r) {
      v[ln++] = r % base;
      r /= base;
    }
  }

  BInt(const char * str) {
    memset(v, 0, sizeof(v));
    ln = 0;
    int t = 0;
    while (pow(10, t) != base) t++;
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i-= t) {
      int foo = 0;
      for (int j = i; j >= std::max(i - t + 1, 0); j--) {
        foo = foo * 10 + str[j] - '0';
      }
      v[ln++] = foo;
    }
  }

  void Out() {
    if (ln == 0) printf("0");
    else {
      for (int i = ln - 1; i >= 0; i--) {
        printf("%d", v[i]);
      }
    }
    printf("\n");
  }

  //
  BInt & operator = (const BInt & b) {
    ln = b.ln;
    memcpy(v, b.v, ln * sizeof(int));
    return *this;
  }
  
  // Note： must be const function
  bool operator < (const BInt & b) const {
    if (ln != b.ln) return ln < b.ln;
    int i = 0;
    for (i = ln - 1; i >= 0; i--) {
      if (v[i] != b.v[i]) break;
    }
    return i < 0 ? false : v[i] < b.v[i];
  }
  
  // 可以看出所有的比较预算都可以基于< 实现
  // 即一个新的数据类型，只需要实现< 操作既可以
  bool operator <= (const BInt & b) const {
    return !(b < (*this));
  }

  //
  bool operator == (const BInt & b) const{
    return !((*this) < b) && !(b < (*this));
  }

  //
  bool operator > (const BInt & b) const {
    return (b < (*this));
  }
  //
  bool operator >= (const BInt & b) const {
    return (b <= (*this));
  }

  //
  BInt operator + (const BInt & b) const {
    BInt res;
    int c = 0;
    int i = 0;
    int j = 0;
    while (i < ln || j < b.ln || c > 0) {
      int t1 = i < ln ? v[i++] : 0;
      int t2 = j < b.ln ? b.v[j++] : 0;
      int t = t1 + t2 + c;
      res.v[res.ln++] = t % base;
      c = t / base;
    }
    while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
    return res;
  }
  // 调用者保证不会出现负数
  BInt operator - (const BInt & b) const {
    BInt res;
    int cy = 0;  // 借位
    for (int i = 0; i < ln; i++) {
      int t = v[i] - b.v[i] - cy;
      res.v[i] = t;
      if (res.v[i] < 0) {
        res.v[i] += base;
        cy = 1;
      } else cy = 0;
    }
    res.ln = ln;
    while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
    return res;
  }
  //
  BInt operator * (const BInt & b) const {
    BInt res;
    res.ln = 0;
    if (0 == b.ln || ln == 0)  return res;
    for (int i = 0; i < ln; i++) {
      int c = 0;
      for (int j = 0; j < b.ln || c > 0; j++) {
        if (j < b.ln) c += v[i] * b.v[j];
        if (i + j < res.ln) c += res.v[i + j];
        if (i + j >= res.ln) res.v[res.ln++] = c % base;
        else res.v[i + j] = c % base;
        c /= base;
      }
    }
    while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
    return res;
  }
  //
  BInt operator / (const BInt & b) const {
    BInt res;
    BInt mod;
    for (int i = ln - 1; i >= 0; i--) {
      mod = mod * base + v[i];
      int l = 0;
      int r = base - 1;
      while (l < r) {
        int mid = (l + r + 1) / 2;
        if (b * mid <= mod) l = mid;
        else r = mid - 1;
      }
      res.v[i] = l;
      mod = mod - b * l;
    }
    res.ln = ln;
    while (res.ln > 0 && res.v[res.ln - 1] == 0) res.ln--;
    return res;
  }
  //
  BInt operator % (const BInt & b) const {
    BInt t = (*this) / b;
    BInt rs = (*this) - t * b;
    return rs;
  }
};

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  BInt a1(8996678);
  BInt a2(83);
  a1.Out();
  a2.Out();
  (a1 + a2).Out();
  return 0;
  (a2 - a1).Out();
  (a2 * a1).Out();
  (a2 / 82).Out();
  (a2 % 3).Out();

  LOG(INFO) << (a1 < a2);
  LOG(INFO) << (a1 <= a2);
  LOG(INFO) << (a1 > a2);
  LOG(INFO) << (a1 >= a2);
  HERE(INFO);
  LOG(INFO) << (a2 < a1);
  LOG(INFO) << (a2 <= a1);
  LOG(INFO) << (a2 > a1);
  LOG(INFO) << (a2 >= a1);
  return 0;
}
