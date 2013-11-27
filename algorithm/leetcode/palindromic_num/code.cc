// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 08:57:22
// File  : code.cc
// Brief :

/*
 *
 Determine whether an integer is a palindrome. Do this without extra space.
 click to show spoilers.
 Some hints:
 Could negative integers be palindromes? (ie, -1)
 If you are thinking of converting the integer to string, note the restriction of using extra space.
 You could also try reversing an integer. 
 However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?
 There is a more generic way of solving this problem.

 不存在一个palindrome数，reverse后越界而在reverse前不越界
 
 * */

#include "base/public/logging.h"
#include <math.h>
#include <algorithm>

// recursive solution
namespace NB {
// 注意这种递归方式的空间复杂度并不是O(1)
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

}  // namespace NB

bool isPalindrome(int x) {
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
  while (foo && (foo%10) == static_cast<int>((foo / pow(10, num)))) {
    foo -= static_cast<int>(foo/pow(10, num)) * static_cast<int>(pow(10, num));
    foo /= 10;
    num -= 2;
  }
  return !foo;
}

namespace twice {

bool isPalindrome(int x) {
  if (x < 0) return false;
  int t = x;
  int r = 0;
  while (t) {
    r = r * 10 + t % 10;
    t /= 10;
  }
  return r == x;
}
}  // namespace twice

int main(int argc, char** argv) {
  LOG(INFO) << isPalindrome(2112);
  return 0;
}
