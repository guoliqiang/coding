// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 14:39:08
// File  : code.cc
// Brief : http://blog.sina.com.cn/s/blog_691161ed0101953v.html
//         http://discuss.leetcode.com/questions/222/wildcard-matching

/*
Implement wildcard pattern matching with support for '?' and '*'.

'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).

The matching should cover the entire input string (not partial).

The function prototype should be:
bool isMatch(const char *s, const char *p)

Some examples:
isMatch("aa","a") → false
isMatch("aa","aa") → true
isMatch("aaa","aa") → false
isMatch("aa", "*") → true
isMatch("aa", "a*") → true
isMatch("ab", "?*") → true
isMatch("aab", "c*a*b") → false

*/

#include <iostream>
#include "base/public/logging.h"

namespace algorithm {
/*
 *      a  b  c  e  f
 *    1 0  0  0  0  0
 *  a 0 1  0  0  0  0
 *  b 0 0  1  0  0  0
 *  c 0 0  0  1  0  0
 *  * 0 0  0  1  1  1  // 只要一个匹配成功，右侧全部成功，即填写1
 *  f 0 0  0  0  0  1
 *
 *  下标从1开始，一行一行的写值
 *  dp[i][j] 表示p[1 ~ i] 与 s[1 ~ j]的匹配情况
 *
 *  if p[i] != '*'
 *  dp[i][j] = dp[i - 1] [j - 1] && Mathch(p[i], s[j])
 *
 *  if p[i]  == '*'
 *  dp[i][j] = dp[i - 1][j]  (如果当前行中还没有出现国成功匹配)
 *  否则  dp[i][j] = 1
 *
 * */

/*
 * Run Status: Accepted!
 * Program Runtime: 132 milli secs
 * Progress: 1800/1800 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 100/100 test cases passed.
 *
 * */
// memory limit Exceeded.
void DP(const char * s, const char * p, int ** dp) {
  HERE(INFO);
  int s_len = strlen(s);
  int p_len = strlen(p);
  for (int i = 0; i < s_len; i++) dp[0][i] = 0;
  dp[0][0] = 1;
  for (int i = 1; i < p_len; i++) {
    for (int j = 0; j < s_len; j++) {
      if (*(p + i) == '*') {
        if (j == 0) {
          dp[i][j] = dp[i - 1][j];  // 在* 出现在pattern第一个字符时会起作用
          continue;
        }
        dp[i][j] = (dp[i - 1][j] + dp[i][j - 1] >= 1 ? 1 : 0);
      } else {
        if (j == 0) {
          dp[i][j] = 0;  // 此时dp[i][0]无用
          continue;
        }
        if (*(p + i) == '?' || *(p + i) == *(s + j)) 
          dp[i][j] = dp[i - 1][j - 1];
        else
          dp[i][j] = 0;
      }
    }
  }
  /*
  for (int i = 0; i < p_len; i++) {
    for (int j = 0; j < s_len; j ++) {
      std::cout << dp[i][j] << " ";
    }
    std::cout << std::endl;
  }
  */
}

bool IsMatchingDP(std::string s, std::string p) {
  s.insert(s.begin(), ' ');
  p.insert(p.begin(), ' ');
  int * * dp = new int * [p.size()];
  for (int i = 0; i < p.size(); i++) {
    dp[i] = new int[s.size()];
  }
  DP(s.c_str(), p.c_str(), dp);
  bool rs = dp[p.size() - 1][s.size() - 1];
  for (int i = 0; i < p.size(); i++) {
    delete [] dp[i];
  }
  delete [] dp;
  return rs;
}

bool DPMem(const char * s, const char * p, int ** dp) {
  int s_len = strlen(s);
  int p_len = strlen(p);
  for (int i = 0; i < s_len; i++) dp[0][i] = 0;
  dp[0][0] = 1;
  bool flag = 1;
  for (int i = 1; i < p_len; i++) {
    for (int j = 0; j < s_len; j++) {
      if (*(p + i) == '*') {
        if (j == 0) {
          dp[flag][j] = dp[!flag][j];  // 相当于*匹配空
          continue;
        }
        // 右边是1也设置为1
        dp[flag][j] = (dp[!flag][j] + dp[flag][j - 1] >= 1 ? 1 : 0);
      } else {
        if (j == 0) {
          dp[flag][j] = 0;  // 此时dp[i][0]无用
          // 即使当前p为？其值也为0，因为其要匹配一个字符，儿当前的空已经被p中第一个空匹配上了
          continue;
        }
        if (*(p + i) == '?' || *(p + i) == *(s + j)) 
          dp[flag][j] = dp[!flag][j - 1];
        else
          dp[flag][j] = 0;
      }
    }
    flag = !flag;
  }
  return dp[!flag][s_len - 1];
}



bool IsMatchingDPMem(std::string s, std::string p) {
  // 必须加两个空字符，否则逻辑太复杂
  s.insert(s.begin(), ' ');
  p.insert(p.begin(), ' ');
  int * * dp = new int * [2];
  for (int i = 0; i < 2; i++) {
    dp[i] = new int[s.size()];
  }
  bool rs = DPMem(s.c_str(), p.c_str(), dp);
  for (int i = 0; i < 2; i++) {
    delete [] dp[i];
  }
  delete [] dp;
  return rs;
}

/*
 *  time limited
 *
 * */

bool IsMatching(const char * s, const char * p) {
  if (*s == '\0' && *p == '\0') return true;
  if (*p == '\0') return false;
  
  if (*p == '?' && *s != '\0') {
    return IsMatching(s + 1, p + 1);
  } else {
    if (*p == '*') {
      if (IsMatching(s , p + 1)) return true;
      if (*s != '\0') return IsMatching(s + 1, p);
    } else {
      if (*s != '\0' && *p == *s) return IsMatching(s + 1, p + 1);
      else return false;
    }
  }
  return false;
}

bool IsMatchingWrapper(const char * s, const char * p) {
  int len = strlen(p);
  std::string pattern;
  int num = 0;
  for (int i = 0; i < len; i++) {
    if(i != 0 && p[i] == '*' && p[i - 1] == '*') continue;
    pattern.push_back((char)p[i]);
    if (p[i] != '*')num++;
  }
  std::string str(s);
  // 加速，有些case不加这个会超时
  if (num > str.size()) return false;
  return IsMatchingDPMem(str, pattern);  // succeed version
  // return IsMatchingDP(str, pattern);  // memory exceeded version
  // return IsMatching(s, pattern.c_str());  // time exceeded version
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::string s;
  std::string p;
  
  s = "a";
  p = "a*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());
  
  s = "";
  p = "*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbababababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaababaaaabb";
  p = "*aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());
  
  
  s = "aa";
  p = "aa";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aaa";
  p = "aa";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aa";
  p = "*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aa";
  p = "a*";

  s = "abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbababababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaababaaaabb";
  p = "*aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());
  
  
  s = "aa";
  p = "aa";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aaa";
  p = "aa";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aa";
  p = "*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aa";
  p = "a*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aa";
  p = "?*";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  s = "aab";
  p = "c*a*b";
  LOG(INFO) << s << " " << p << " " << IsMatchingWrapper(s.c_str(), p.c_str());

  return 0;
}
