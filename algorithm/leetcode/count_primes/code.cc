// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-18 18:02:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int CountPrimes(int n) {
  unsigned int size = n / (8 * sizeof(int)) + 1;
  unsigned int * data = new unsigned int[size];
  memset(data, 0xff, size * sizeof(unsigned int));
  unsigned int key = 0x80000000;
  for (int i = 2; i < n; i++) {
    int j = i * 2;

    int index = i / (8 * sizeof(unsigned int));
    int offset = i % (8 * sizeof(unsigned int));
    if ((data[index] & (key >> offset)) == 0) continue;

    while (j < n) {
      index = j / (8 * sizeof(unsigned int));
      offset = j % (8 * sizeof(unsigned int));
      data[index] &= ~(key >> offset);
      j+= i;
    }
  }
  int ans = 0;
  for (int i = 2; i < n; i++) {
    int index = i / (8 * sizeof(unsigned int));
    int offset = i % (8 * sizeof(unsigned int));
    if ((data[index] & (key >> offset)) != 0) {
      ans++;
    }
  }
  delete [] data;
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

namespace twice {
 int CountPrimes(int n) {
        char * ptr = new char[n / 8 + 1];
        memset(ptr, 0xFF, sizeof(char) * (n / 8 + 1));
        
        for (int i = 2; i < n; i++) {
            int idx = i / 8;
            int offset = i % 8;
            if ((ptr[idx] & (1 << offset)) == 0) continue;
            
            int j = i + i;
            while (j < n) {
                int idx2 = j / 8;
                int offset2 = j % 8;
                ptr[idx] &= ~(1 << offset);
                j += i;
            }
        }
        int ans = 0;
        for (int i = 2; i < n; i++) {
            int idx = i / 8;
            int offset = i % 8;
            if (ptr[idx] & (1 << offset)) ans++;
        }
        return ans;
    }
}

int main(int argc, char** argv) {
  LOG(INFO) << twice::CountPrimes(2);
  return 0;
  LOG(INFO) << CountPrimes(100);
  LOG(INFO) << CountPrimes(1000);
  LOG(INFO) << CountPrimes(1500000);
  return 0;
}
