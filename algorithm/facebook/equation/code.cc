// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-31 14:29:23
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"


namespace algorithm {

std::string ToString(const int k) {
  char t[10];
  sprintf(t, "%d", k);
  return std::string(t);
}

void Trace(const std::vector<int> & v, int cur, int t, int k,
           std::string path, std::set<std::string> & rs) {
  if (k == v.size()) {
    if (cur == t) rs.insert(path);
  } else {
    for (int i = 0; i < 4; i++) {
      if (i == 0) Trace(v, cur + v[k], t, k + 1, path + "+" + ToString(v[k]), rs);
      else if (i == 1) Trace(v, cur - v[k], t, k + 1, path + "-" + ToString(v[k]), rs);
      else if (i == 2) Trace(v, cur * v[k], t, k + 1, path + "*" + ToString(v[k]), rs);
      else Trace(v, cur / v[k], t, k + 1, path + "/" + ToString(v[k]), rs);
    }
  }
}

void Permutation(std::vector<int> & v, std::set<std::vector<int> > & rs, int k) {
  if (k == v.size()) {
    rs.insert(v);
  } else {
    for (int i = k; i < v.size(); i++) {
      std::swap(v[k], v[i]);
      Permutation(v, rs, k + 1);
      std::swap(v[k], v[i]);
    }
  }
}

std::vector<std::string> Trace(std::vector<int> & v, int t) {
  if (v.size() == 0) return std::vector<std::string>();
  std::set<std::vector<int> > per;
  Permutation(v, per, 0);
  std::set<std::string> rs;
  for (std::set<std::vector<int> >::iterator i = per.begin(); i != per.end(); i++) {
    const std::vector<int> & vec = *i;
    std::string path = ToString(vec[0]);
    int cur = vec[0];
    Trace(vec, cur, t, 1, path, rs);
  }
  return std::vector<std::string>(rs.begin(), rs.end());
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(5);
  v.push_back(5);
  std::vector<std::string> rs = Trace(v, 2);
  for (int i = 0; i < rs.size(); i++) LOG(INFO) << rs[i];
  return 0;
}
