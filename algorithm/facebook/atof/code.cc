// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 12:20:47
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
int atoi(std::string s) {
  bool minus = false;
  int b = 0;
  if (s[b] == '-') {
    minus = true;
    b++;
  } else if (s[b] == '+') b++;
  int rs = 0;
  for (int i = b; i < s.size(); i++) {
    rs = rs * 10 + s[i] - '0';
  }
  return minus ? -rs : rs;
}

double atof(std::string str) {
  int e_i = 0;
  while (e_i < str.size() && str[e_i] != 'e') e_i++;
  double last = 1;
  if (e_i != str.size()) {
    last = pow(10, atoi(str.substr(e_i + 1)));
    str = str.substr(0, e_i);
  }
  int d_i = 0;
  while (d_i < str.size() && str[d_i] != '.') d_i++;
  if (d_i == str.size()) {
    return atoi(str) * last;
  } else {
    double first = atoi(str.substr(0, d_i));
    str = str.substr(d_i + 1);
    double mid = atoi(str) * pow(10, -str.size());
    return first * mid * last;
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  LOG(INFO) << atof("+3.5e-2");
  LOG(INFO) << atof("1e1");
  return 0;
}
