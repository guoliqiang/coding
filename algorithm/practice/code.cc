// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-11-25 18:11:36
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

char * BM(char * s, char * p) {
  int slen = strlen(s);
  int plen = strlen(p);
  if (plen == 0) return s;
  std::map<char, int> bc;
  std::vector<int> suffix(plen, 0);
  std::vector<int> gs(plen, plen);
  for (int i = 0; i < plen; i++) bc[p[i]] = i;
  suffix[plen - 1] = plen;
  for (int i = plen - 2; i >= 0; i--) {
    int k = 0;
    while (i - k >= 0 && p[i - k] == p[plen - 1 - k]) k++;
    suffix[i] = k;
  }
  for (int i = plen - 1; i >= 0; i++) {
    if (suffix[i] == i + 1) {
      for (int j = plen - 1 - suffix[i]; j >= 0; j--) {
        gs[j] = plen - 1 - i;
      }
      break;
    }
  }
  for (int i = 0; i < plen - 1; i++) {
    gs[plen - 1 - suffix[i]] = plen - 1 - i;
  }
  int j = 0;
  while (j <= slen - plen) {
    int k = 0;
    while (k < plen && p[plen - 1 - k] == s[j + plen - 1 - k]) k++;
    if (k == plen) return s + j;
    int move = gs[plen - 1 - k];
    if (!bc.count(s[j + plen - 1 - k])) move = std::max(move, plen - k);
    else move  = std::max(move, plen - 1 - k - bc[s[j + plen - 1 - k]]);
    j += move;
  }
  return NULL;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
 return 0;
}
