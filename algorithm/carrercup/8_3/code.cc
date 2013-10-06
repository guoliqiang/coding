// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 16:09:36
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {
void Trace(std::vector<int> & v, int n, std::vector<int> & path) {
  if (n == v.size()) {
    LOG(INFO) << JoinVector(path);
  } else {
    Trace(v, n + 1, path);
    path.push_back(v[n]);
    Trace(v, n + 1, path);
    path.pop_back();
  }
}

// 另一种写法，可以追溯路径

void Trace2(std::vector<int> & v, int n, std::vector<int> & path) {
  if (n == v.size()) {
    for (int i = 0; i < path.size(); i++) {
      if (path[i]) std::cout << v[i] << " ";
    }
    std::cout << std::endl;
  } else {
    for (int i = 0; i < 2; i++) {
      path[n] = i;
      Trace(v, n + 1, path);
    }
  }
}

void Trace(std::vector<int> & v) {
  std::vector<int> path;
  Trace(v, 0, path);
}

// 这个思路也挺有意思，但对ｎ的大小有限制
/*
 * 对于一个集合，它的子集一共有2n 个(包括空集和它本身)。它的任何一个子集，
 * 我们都可以理解为这个集合本身的每个元素是否出现而形成的一个序列。比如说，
 * 对于集合{1, 2, 3}，空集表示一个元素都没出现，对应{0, 0, 0}； 子集{1, 3}，
 * 表示元素2没出现(用0表示)，1，3出现了(用1表示)，所以它对应 {1, 0, 1}。这
 * 样一来，我们发现，{1, 2, 3}的所有子集可以用二进制数000到111 的8个数来指
 * 示。泛化一下，如果一个集合有n个元素，那么它可以用0到2n -1 总共2n 个数的
 * 二进制形式来指示。每次我们只需要检查某个二进制数的哪一位为1， 就把对应的
 * 元素加入到这个子集就OK
 * */

void Trace3(std::vector<int> & v) {
  int n = 1 << v.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if ((i & 1 << j) != 0) std::cout << v[j] << " ";
    }
    std::cout << std::endl;
  }
}

};  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(0);
  v.push_back(1);
  Trace(v);
  Trace3(v);
  return 0;
}
