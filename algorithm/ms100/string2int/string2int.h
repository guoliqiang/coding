// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 18:10:05
// File  : sting2int.h
// Brief :

#ifndef  __STING2INT_H_
#define  __STING2INT_H_

#include <limits>
#include "base/public/logging.h"

namespace algorithm {

bool String2Int(std::string & str, int * num) {
  long long rs = 0;
  bool ne = false;
  bool begin_num = false;
  
  for (int i = 0; i < str.size(); i++) {
    if (!begin_num && str[i] == '-') {
      ne = true;
      begin_num = true;
      continue;
    }

    if (!begin_num && str[i] == '+') {
      begin_num = true;
      continue;
    }

    if (begin_num) {
      if (str[i] < '0' || str[i] > '9') {
        LOG(INFO) << "ill char:" << str[i];
        return false;
      }
    }

    if (!begin_num) {
      if ((str[i] < '0' || str[i] > '9')&&
          str[i] != '-' &&
          str[i] != '+') {
        LOG(INFO) << "ill char:" << str[i];
        return false;
      }
      if (str[i] != '-' && str[i] != '+') {
        begin_num = true;
      } else {
       i--;
       continue;
      }
    }

    rs *= 10;
    rs += str[i] - '0';
    if (rs > std::numeric_limits<int>::max()) {
      LOG(INFO) << "over flow " << str;
      return false;
    }
  }

  *num = ne ? -1 * rs : rs;
  return true;
}
}  // namespace algorithm

#endif  //  __STING2INT_H_
