// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 17:20:41
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {


/*
 * 算法导论快速排序分划的写法
 * */

void RemoveDup(char * ptr) {
  int n = strlen(ptr);
  int k = -1;
  for (int i = 0; i < n; i ++) {
    bool not_see = true;
    for (int j = 0; j <= k; j++) {
      if (ptr[j] == ptr[i]) {
        not_see = false;
        break;
      }
    }
    if (not_see) {
      if (k + 1 != i) ptr[++k] = ptr[i];
      else ++k;
    }
  }
  ptr[++k] = '\0';
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  char ptr[] = "abcadfg";
  RemoveDup(ptr);
  LOG(INFO) << ptr;
  return 0;
}
