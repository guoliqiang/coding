// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 21:24:14
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 44 milli secs
 * Progress: 1016/1016 test cases passed.
 *
 * 如果是字符串存储的很大的数，只能是手动开平方根方法  见number/sqrt
 * */

#include "base/public/common_head.h"

namespace algorithm {

int Sqrt(int x) {
  long long int b = 0;
  long long int e = x;
  long long int rs = 0;
  if (e < b)  return rs;

  while (b <= e) {
    if (b == e) {
      if(b * b <= x && (b + 1) * (b + 1) > x) rs = b;
      break;
    }
    long long int mid = b + (e - b) / 2;
    if (mid * mid <= x && (mid + 1) * (mid + 1) > x) {
      rs = mid;
      break;
    } else {
      if (mid * mid < x) b = mid + 1;
      else e = mid - 1;
    }
  }
  return rs;
}


}  // namespace algorithm

using namespace algorithm;

namespace twice {
    int sqrt(int x) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        int b = 0;
        int e = x / 2 + 1;
        int rs = 0;
        while (b <= e) {
            long long mid = b + (e - b) / 2;  // 防止越界　bug fixed
            if (mid * mid <= x && (mid + 1) * (mid + 1) > x) {
                rs = mid;
                break;
            } else {
                if (mid * mid < x) b = mid + 1;
                else e = mid - 1;
            }
        }
        return rs;
    }

    int sqrt2(int x) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        if (x <= 1) return x;  // bug fixed
        int b = 0;
        int e = x / 2 + 1;  // 或者　e = x  e == x /2  error!!
        int rs = 0;
        while (b <= e) {
            int mid = b + (e - b) / 2;
            if (mid == 0) {
                break;
            }
            if (mid <= x / mid &&  mid + 1 > x / (mid + 1)) {
                rs = mid;
                break;
            } else {
                if (mid < x / mid) b = mid + 1;
                else e = mid - 1;
            }
        }
        return rs;
    }
}  // namespace twice

int main(int argc, char** argv) {
  LOG(INFO) << Sqrt(2147395599);
  return 0;
  LOG(INFO) << Sqrt(10);
  LOG(INFO) << Sqrt(25);
  LOG(INFO) << Sqrt(0);
  LOG(INFO) << Sqrt(1);
  return 0;
}
