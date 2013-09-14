// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-11 11:26:57
// File  : p25.h
// Brief :

#ifndef  __P25_H_
#define  __P25_H_

#include "base/public/logging.h"
#include <math.h>
#include <string>

namespace algorithm {

int get_number(std::string & str, int i) {
  int rs = 0;
  for (int j = i; j < str.size(); j++) {
    rs = rs * 10 + (str[j] - '0');
  }
  return rs;
}

int get_num(std::string & str, int b, int e) {
  int rs = 0;
  for (int j = b; j <= e; j++) {
    rs = rs * 10 + (str[j] - '0');
  }
  return rs;
}

int one_num_sub(std::string & str, int b) {
  if (b >= str.size()) return 0;
  if (b == str.size() - 1) {
    if (str[b] != '0') return 1;
  }
  if (str[b] == '0') {
    return one_num_sub(str, b + 1);
  }
  if (str[b] == '1') {
    return get_number(str, b + 1) + 1 + one_num_sub(str, b + 1);
  }
  return pow(10, str.size() - 1 - b) + one_num_sub(str, b + 1);
}

int one_num(int i) {
  std::string foo;
  while (i) {
    char bar = '0' + i % 10;
    foo = bar + foo;
    i /= 10;
  }
  LOG(INFO) << foo;
  return one_num_sub(foo, 0);
}

// above is wrong
// http://zhedahht.blog.163.com/blog/static/25411174200732494452636/

int one_num_v2(std::string & str) {
  int rs = 0;
  for (int i = 0; i < str.size(); i++) {
    if (str[i] != '0') {
      if (i == 0) rs += pow(10, str.size() - 1 - i);
      else rs += (get_num(str, 0, i - 1) + 1)* pow(10, str.size() - 1 - i);
    } else {  // i != 0
      rs += get_num(str, 0, i - 1) * pow(10, str.size() - 1 - i);
    }
  }
  return rs;
}

int one_num_v2(int i) {
  std::string foo;
  while (i) {
    char bar = '0' + i % 10;
    foo = bar + foo;
    i /= 10;
  }
  LOG(INFO) << foo;
  return one_num_v2(foo);
}

}  // namespace algorithm

#endif  // __P25_H_
