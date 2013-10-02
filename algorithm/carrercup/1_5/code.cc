// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 18:03:29
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

void Replace(char * p) {
  if (p == NULL) return;
  int n = strlen(p);
  int num = 0;
  for (int i = 0; i < n; i++) {
    if (p[i] == ' ') num++;
  }
  int k = n + 2 * num;
  p[k--] = '\0';
  // from last to first
  for (int i = n - 1; i >= 0; i--) {
    if (p[i] == ' ') {
      p[k--] = '0';
      p[k--] = '2';
      p[k--] = '%';
    } else {
      p[k--] = p[i];
    }
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  char * p = new char[20];
  p[0] = ' ';
  p[1] = 'a';
  p[2] = ' ';
  p[3] = ' ';
  p[4] = '\0';
  Replace(p);
  LOG(INFO) << p;
  return 0;
}
