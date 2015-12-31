// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-29 11:22:13
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

std::vector<int> CountSmaller(std::vector<int>& nums) {
  int n = nums.size();
  std::multiset<int> idx;
  std::vector<int> ans(n, 0);
  for (int i = n - 1; i >= 0; i--) {
    std::multiset<int>::iterator it = idx.lower_bound(nums[i]);
    if (it == idx.end()) {
      ans[i] = idx.size();
    } else {
      int cnt = 0;
      while (it != idx.begin()) {
        if (*it != nums[i]) cnt++;
        it--;
      }
      ans[i] = cnt;
    }
    idx.insert(nums[i]);
  }
  return ans;
}
}  // namespace algorithm

namespace bound {

std::vector<int> CountSmaller(std::vector<int>& nums) {
  int n = nums.size();
  std::vector<int> tmp;
  tmp.reserve(n);
  std::vector<int> ans(n, 0);

  for (int i = n - 1; i >= 0; i--) {
    std::vector<int>::iterator it = std::lower_bound(tmp.begin(), tmp.end(), nums[i]);
    ans[i] = it - tmp.begin();
    tmp.insert(it, nums[i]);
  }
  return ans;
}
}  // namespace bound

namespace NB {
struct Node {
  int val;
  int less;
  int dup;
  Node * left;
  Node * right;
  Node(int v) : val(v), less(0), dup(1), left(NULL), right(NULL) {}
};

Node * Insert(int v, Node * root, int * less) {
  if (root == NULL) {
    return new Node(v);
  } else {
    if (root->val == v) {
      *less += root->less;
      root->dup++;
    } else if(root->val < v) {
      *less += (root->less + root->dup);
      root->right = Insert(v, root->right, less);
    } else {
      root->less++;
      root->left = Insert(v, root->left, less);
    }
    return root;
  }
}

std::vector<int> CountSmaller(std::vector<int>& nums) {
  int n = nums.size();
  Node * root = NULL;
  std::vector<int> ans(n, 0);

  for (int i = n - 1; i >= 0; i--) {
    int less = 0;
    root = Insert(nums[i], root, &less);
    ans[i] = less;
  }
  return ans;
}
}  // namespace NB

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(0);
  vec.push_back(1);
  vec.push_back(1);
  LOG(INFO) << JoinVector(CountSmaller(vec));
  LOG(INFO) << JoinVector(NB::CountSmaller(vec));
  LOG(INFO) << JoinVector(bound::CountSmaller(vec));
  return 0;
}
