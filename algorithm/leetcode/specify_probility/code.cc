// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-02 15:17:35
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

char Random1(std::string & str, std::vector<double> & pro) {
  double sum = 0;
  for (int i = 0; i < pro.size(); i++) {
    sum += pro[i];
  }
  double t = double(rand()) / (RAND_MAX) * sum;
  for (int i = 0; i < pro.size(); i++) {
    if (t <= pro[i]) return str[i];
  }
  return '\0';
}

char Random2(std::string & str, std::vector<double> & pro) {
  int n = pro.size();
  std::vector<int> vec(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) vec[i] = pro[i];
    else vec[i] = vec[i - 1] + pro[i];
  }
  double t = double(rand()) / (RAND_MAX) * vec[n - 1];

  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int mid = b + (e - b) / 2;
    if (vec[mid] < t) b = mid + 1;
    else e = mid - 1;
  }
  return str[b];
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
