// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-04 23:16:33
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Check(std::vector<int> & v, std::vector<std::vector<int> > & matrix) {
  int n = v.size();
  int k = n - 1;
  for (int i = 0; i < n - 1; i++) {
    if (matrix[v[i]][v[k]] == 1) {
      return false;
    }
  }
  return true;
}

void Trace(std::vector<int> & num, std::vector<int> & path, std::vector<std::vector<int> > & matrix, int k, std::vector<std::vector<int> > & rs) {
  if (k == num.size()) {
    std::vector<int> tmp;
    for (int i = 0; i < path.size(); i++) tmp.push_back(num[path[i]]);
    if (tmp.size() > 0) rs.push_back(tmp);
  } else {
    Trace(num, path, matrix, k + 1, rs);
    path.push_back(k);
    if (Check(path, matrix)) {
      Trace(num, path, matrix, k + 1, rs);
    } else LOG(INFO) << JoinVector(path);
    path.pop_back();
  }
}

std::vector<std::vector<int> > ExclusionSubset(std::vector<int> & num, std::vector<std::vector<int> > & rules) {
  int n = num.size();
  std::vector<std::vector<int> > matrix(n, std::vector<int>(n, 0));
  std::map<int, int> tmap;
  for (int i = 0; i < num.size(); i++) tmap[num[i]] = i;
  for (int i = 0; i < rules.size(); i++) {
    for (int j = 0; j < rules[i].size(); j++) {
      for (int k = j + 1; k < rules[i].size(); k++) {
        matrix[tmap[rules[i][j]]][tmap[rules[i][k]]] = 1;
        matrix[tmap[rules[i][k]]][tmap[rules[i][j]]] = 1;
      }
    }
  }
  LOG(INFO) << JoinMatrix(&matrix);
  std::vector<int> path;
  std::vector<std::vector<int> > rs;
  Trace(num, path, matrix, 0, rs);
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(2);  // 0
  num.push_back(3);  // 1
  num.push_back(4);  // 2
  num.push_back(5);  // 3
  std::vector<std::vector<int> > rules;
  std::vector<int> r;
  r.push_back(2);
  r.push_back(3);
  rules.push_back(r);
  r.clear();
  r.push_back(4);
  r.push_back(5);
  rules.push_back(r);
  std::vector<std::vector<int> > rs = ExclusionSubset(num, rules);
  LOG(INFO) << JoinMatrix(&rs);
  return 0;
}
