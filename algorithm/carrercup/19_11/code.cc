// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 15:36:34
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

// O(n * log(n))
// O(1)
std::vector<std::pair<int, int> > Find(std::vector<int> v, int k) {
  std::sort(v.begin(), v.end());
  std::vector<std::pair<int, int> >rs;
  int b = 0;
  int e = v.size() - 1;
  while (b < e) {
    if (v[b] + v[e] < k) b++;
    else if (v[b] + v[e] > k) e--;
    else {
      rs.push_back(std::make_pair(v[b], v[e]));
      b++;
    }
  }
  return rs;
}

// O(n)
// O(n)
std::vector<std::pair<int, int> > Find2(std::vector<int> &v, int k) {
  std::vector<std::pair<int, int> > rs;
  std::map<int, std::vector<int> > t;
  for (int i = 0; i < v.size(); i++) {
    if (!t.count(v[i])) t.insert(std::make_pair(v[i], std::vector<int>()));
    t[v[i]].push_back(i);
  }
  for (int i = 0; i < v.size(); i++) {
    int m = k - v[i];
    if (t.count(m)) {
      for (int j = 0; j < t[m].size(); j++)
        if (i < t[m][j]) rs.push_back(std::make_pair(v[i], v[t[m][j]]));
    }
  }
  return rs;
}
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  std::vector<std::pair<int, int> > rs;
  rs = Find(v, 7);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << " " << rs[i].second;
  }
  rs = Find2(v, 7);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << rs[i].first << " " << rs[i].second;
  }
  return 0;
}
