// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 00:20:34
// File  : code.cc
// Brief :

#include "base/public/logging.h"

// TODO(guoliqiang) more readability

    int atoi(const char *str) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
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
}  // namespace twice

int main(int argc, char** argv) {
  std::string str = "    10522545459";
  LOG(INFO) << atoi(str.c_str());
  return 0;
}
