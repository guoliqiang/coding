// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-25 20:17:00
// File  : code.cc
// Brief :

/*
Given a collection of numbers, return all possible permutations.
For example,
[1,2,3] have the following permutations:
[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], and [3,2,1].

*/

/*
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 5/5 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 88 milli secs
 * Progress: 25/25 test cases passed.
 *
 * */

#include <vector>
#include "base/public/logging.h"
#include "base/public/string_util.h"

namespace algorithm {
// 其实就是DFS
void PermuteSub(std::vector<int> & num, int n, std::vector<std::vector<int> > * rs) {
  if (n >= num.size()) {
    rs->push_back(num);
  } else {
    for (int i = n; i < num.size(); i++) {
      std::swap(num[n], num[i]);
      PermuteSub(num, n + 1, rs);
      std::swap(num[n], num[i]);
    }
  }
}

std::vector<std::vector<int> > Permute(std::vector<int> & num) {
  std::vector<std::vector<int> > rs;
  PermuteSub(num, 0, &rs);
  return rs;
}
}  // namespace algorithm

namespace iter {
bool Next(std::vector<int> & num) {
  int n = num.size();
  int idx1 = n - 2;
  while (idx1 >= 0 && num[idx1] >= num[idx1 + 1]) idx1--;
  if (idx1 < 0) return false;

  int idx2 = n - 1;
  while (num[idx2] < num[idx1]) idx2--;
  std::swap(num[idx1], num[idx2]);

  int b = idx1 + 1;
  int e = n - 1;
  while (b < e) {
    std::swap(num[b], num[e]);
    b++;
    e--;
  }
  return true;
}

std::vector<std::vector<int> > Permute(std::vector<int> & num) {
  std::sort(num.begin(), num.end());
  std::vector<std::vector<int> > ans;
  do {
    ans.push_back(num);
  } while (Next(num));
  return ans;
}
}  // namespace iter

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> num;
  num.push_back(1);
  num.push_back(2);
  num.push_back(3);
  std::vector<std::vector<int> > foo = iter::Permute(num);
  for (int i = 0; i < foo.size(); i++) {
    LOG(INFO) << JoinVector(foo[i]);
  }
  return 0;
}
