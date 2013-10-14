// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-26 14:49:52
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 19/19 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 59/59 test cases passed.
 *
 * */

#include <algorithm>
#include "base/public/logging.h"

namespace algorithm {

int LengthOfLastWord(const char * s) {
  if (!s) return 0;
  int size  = strlen(s);
  
  int e = size - 1;
  while (e >= 0 && s[e] == ' ') e--;
  if (e < 0) return 0;
  
  int b = e;
  while (b >= 0 && s[b] != ' ') b--;
  // Note if b == -1 should return e + 1 (== e - b)
  return e - b;
}

}  // namespace algorithm

namespace other {
int length(const char * s) {
  int r = 0;
  while(*s) {
    if(*(s++) != ' ')
      ++r;
     else if(*s && *s != ' ')
      r = 0;
  }
  return r;
}
}

namespace twice {
   int lengthOfLastWord(const char *s) {
        // Note: The Solution object is instantiated only once and is reused by each test case.
        int pre = 0;
        int cur = 0;
        while (*s != '\0') {
            if (*s == ' ') {
                if (cur != 0) pre = cur;
                cur = 0;
            } else cur++;
            s++;
        }
        if (cur != 0) return cur;
        return pre;
    }
}  // namespace twice

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "hello world  ";
  LOG(INFO) << LengthOfLastWord(str.c_str());
  LOG(INFO) <<  other::length(str.c_str());
  return 0;
}
