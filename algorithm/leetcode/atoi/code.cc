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


int main(int argc, char** argv) {
  std::string str = "    10522545459";
  LOG(INFO) << atoi(str.c_str());
  return 0;
}
