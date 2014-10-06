// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-16 00:44:42
// File  : quit_sort.h
// Brief :

#ifndef  __QUIT_SORT_H_
#define  __QUIT_SORT_H_

#include <string>
#include <vector>
#include <algorithm>
#include "stdlib.h"
#include "time.h"
#include "base/public/logging.h"
#include <stack>

namespace algorithm {

template <typename type>
void Qsort(std::vector<type> & v, int b, int e) {
  if (b >= e) return;
  type foo = v[e];
  int i = b;
  int j = e - 1;
  while (i < j) {
    while (i < e && v[i] < foo) i++;
    while (j > b && v[j] >= foo) j--;
    if (i < j) {
      std::swap(v[i], v[j]);
    }
  }
  std::swap(v[i], v[e]);
  Qsort(v, b, i - 1);
  Qsort(v, i + 1, e);
}

template <typename type>
void Qsort(std::vector<type> & v) {
  if (v.size() == 0) return;
  Qsort(v, 0, v.size() - 1);
}

template <typename type>
void Qsort2(std::vector<type> & v, int b, int e) {
  if (b >= e) return;
  type foo = v[e];
  int k = b - 1;
  for (int i = b; i < e; i++) {
    if (v[i] < foo) std::swap(v[++k], v[i]);
  }
  std::swap(v[++k], v[e]);
  Qsort2(v, b, k - 1);
  Qsort2(v, k + 1, e);
}

template <typename type>
void Qsort2(std::vector<type> & v) {
  if (v.size() == 0) return;
  Qsort2(v, 0, v.size() - 1);
}

template <typename type>
void QsortIte(std::vector<type> & v) {
  std::stack<std::pair<int, int> > stack;
  stack.push(std::make_pair(0, v.size() - 1));
  while(!stack.empty()) {
    std::pair<int, int> cur = stack.top();
    stack.pop();
    if (cur.first >= cur.second) continue;
    int k = cur.first - 1;
    int partion = v[cur.second];
    for (int i = cur.first; i < cur.second; i++) {
      if (v[i] < partion) std::swap(v[++k], v[i]);
    }
    std::swap(v[++k], v[cur.second]);
    stack.push(std::make_pair(cur.first, k - 1));
    stack.push(std::make_pair(k + 1, cur.second));
  }
}
}  // namespace algorithm

#endif  //  __QUIT_SORT_H_
