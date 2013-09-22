// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-21 22:41:43
// File  : code.cc
// Brief :

#include <map>
#include <set>
#include <algorithm>
#include "base/public/logging.h"
#include "base/public/string_util.h"

// 可以直接用std::set<vector<int>>
//
// official solution 很精简
// 分类：含有第i个并且i是最小的那个，因此可以用 j = i + 1
/*
set<vector<int> > find_triplets(vector<int> arr) {
  sort(arr.begin(), arr.end());
  set<vector<int> > triplets;
  vector<int> triplet(3);
  int n = arr.size();
  for (int i = 0;i < n; i++) {
    int j = i + 1;
    int k = n - 1;
    while (j < k) {
      int sum_two = arr[i] + arr[j];
      if (sum_two + arr[k] < 0) {
        j++;
      } else if (sum_two + arr[k] > 0) {
        k--;
      } else {
        triplet[0] = arr[i];
        triplet[1] = arr[j];
        triplet[2] = arr[k];
        triplets.insert(triplet);
        j++;
        k--;
      }
    }
  }
  return triplets;
}
*/
class Node {
 public:
  Node() {
    a = 0;
    b = 0;
    c = 0;
  }

  Node(int a_i, int b_i, int c_i) {
    a = std::min(a_i, b_i);
    if (c_i < a) {
      a = c_i;
      b = std::min(a_i, b_i);
      c = std::max(a_i, b_i);
    } else {
      b = std::max(a_i, b_i);
      if (b > c_i) {
        c = b;
        b = c_i;
      } else {
        c = c_i;
      }
    }
  }
  // http://www.cnblogs.com/littleant/archive/2012/08/01/2618846.html
  // 非const 引用不能指向临时对象,因为临时对象是不能被修改的。
  // 函数属性注意加const
  bool operator < (const Node & t) const {
    if (a < t.a) return true;
    if (a > t.a) return false;

    if (b < t.b) return true;
    if (b > t.b) return false;

    if (c < t.c) return true;
    return false;
  }

  int a;
  int b;
  int c;
};

void Find(std::vector<int> & v, int t, std::set<Node> * rs) {
  int i = 0;
  int j = v.size() - 1;
  while (i <= j) {
    if (j >= 0 && v[i] + v[j] > -v[t]) {
      j--;
    } else if (i < v.size() && v[i] + v[j] < - v[t]) {
      i++;
    } else if (i < j && v[i] + v[j] == -v[t] && i != t && j != t) {
      rs->insert(Node(v[t], v[i], v[j]));
      i++;
    } else {
      break;
    }
    // LOG(INFO) << i << " " << j << " " << v[i] << " " << v[j] << " " << -v[t];
  }
}

std::vector<std::vector<int> > tsum(std::vector<int> & v) {
  std::vector<std::vector<int> > rs;
  std::set<Node> foo;
  std::sort(v.begin(), v.end());
  for (int i = 0; i < v.size(); i++) {
    Find(v, i, &foo);
  }
  LOG(INFO) << "size:" << foo.size();
  for (std::set<Node>::iterator i = foo.begin(); i != foo.end(); i++) {
    std::vector<int> tmp;
    tmp.push_back(i->a);
    tmp.push_back(i->b);
    tmp.push_back(i->c);
    rs.push_back(tmp);
  }
  return rs;
}

int main(int argc, char** argv) {
  std::vector<int> foo;
  foo.push_back(-1);
  foo.push_back(0);
  foo.push_back(1);
  foo.push_back(2);
  foo.push_back(-1);
  foo.push_back(-4);
  std::vector<std::vector<int> > rs = tsum(foo);
  for (int i = 0; i < rs.size(); i++) {
    LOG(INFO) << JoinVector(rs[i]);
  }
  return 0;
}
