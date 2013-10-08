// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 14:25:38
// File  : code.cc
// Brief :
// NB http://hawstein.com/posts/19.10.html

#include <time.h>
#include "base/public/common_head.h"

namespace algorithm {

int rand5() {
  return rand() % 5 + 1;
}

int rand7() {
  int n1 = rand5() - 1;
  int n2 = rand5() - 1;
  int t = (n1 * 5 + n2) / 3 + 1;
  if (t <= 7) return t;
  else return rand7();
}

}  // namespace algorithm


namespace NB {
/*
 *  
 * 由 rand_k 构造 rand_k^2:
 *  k * (rand_k() - 1) + rand_k();
 * 构造rand_k^3
 *  k^2 * (rand_k() - 1) + k * (rand_k() - 1) + rand_k()
 *  
 *
 * */
int rand5() {
  return rand() % 5 + 1;
}

int rand7() {
  int x = ~(1 << 31);
  while (x > 21) {
    x = 5 * (rand5() - 1) + rand5();
  }
  return x % 7 + 1;
}

int rand7G() {
  int t = 5;
  int num = 1;
  while (t < 7) {
    t *= 5;
    num++;
  }
  int x = ~(1 << 31);
  while (x > 7 * ( t / 7)) {
    int tmp = 0;
    for (int j = 0; j < num; j++) tmp += pow(5, j + 1);
    x = tmp * (rand5() - 1) + rand5();
  }
  return x % 7 + 1;
}

/*
 * 通用架构 A > B
 * */
// A  == > B
/*
int randA() {
  return 0;
}

int randB() {
  int x = ~(1 << 31);
  while (x > B) x = randA();
  return x;
}

// B == > A
int RandB() {
  return 0;
}

int RandA() {
  int t = B;
  int num = 1;
  while (t < A) {
    t *= B;
    num++;
  }
  int x = ~(1 << 31);
  while (x > A * (t / A)) {
    int tmp = 0;
    for (int j = 0; j < num; j++) tmp += pow(B, j + 1);
    x = tmp * (RandB() - 1) + RandB();
  }
}
*/
}  // namespace NB

using namespace algorithm;


int main(int argc, char** argv) {
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << rand7();
    LOG(INFO) << NB::rand7();
    LOG(INFO) << NB::rand7G();
  }
  return 0;
}
