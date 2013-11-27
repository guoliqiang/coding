// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 00:20:34
// File  : code.cc
// Brief :

/*
Implement atoi to convert a string to an integer.
Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
Notes: It is intended for this problem to be specified vaguely (ie, no given input specs). You are responsible to gather all the input requirements up front.

spoilers alert... click to show requirements for atoi.
Requirements for atoi:
The function first discards as many whitespace characters as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many numerical digits as possible, and interprets them as a numerical value.
The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.
If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed.
If no valid conversion could be performed, a zero value is returned. If the correct value is out of the range of representable values, INT_MAX (2147483647) or INT_MIN (-2147483648) is returned.

*/

#include "base/public/logging.h"

// TODO(guoliqiang) more readability
int atoi(const char *str) {
  bool neg = false;
  bool flag = true;
  bool add_flag = false;
  int len = strlen(str);
  int rs = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] == '-' && !neg && rs == 0) {
      neg = true;
      flag = false;
    } else {
      if (str[i] >= '0' && str[i] <= '9') {
        int tmp = rs;
        rs *= 10;
        if (rs / 10 != tmp) {
          rs = -1;
          break;
        }
        rs += str[i] - '0';
        flag = false;
        if (rs < 0) break;
      } else {
        if (str[i] == '+' && !add_flag) {
          flag = false;
          add_flag = true;
          continue;
        }
        if ((str[i] != '+') && flag && (str[i] == ' ' || str[i] == '\t')) continue;
        break;
      }
    }
  }
  if (!neg && rs < 0) return (int)(0x7fffffff);
  if (neg && rs < 0) return (int)(0x80000000);
  return neg ? -rs : rs;
}


namespace twice {
// 注意判断越界的情况有两种
// 1. 加着加着t变为负数
// 2. (t* 10) / 10 != t

int Atoi(const char * ptr) {
  while (*ptr != '\0' && (*ptr == ' ' || *ptr == '\t')) ptr++;
  bool minus = false;
  if (*ptr == '\0') return 0;
  else if (*ptr == '-') {
      minus = true;
      ptr++;
  } else if (*ptr == '+') {
      ptr++;
  }
  
  int t = 0;
  while (*ptr != '\0') {
      if (*ptr >= '0' && *ptr <= '9') { 
        int foo = t;
        t *= 10;
        if (t / 10 != foo) {
            t = -1;
            break;
        }
        t += (*ptr - '0');
      } else break;
      if (t < 0) break;
      ptr++;
  }
  if (t < 0) return minus ? INT_MIN : INT_MAX;
  return minus ? -t : t;
}
}  // namespace twicea

// 使用long long 判断有没有越界

namespace third {

int Atoi(const char * str) {
  long long rs = 0;
  bool minus = false;
  while (*str != '\0' && (*str == ' ' || *str == '\t')) str++;
  if (*str == '\0') return 0;
  if (*str == '-')  {
    minus = true;
    str++;
  } else if (*str == '+') {
    str++;
  }
  while (*str != '\n') {
    if (*str >= '0' && *str <= '9') {
      rs = rs * 10 + (*str - '0');
    } else break;
    str++;
    if (rs > INT_MAX) break;
  }
  if (rs > INT_MAX) {
    return minus ? INT_MIN : INT_MAX;
  } else {
    return minus ? -rs : rs;
  }
}
}  // namespace third

int main(int argc, char** argv) {
  std::string str = "    10522545459";
  LOG(INFO) << atoi(str.c_str());
  return 0;
}
