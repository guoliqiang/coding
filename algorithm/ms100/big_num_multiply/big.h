// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 11:42:22
// File  : big.h
// Brief :

#ifndef  __BIG_H_
#define  __BIG_H_
#include "base/public/logging.h"
#include <string>

namespace algorithm {

std::string Big(std::string & a, std::string &b) {
  std::string rs;
  rs.resize(a.size() + b.size() + 1);
  for (int i = 0; i< rs.size(); i++) rs[i] = '0';
  char * ptr = &rs[rs.size() - 1];
  for (int i = a.size() - 1; i >= 0; i--) {
    char * foo = ptr - (a.size() - 1 - i);
    int carry = 0;
    for (int j = b.size() - 1; j >= 0; j--) {
       int tmp = (*foo - '0') + (a[i] - '0') * (b[j] - '0') + carry;
       *foo = tmp % 10 + '0';
       // LOG(INFO) << a[i] << " " << b[j] << " " << *foo << " " << carry << " " << tmp;
       // LOG(INFO) << *foo;
       carry = tmp / 10;
       foo--;
    }
    while (carry) {
      *foo = carry%10 + '0';
      carry /= 10;
      foo--;
    }
  }
  int i = 0;
  while (rs[i] == '0') i++;
  rs = rs.substr(i);
  return rs;
}

}  // namespace algorithm


#endif  // __BIG_H_
