// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 08:57:22
// File  : code.cc
// Brief :

#include "base/public/logging.h"
#include <math.h>
#include <algorithm>

// recursive solution

bool isPalindrome(int x, int &y) {
  if (x < 0) return false;
  if (x == 0) return true;
  if (isPalindrome(x/10, y) && (x%10 == y%10)) {
    LOG(INFO) << "x:" << x << " y: " << y;
    y /= 10;
    LOG(INFO) << "y:" << y;
    return true;
  } else {
    return false;
  }
}
bool isPalindrome(int x) {
  // return isPalindrome(x, x);
  // Start typing your C/C++ solution below
  // DO NOT write int main() function
  if (x < 0) return false;
  if (x < 10) return true;
  int num = 0;
  int foo = x;
  while (foo) {
    foo /= 10;
    num++;
  }
  num--;
  foo = x;
  // LOG(INFO) << foo << " " << foo%10 << " " << static_cast<int>(foo / pow(10, num));
  while (foo && (foo%10) == static_cast<int>((foo / pow(10, num)))) {
    foo -= static_cast<int>(foo/pow(10, num)) * static_cast<int>(pow(10, num));
    foo /= 10;
    num -= 2;
  }
  return !foo;
}


int main(int argc, char** argv) {
  LOG(INFO) << isPalindrome(2112);
  return 0;
}
