// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-23 10:11:08
// File  : code.cc
// Brief :

#include <algorithm>
#include <map>
#include <iostream>
#include "base/public/logging.h"
#include "base/public/string_util.h"

/*
Implement strStr().
Returns a pointer to the first occurrence of needle in haystack, or null if needle is not part of haystack.
*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 16/16 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 36 milli secs
 * Progress: 67/67 test cases passed.
 *
 * */

namespace algorithm {

const char * BM(const char * str, const char * pattern) {
  int p_len = strlen(pattern);
  int s_len = strlen(str);
  if (p_len == 0) return str;

  std::map<char, int> bc;
  std::vector<int> gs;
  std::vector<int> suffix;

  for (int i = 0; i < p_len; i++) {
    if (bc.count(pattern[i])) bc[pattern[i]] = i;
    else bc.insert(std::make_pair(pattern[i], i));
  }

  suffix.resize(p_len);
  gs.resize(p_len);

  suffix[p_len - 1] = p_len;
  for (int i = p_len - 2; i >= 0; i--) {
    int k = 0;
    while (pattern[i - k] == pattern[p_len - 1 - k]) {
      k++;
    }
    suffix[i] = k;
  }

  for (int i = 0; i < p_len; i++) gs[i] = p_len;

  for (int i = p_len - 2; i >= 0; i --) {
    if (suffix[i] == i + 1) {
      // debug
      for (int j = p_len - 1 - (i + 1); j >= 0; j--) {
        gs[j] = p_len - 1 - i;
      }
      break;
    }
  }

  for (int i = 0; i < p_len - 1; i++) {
    gs[p_len - 1 - suffix[i]] = p_len - 1 - i;
  }

  int k = 0;
  while (k <= s_len - p_len) {
    // 注意每次都是从末尾开始匹配的
    int m = 0;
    while (m < p_len &&
           str[k + p_len - 1 - m] == pattern[p_len - 1 - m]) {
      m++;
    }
    if (m == p_len) return &str[k];
    int foo = p_len - m;
    if (bc.count(str[k + p_len - 1 - m])) {
      foo = p_len - 1 - m - bc[str[k + p_len - 1 - m]];
    }
    int move = std::max(gs[p_len - 1 - m], foo);
    k += move;
    
    // LOG(INFO) << "foo:" << foo << " gs:" << gs[p_len - 1 - m] << " move:" << move
    //           << " k :" << k;
    // LOG(INFO) << "move to char:" << str[k];
  }
  return NULL;
}

}  // namespace algorithm

namespace algorithm {

/*
 * 标准写法，在已经知道的当前ｉ计算 i + 1的next
 *
 * 以后要使用这个，更漂亮
 * i  s  s  i  p
 * -1 0  0  -1
 *  此时i = 3 但j = ０,所以求解的ｐ的next是正确的
 * */

void Next(const char * pattern, std::vector<int> & next) {
  int size = strlen(pattern);
  next[0] = -1;
  if (size > 1) {
    int k = next[0];
    int i = 0;
    while (i < size) {  // 不能用for
      while (k >=0 && pattern[k] != pattern[i]){
        k = next[k];
      }
      k++;
      i++;
      if (i == size) break; // bug fixed
      if (pattern[i] == pattern[k]) next[i] = next[k];
      else next[i] = k;
    }
  }
  LOG(INFO) << "next:" << JoinVector(next);
}

const char * KMP(const char * str, const char * pattern) {
  int p_len = strlen(pattern);
  int s_len = strlen(str);
  if (p_len == 0) return str;

  std::vector<int> next;
  next.resize(p_len);

/*
 * 自己常用的这种求解next数组的方式不能使用优化方案
 * 比如 pattern :i  s  s  i  p
 *              -1  0  0  -1 
 * 由于ｉ的next为-1　求解ｐ的next时出错
 * */
/*
  next[0] = -1;
  if (p_len > 1) {
    next[1] = 0;
    for (int i = 2; i < p_len; i++) {
      int k = next[i - 1];
      // 在有优化的条件下第一次的str[k] 一定不等于 str[i - 1]
      // LOG(INFO) << pattern[k] << " " << pattern[i - 1];
      while (k >= 0 && pattern[k] != pattern[i - 1]) {
        k = next[k];
      }
      k++;
      next[i] = k;
    }
  }
*/
  // 标准求解next的方法
  Next(pattern, next);
  int k = 0;
  int j = 0;
  while (k <= s_len - p_len) {
    while (j < p_len && str[k] == pattern[j]) {
      j++;
      k++;
    }
    if (j == p_len) return &str[k - j];
    j = next[j];
    if (j < 0) {
      j++;
      k++;
    }
  }
  return NULL;
}

}

namespace twice {

//　最简单写法
void Next(const char * pattern, std::vector<int> &next) {
  int n = strlen(pattern);
  next.resize(n, 0);
  next[0] = -1;
  int i = 0;
  int t = next[i];  // 必须在while循环外定义，否则优化时，如果有个值是-1，就出错了
  while(i < n - 1) {  // note : n- 1  or add break
      while (t >= 0 && pattern[t] != pattern[i]) t = next[t];
      t++;
      i++;
      if (pattern[i] == pattern[t]) next[i] = next[t];
      else next[i] = t; 
   }
}

void NextOp(const char * pattern, std::vector<int> &next) {
  int n = strlen(pattern);
  next.resize(n, 0);
  next[0] = -1;
  int i = 0;
  while(i < n - 1) {
      int t = next[i];  // 不使用优化，t可以定义在循环内,因为不使用优化时只有next[0] == -1
                        // 其它的都不可能为-1
      while (t >= 0 && pattern[t] != pattern[i]) t = next[t];
      t++;
      i++;
      next[i] = t; 
   }
}

const char * KMP(const char * str, const char * pattern) {
    int len = strlen(str);
    int n = strlen(pattern);
    if (n == 0) return str;
    std::vector<int> next;
    Next(pattern, next);
    LOG(INFO) << JoinVector(next);
    int i = 0;
    int j = 0;
    while (i < len) {
        LOG(INFO) << "i:" << i << " j :" << j;
        if (str[i] == pattern[j]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
        if (j == -1) {
            j++;
            i++;
        }
        if (j == n) return &(str[i - n]);
    }
    // 如果str的长度小于等于pattern的长度，ｊ最后的大小可以表示出
    // str末尾与pattern开头有几个字符是匹配的
    return NULL;
}
}  // namespace twice

namespace third {
void Next(std::vector<int> & next, char * p) {
  int n = strlen(p);
  if (n == 0) return;
  next.resize(n, 0);
  int i = 0;
  next[i] = -1;
  int k = next[i];
  while (i < n) {
    while (k >= 0 && p[i] != p[k]) k = next[k];
    i++;
    k++;
    if (i == n) break;
    if (p[i] == p[k]) next[i] = next[k];
    else next[i] = k;
  }
}

char * KMP(char * s, char * p) {
  int s_len = strlen(s);
  int p_len = strlen(p);
  if (p_len == 0) return s;
  std::vector<int> next;
  Next(next, p);
  int i = 0;
  int j = 0;
  while (i < s_len) {
    if (s[i] == p[j]) {
      i++;
      j++;
    } else j = next[j];
    if (j == p_len) return s + i - p_len;
    if (j == -1) {
      i++;
      j++;
    }
  }
  return NULL;
}
}  // namespace third

namespace updated {
void Next(std::vector<int> & next, char * p) {
  int n = strlen(p);
  if (n == 0) return;
  next.resize(n, 0);
  int i = 0;
  next[i] = -1;
  int k = next[i];
  while (i < n) {
    while (k >= 0 && p[i] != p[k]) k = next[k];
    i++;
    k++;
    if (i == n) break;
    if (p[i] == p[k]) next[i] = next[k];
    else next[i] = k;
  }
}

int KMP(char * s, char * p) {
  int s_len = strlen(s);
  int p_len = strlen(p);
  if (p_len == 0) return 0;
  if (s_len == 0) return -1;
  std::vector<int> next;
  Next(next, p);
  int i = 0;
  int j = 0;
  while (i < s_len) {
    if (s[i] == p[j]) {
      i++;
      j++;
    } else j = next[j];
    if (j == p_len) return i - p_len;
    if (j == -1) {
      i++;
      j++;
    }
  }
  return -1;
}

}  // namespcae update

namespace mystyle {
void Next(std::vector<int> & next, const char * p) {
  int n = strlen(p);
  if (n == 0) return;
  next.resize(n, 0);
  next[0] = -1;
  for (int i = 1; i < n; i++) {
    int t = next[i - 1];
    while (t >= 0 && p[i - 1] != p[t]) t = next[t];
    t++;
    next[i] = t;
    // if (p[i] == p[t]) next[i] = next[t];
    // if t is local can not add this code, becase i - 1 loop need use the
    // origin value
  }
}

// this is the optimize version
void NextOp(std::vector<int> & next, const char * p) {
  int n = strlen(p);
  if (n == 0) return;
  next.resize(n, 0);
  next[0] = -1;
  int t = next[0];
  for (int i = 1; i < n; i++) {
    while (t >= 0 && p[i - 1] != p[t]) t = next[t];
    t++;
    next[i] = t;
    if (p[i] == p[t]) next[i] = next[t];
  }
}

int KMP(const char * s, const char * p) {
  int s_len = strlen(s);
  int p_len = strlen(p);
  if (p_len == 0) return 0;
  if (s_len == 0) return -1;
  std::vector<int> next;
  Next(next, p);
  LOG(INFO) << JoinVector(next);
  int i = 0;
  int j = 0;
  while (i < s_len) {
    if (s[i] == p[j]) {
      i++;
      j++;
    } else j = next[j];
    if (j == p_len) return i - p_len;
    if (j == -1) {
      i++;
      j++;
    }
  }
  return -1;
}
}  // namespace mystyle


namespace DP {
std::vector<int> Next(const std::string & pattern) {
    int n = pattern.size();
    if (n == 0) return std::vector<int>();
    
    std::vector<int> dp(n, 0);
    for (int i = 0; i < n; i++) {
        if (i == 0) dp[i] = -1;
        else {
            int t = dp[i - 1];
            while (t >= 0 && pattern[t] != pattern[i - 1]) t = dp[t];
            dp[i] = t + 1;
        }
    }
    return dp;
}

int KMP(const std::string & str, const std::string & pattern) {
    if (pattern.size() == 0) return 0;
    std::vector<int> next = Next(pattern);
    
    int i = 0;
    int j = 0;
    while (i < str.size()) {
        if (str[i] == pattern[j]) {
            i++;
            j++;
        } else j = next[j];
        
        if (j < 0) {
            i++;
            j++;
        }
        if (j == pattern.size()) return i - j;
    }
    return -1;
}
}  // namespace DP
using namespace algorithm;

int main(int argc, char** argv) {
  std::string str = "mississippi";
  std::string pattern = "issip";
  /*
  if (BM(str.c_str(), pattern.c_str()) != NULL) {
    LOG(INFO) << "find at " << BM(str.c_str(), pattern.c_str());
  } else {
    LOG(INFO) << "not find";
  }
  */
  KMP(str.c_str(), pattern.c_str());
  if (mystyle::KMP(str.c_str(), pattern.c_str()) != -1) {
    LOG(INFO) << "KMP find at " << mystyle::KMP(str.c_str(), pattern.c_str());
  } else {
    LOG(INFO) << "KMP not find";
  }
  return 0;
}
