// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-24 09:46:21
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

int Dis(const std::pair<int, int> & x) {
  return x.first * x.first + x.second * x.second;
}

void NearestK(std::vector<std::pair<int, int> > & vec, int k) {
  int n = vec.size();
  if (k > n) return;

  int b = 0;
  int e = n - 1;
  while (b <= e) {
    int idx = b - 1;
    for (int i = b; i < e; i++) {
      if (Dis(vec[i]) < Dis(vec[e])) {
        std::swap(vec[++idx], vec[i]);
      }
    }
    std::swap(vec[++idx], vec[e]);

    if (idx - b + 1 == k) break;
    else if (idx - b + 1 > k) e = idx - 1;
    else {
      k = k - (idx - b + 1);
      b = idx + 1;
    }
  }
}

}  // namespace algorithm

namespace algorithm2 {

int CmpX(const std::pair<int, int> & x, const std::pair<int, int> & y) {
  return x.first < y.first;
}

int CmpY(const std::pair<int, int> & x, const std::pair<int, int> & y) {
  return x.second < y.second;
}

int Dis(const std::pair<int, int> & x, const std::pair<int, int> & y) {
  return (x.first - y.first) * (x.first - y.first) +
         (x.second - y.second) * (x.second - y.second);
}

int Sub(std::vector<std::pair<int, int> > & vec, int b, int e) {
  if (b > e) return 0;
  if (b == e) return 0;
  if (b + 1 == e) return Dis(vec[b], vec[e]);
  else if (b + 2 == e) {
    int t = std::min(Dis(vec[b], vec[b + 1]), Dis(vec[b + 1], vec[b + 2]));
    return std::min(t, Dis(vec[b], vec[b + 2]));
  } else {
    int mid = b + (e - b) / 2;
    int l = Sub(vec, b, mid);
    int r = Sub(vec, mid + 1, e);
    int min = std::min(l, r);

    std::vector<std::pair<int, int> > t;
    for (int i = b; i <= e; i++) {
      if (fabs(vec[i].first - vec[mid].first) < min) {
        t.push_back(vec[i]);
      }
    }
    std::sort(t.begin(), t.end(), CmpY);
    for (int i = 0; i < t.size(); i++) {
      for (int j = i + 1; j < t.size(); j++) {
        if (vec[j].second - vec[i].second > min) break;
        if (Dis(vec[j], vec[i]) < min) min = Dis(vec[j], vec[i]);
      }
    }
    return min;
  }
}


int NearestTwo(std::vector<std::pair<int, int> > vec) {
  int n = vec.size();
  std::sort(vec.begin(), vec.end(), CmpX);
  return Sub(vec, 0, n - 1);
}

int NearestTwo2(std::vector<std::pair<int, int> > vec) {
  int min = INT_MAX;
  for (int i = 0; i < vec.size(); i++) {
    for (int j = i + 1; j < vec.size(); j++) {
      min = std::min(min, Dis(vec[i], vec[j]));
    }
  }
  return min;
}
}  // namespace algorithm2

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::pair<int, int> > vec;
  for (int i = 0; i < 10; i++) {
    vec.push_back(std::make_pair(i, 100 - i));
  }
  LOG(INFO) << JoinVectorP(vec);
  LOG(INFO) << "min=" << algorithm2::NearestTwo(vec);
  LOG(INFO) << "min=" << algorithm2::NearestTwo2(vec);

  NearestK(vec, 5);
  LOG(INFO) << JoinVectorP(vec);
  for (int i = 0; i < vec.size(); i++) {
    LOG(INFO) << vec[i].first << " " << vec[i].second << " " << Dis(vec[i]);
  }
  return 0;
}
