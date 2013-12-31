// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-30 18:31:45
// File  : code.cc
// Brief :

/*
Characters: a to z 
Operators: * + 
* -> matches zero or more (of the character that occurs previous to this operator) 
+ -> matches one or more (of the character that occurs previous to this operator) 

Output if a given pattern matches a string. 
Example: 
pattern:a*b 
string:aaab b, ab, aab, aaab, ab 
output:1 

pattern:a+aabc 
string:ab aabc, aaabc, aaaabc .. 
output:0 

pattern:aa*b*ab+ 
string:aab aab, aabab, aaaabbab 
output:1 

pattern: a+a*b* 
string: a ab, aab, aaabb 
output: 1 

Valid Assumptions: Please assume that both the pattern and string input are valid
*/

#include "base/public/common_ojhead.h"

namespace algorithm {
bool IsMatch(const char * s, const char * p) {
  if (*s == '\0' && *p == '\0') return true;
  if (*p == '\0') return false;
  if (*(p + 1) == '*' && IsMatch(s, p + 2)) return true;
  
  if (*p == '+' || *p == '*') {
    if (IsMatch(s, p + 1)) return true;
    if (*s == *(s - 1) && IsMatch(s + 1, p)) return true;
    return false;
  } else if (*p == *s) {
    return IsMatch(s + 1, p + 1);
  } else {
    return false;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  LOG(INFO) << IsMatch("a", "a+a*b*");
  LOG(INFO) << IsMatch("ab", "a+a*b*");
  LOG(INFO) << IsMatch("aab", "a+a*b*");
  LOG(INFO) << IsMatch("aaabb", "a+a*b*");
  HERE(INFO);
  LOG(INFO) << IsMatch("aab", "aa*b*ab+");
  LOG(INFO) << IsMatch("aabab", "aa*b*ab+");
  LOG(INFO) << IsMatch("aaaabbab", "aa*b*ab+");
  HERE(INFO);
  LOG(INFO) << IsMatch("ab", "a+aabc");
  LOG(INFO) << IsMatch("aabc", "a+aabc");
  LOG(INFO) << IsMatch("aaabc", "a+aabc");
  LOG(INFO) << IsMatch("aaaabc", "a+aabc");
  HERE(INFO);
  LOG(INFO) << IsMatch("aaab", "a*b");
  LOG(INFO) << IsMatch("b", "a*b");
  LOG(INFO) << IsMatch("ab", "a*b");
  LOG(INFO) << IsMatch("aab", "a*b");
  LOG(INFO) << IsMatch("aaab", "a*b");
  LOG(INFO) << IsMatch("ab", "a*b");
  return 0;
}
