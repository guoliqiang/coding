// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-01 02:59:23
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
/*
 * Note: mod选择足够大的质数,至少大于字符串的长度，
 *       因为如果小于字符串的长度，其有可能被长度整除
 *
 * 
 * 因此 hash table的大小应该是质数才好
 * */

unsigned int HashA(char * url, int mod) {
  unsigned int n = 0;
  char * p = reinterpret_cast<char *>(&n);
  for (int i = 0; i < strlen(url); i++) {
    p[i % sizeof(n)] ^= url[i];
  }
  return n % mod;
}



}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
