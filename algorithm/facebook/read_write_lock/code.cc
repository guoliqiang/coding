// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 21:22:56
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
class RWClock {
 public:
  RWClock() : read_cnt(0), write_cnt(0) {}
  void ReadLock() {
    MutexLock(mutex);
    while (write_cnt != 0) {
      WaiteR();
    }
    read_cnt++;
  }
  void ReadUnLock() {
    MutexLock(mutex);
    read_cnt--;
    if (readcnt == 0) WakeUpWrite();
  }
  void WriteLock() {
    MutexLock(mutex);
    while (read_cnt != 0 || write_cnt != 0) {
      WaiteW();
    }
    write_cnt++;
  }
  void WriteUnLock() {
    MutexLock(mutex);
    write_cnt--;
    WakeUpW();
    WakeUpAllR();
  }
 private:
   int read_cnt;
   int write_cnt;
   Mutex mutex;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
