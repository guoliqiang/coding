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


namespace twice {
bool IsMatch(std::string s, std::string p) {
  s = ' ' + s;
  p = ' ' + p;
  int n = s.size();
  int m = p.size();
  std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
  bool flag = false;
  dp[flag][0] = 1;
  for (int i = 1; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (j == 0) {
        if (p[i] == '*') dp[!flag][j] = dp[flag][j];
        else dp[!flag][j] = 0;
      } else {
         if (p[i] == '?' || p[i] == s[j]) dp[!flag][j] = dp[flag][j - 1];
         else if (p[i] == '*') dp[!flag][j] = std::max(dp[!flag][j - 1], dp[flag][j]);
         else dp[!flag][j] = 0;
      }
    }
    flag = !flag;
  }
  return dp[flag][n - 1];
}
}  // namespac twice

namespace TLE {
bool IsMatch(const std::string & s, int i, const std::string & p, int j) {
    if (i == s.size() && j == p.size()) return true;
    if (j == p.size()) return false;
    if (p[j] == '*') {
        if (IsMatch(s, i, p, j + 1)) return true;
        if (i < s.size() && IsMatch(s, i + 1, p, j)) return true;
        return false;
    } else if (p[j] == '?') {
        if (i < s.size() && IsMatch(s, i + 1, p, j + 1)) return true;
        return false;
    } else {
        if (i < s.size() && s[i] == p[j]) {
            return IsMatch(s, i + 1, p, j + 1);
        }
        return false;
    }
}
}  // namespace TLE

namespace NB {
using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
       int i = 0;
       int j = 0;
       int start = -1;
       int bi = 0;

       while (i < s.size()) {
           if (j < p.size() && (s[i] == p[j] || p[j] == '?')) {
               i++;
               j++;
               continue;
           }
           // 一个也不匹配
           if (j < p.size() && p[j] == '*') {
               start = j;
               j++;
               bi = i;
               continue;
           }
           // 至少匹配一个
           if (start != -1) {
               j = start + 1;
               i = ++bi;
               continue;
           }
           return false;
       }

       while (j < p.size() && p[j] == '*') j++;
       return j == p.size();
    }
};
}  // namespace NB


namespace clear {
bool isMatch(string s, string p) {
    s = " " + s;
    p = " " + p;
    int m = p.size();
    int n = s.size();

    std::vector<std::vector<int> > dp(2, std::vector<int>(n, 0));
    dp[0][0] = 1;

    for (int i = 1; i < m; i++) {
        int left = 0;
        for (int j = 0; j < n; j++) {
            if (p[i] == '*') {
                dp[i % 2][j] = std::max(dp[(i - 1) % 2][j], left);
            } else if (p[i] == '?' || p[i] == s[j]) {
                dp[i % 2][j] = dp[(i - 1)%2][j - 1];
            } else {
                dp[i % 2][j] = 0;
            }
            left = std::max(left, dp[i % 2][j]);
        }
    }
    return dp[(m - 1) % 2][n - 1];
}
}  // namespace clear

using namespace algorithm;

int main(int argc, char** argv) {
  std::string s;
  std::string p;
  
  s = "abc";
  p = "*?*?*?*?";
  LOG(INFO) << s << " " << p << " " << test::IsMatch(s, p);
  return 0;
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
