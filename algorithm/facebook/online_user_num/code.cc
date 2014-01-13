// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-01 21:08:07
// File  : code.cc
// Brief :

// A period of time where users login and logout, given a sets of login and logout time pairs,
// write a function that can show the number of users online at any given time.
// http://www.careercup.com/question?id=14797777

#include "base/public/common_ojhead.h"

namespace algorithm {
struct LogPeriod {
  int b;
  int e;
};

// O(log(n))
int Num(std::vector<LogPeriod> & v, int t) {
  std::vector<int> login;
  std::vector<int> logout;
  for (int i = 0; i < v.size(); i++) {
    login.push_back(v[i].b);
    logout.push_back(v[i].e);
  }
  std::sort(login.begin(), login.end());
  std::sort(logout.begin(), logout.end());
  int b = 0;
  int e = v.size() - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (login[mid] < t) b = mid + 1;
    else e = mid - 1;
  }
  if (b < v.size() && login[b] == t) b++;
  int b2 = 0;
  int e2 = v.size() - 1;
  while (b2 <= e2) {
    int mid = b2 + (e2 - b2) / 2;
    if (logout[mid] < t) b2 = mid + 1;
    else e2 = mid - 1;
  }
  if (b2 < v.size() && logout[b2] == t) b2++;
  return b - b2;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
