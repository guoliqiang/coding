// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-05 20:19:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// http://www.careercup.com/question?id=9381092

namespace algorithm {
class Record {
 public:
  Record() {
    pre_time = -1;
    memset(cnt, 0, sizeof(cnt));
    sum = 0;
  }

  void Add(int cur) {
    int t = cur % 3600;
    if (pre_time != -1 && pre_time != cur) {
      if (cur - pre_time >= 3600) {
        memset(cnt, 0, sizeof(cnt));
        sum = 0;
      } else {
        int b = (pre_time  + 1)% 3600;
        int e = t;
        while (b != e) {
          sum -= cnt[b];
          cnt[b] = 0;
          b = (b + 1) % 3600;
        }
        sum -= cnt[e];
        cnt[e] = 0;
      }
    }
    cnt[t]++;
    sum++;
    pre_time = t;
  }

  int GetHour() {
    return sum;
  }

  int GetSecond() {
    int t = time(NULL) % 3600;
    return cnt[t];
  }

  int GetMiniute() {
    int cur = time(NULL);
    int b = cur - 60 + 1;
    int rs = 0;
    for (int i = b; i <= cur; i++) rs += cnt[i % 3600];
    return rs;
  }

 private:
  int cnt[3600];
  int pre_time;
  int sum;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
