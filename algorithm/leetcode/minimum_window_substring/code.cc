// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-23 22:01:05
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 43/43 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 472 milli secs
 * Progress: 266/266 test cases passed.
 *
 *
 * http://discuss.leetcode.com/questions/97/minimum-window-substring
 * http://www.cnblogs.com/lichen782/archive/2013/07/16/leetcode_minimum_window_substring.html
 *
 * */

#include <map>
#include <vector>
#include <set>
#include "base/public/logging.h"

namespace algorithm {

std::string  MiniWindow(std::string & str, std::string & t) {
  std::map<char, int> ch_count;
  std::map<char, int> foo;
  int count = 0;
  std::string rs = "";

  int min = 0x7fffffff;
  for (int i = 0; i < t.size(); i++) {
    if (ch_count.count(t[i])) ch_count[t[i]]++;
    else ch_count[t[i]] = 1;
    foo[t[i]] = 0;
  }
  int begin = 0;
  int end = 0;
  while (end < str.size()) {
    if (!foo.count(str[end])) {
      end++;
      continue;
    }
    if (foo[str[end]] < ch_count[str[end]]) count++;
    foo[str[end]]++;
    
    if (count == t.size()) {
      while (begin < end) {
        if (foo.count(str[begin]) == 0) {
          begin++;
          continue;
        }
        
        if (foo[str[begin]] > ch_count[str[begin]]) {
          foo[str[begin]]--;
          begin++;
          // Note begin++ must be after foo[str[begin]]--
          // this bug used much time to fix.....
        } else {
          break;
        }
      }
      if (end - begin + 1 < min) {
        min = end - begin + 1;
        rs = str.substr(begin, end - begin + 1);
        // LOG(INFO) << "end:" << end << " begin:" << begin;
      }
    }

    end++;
  }
  return rs;
}

}  // namespace algorithm

namespace twice {
std::string MinWindow(std::string & s, std::string & t) {
  std::map<char, int> dict;
  std::map<char, int> tmap;
  for (int i = 0; i < t.size(); i++) {
      if (dict.count(t[i])) dict[t[i]]++;
      else dict[t[i]] = 1;
      tmap[t[i]] = 0;
  }
  int b = 0;
  int e = 0;
  int cur = 0;
  int min = INT_MAX;
  std::string rs = "";
  while (e < s.size()) {
      char ch = s[e];
      if (!dict.count(ch)) {
          e++;
          continue;
      }
      if (tmap[ch] < dict[ch]) {
        cur++;
      }
      tmap[ch]++;
      if (cur == t.size()) {
          while (true) {
            if (!dict.count(s[b])) b++;
            else {
              if (tmap[s[b]] > dict[s[b]]) {
                  tmap[s[b]]--;
                  b++;
              }
              else break;
            }
          }
          if (e - b + 1 < min) {
              min = e - b + 1;
              rs = s.substr(b, e - b + 1);
          }
      }
      e++;
  }
  return rs;
}
}  // namespace twice

using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "ADOBECODEBANC";
  std::string t = "ABC";
  LOG(INFO) << MiniWindow(str, t);
  return 0;
}
