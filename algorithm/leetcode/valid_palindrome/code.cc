// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-01 00:43:09
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 4 milli secs
 * Progress: 27/27 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 48 milli secs
 * Progress: 475/475 test cases passed.
 * */

#include "base/public/common_head.h"
namespace algorithm {

bool Ignore(char ch) {
  if ((ch >= 'A' && ch <= 'Z') ||
      (ch >= 'a' && ch <= 'z') ||
      (ch >= '0' && ch <= '9')) return false;
  return true;
}

char Lower(char ch) {
  if (ch >= 'A' && ch <= 'Z') return ch + ('a' - 'A');
  return ch;
}

// 会 T
bool IsPalindrome(std::string & s) {
  std::string s1;
  std::string s2;
  for (int i = 0; i < s.size(); i++) {
    if (!Ignore(s[i])){
      s1.append(1, Lower(s[i]));
      s2.insert(s2.begin(), 1, Lower(s[i]));
    }
  }
  LOG(INFO) << s1 << " " << s2;
  return s1 == s2;
}

bool IsPalindrome2(std::string & s) {
  int b = 0;
  int e = s.size() - 1;
  if (b == e) return true;
  while (b < s.size() && Ignore(s[b])) b++;
  while (e >= 0 && Ignore(s[e])) e--;
  if (e < 0 && b == s.size()) return true;
  while (b <= e) {
    while (Ignore(s[b])) b++;
    while (Ignore(s[e])) e--;
    if (Lower(s[b]) != Lower(s[e])) return false;
    else {
      e--;
      b++;
    }
  }
  return true;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "A man, a plan, a canal: Panama";
  LOG(INFO) << IsPalindrome2(str);
  return 0;
}
