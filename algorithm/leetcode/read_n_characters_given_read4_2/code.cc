// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-11 23:57:16
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class Solution {
 public:
  Solution() {
    cnt = 0;
    memset(buff, 0, sizeof(buff));
  }

  int read4(char * buff) { return 0; }

  int read2(char * data, int n) {
    int len = 0;
    while (len < n) {
      while (len < n && index < cnt) {
        data[len++] = buff[index++];
      }
      if (index == cnt) {
        index = 0;
        cnt = read4(buff);
      }
      if (cnt == 0) break;
    }
    return len;
  }
  int read(char * data, int n) {
    int k = 0;
    while (k < n) {
      if (index == 0) {
        cnt = read4(buff);
      }
      if (cnt == 0) break;

      while (k < n && index < cnt) {
        data[k++] = buff[index++];
      }
      if (index == cnt) {
        index = 0;
      }
    }
    return k;
  }

 private:
  char buff[4];
  int index;
  int cnt;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
