// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 15:51:44
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

// 通用的next permutaition算法
namespace algorithm {

int Get(int a, int b, int e) {
  int mask = (1 << e) + ((1 << e) - 1) - ((1 << b) - 1);
  return a & mask;
}

int GetReverse(int a, int b, int e) {
  int t = Get(a, b, e);
  for (int i = b; i <= (b + e) / 2; i++) {
    if (static_cast<bool>((1 << i) & t) ==
        static_cast<bool>(1 << (e - (i - b)) & t)) continue;
    if (static_cast<bool>((1 << i) & t) == true) {
      t = t & ~(1 << i);
    } else {
      t = t | (1 << i); 
    }
    if (static_cast<bool>((1 << (e - (i - b))) & t) == true) {
      t = t & ~(1 << (e - (i - b)));
    } else {
      t = t | (1 << (e - (i - b))); 
    }
  }
  int mask = (1 << e) + ((1 << e) - 1) - ((1 << b) - 1);
  return a ^ (a & (mask)) ^ t;
}

void Swap(int & a, int i, int j) {
  if (static_cast<bool>((1 << i) & a) ==
      static_cast<bool>((1 << j) & a)) return;
  if (static_cast<bool>((1 << i) & a) == true) {
    a = a & ~(1 << i);
  } else {
    a = a | (1 << i); 
  }
  if (static_cast<bool>((1 << j) & a) == true) {
    a = a & ~(1 << j);
  } else {
    a = a | (1 << j); 
  }
}

bool NextL(int & a) {
  int i = 0;
  while (i < 32 && (1 << i & a) == 0) i++;
  // LOG(INFO) << "i:" << i;

  if (i == 32) return false;
  int j = i;
  while (j < 32 && ((1 << j) & a) != 0) j++;
  if (j == 32) return false;
  
  // LOG(INFO) << "i:" << i << " j :" << j;
  Swap(a, j, i);
  // LOG(INFO) << a;
  a = GetReverse(a, 0, j - 1);
  return true;
}

bool NextS(int & a) {
  int i = 0;
  while (i < 32 && (1 << i & a) != 0) i++;
  if (i == 32) return false;
  int j = i;
  while (j < 32 && (1 << j & a) == 0) j++;
  if (j == 32) return false;
  Swap(a, j, i);
  a = GetReverse(a, 0, j - 1);
  return true;
}

}  // namespace algorithm

// 此题目由于每一位非0即1，因此可以有简单的处理办法，编程
// 比较容易

namespace NB {
bool GetBit(int a, int i) {
  return (1 << i & a) != 0;
}

int SetBit(int a, int i, bool r) {
  if (r) {
    return a | 1 << i;
  } else {
    return a & ~(1 << i);
  }
}

bool NextL(int & a)  {
  int i = 0;
  while (i < 32 && !GetBit(a, i)) i++;
  if (i == 32) return false;
  int onenum = 0;
  while (i < 32 && GetBit(a, i)) {
    onenum++;
    i++;
  }
  if (i == 32) return false;
  // LOG(INFO) << "i:" << i << " num :" << onenum; 
  a = SetBit(a, i, true);
  for (int j = 0; j < i; j++) {
    if (j < onenum - 1) a = SetBit(a, j, true);
    else a = SetBit(a, j, false);
  }
  return true;
}

bool NextS(int & a)  {
  int i = 0;
  while (i < 32 && GetBit(a, i)) i++;
  if (i == 32) return false;
  int zeronum = 0;
  while (i < 32 && !GetBit(a, i)) {
    zeronum++;
    i++;
  }
  if (i == 32) return false;
  // LOG(INFO) << "i:" << i << " num :" << onenum; 
  a = SetBit(a, i, false);
  for (int j = 0; j < i; j++) {
    if (j < zeronum - 1) a = SetBit(a, j, false);
    else a = SetBit(a, j, true);
  }
  return true;
}

}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  int a = 12;
  int num = 10;
  while (NB::NextS(a)) {
    LOG(INFO) << a;
    if (num-- < 0) break;
  }
  /*
  int a = 12;
  int num = 10;
  
  while (NextS(a)) {
    LOG(INFO) << a;
    if (num-- <= 0) break;
  }

  a = 3;
  num = 10;
  while (NextL(a)) {
    LOG(INFO) << a;
    if (num-- <= 0) break;
  }
  */
  return 0;
}
