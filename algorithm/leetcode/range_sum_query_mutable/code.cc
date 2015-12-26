// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-26 23:32:18
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
struct Node {
  int sum;
  Node * left;
  Node * right;
  Node() : sum(0), left(NULL), right(NULL) {}
};

class NumArray {
 public:
  NumArray(std::vector<int> &nums) {
    size = nums.size();
    root = NULL;
    if (size > 0) {
      Build(nums, 0, nums.size() - 1, &root);
    }
  }

  int Build(std::vector<int> & nums, int b, int e, Node ** father) {
    Node * cur = new Node();
    if (b == e) {
      cur->sum = nums[b];
    } else {
      int mid = b + (e - b) / 2;
      cur->sum = Build(nums, b, mid, &(cur->left)) +
                 Build(nums, mid + 1, e, &(cur->right));
    }
    *father = cur;
    return cur->sum;
  }

  void update(int i, int val) {
    update(i, val, 0, size - 1, root);
  }

  int update(int i, int val, int b, int e, Node * root) {
    if (b == i && e == i) {
      int tmp = root->sum;
      root->sum = val;
      return tmp;
    } else {
      int mid = b + (e - b) / 2;
      int tmp = 0;
      if (i <= mid) {
        tmp = update(i, val, b, mid, root->left);
      } else {
        tmp = update(i, val, mid + 1, e, root->right);
      }
      root->sum -= tmp;
      root->sum += val;
      return tmp;
    }
  }

  int sumRange(int i, int j, int b, int e, Node * root) {
    if (i == b && j == e) {
      return root->sum;
    } else {
      int mid = b + (e - b) / 2;
      if (i > mid) return sumRange(i, j, mid + 1, e, root->right);
      else if (j <= mid) return sumRange(i, j, b, mid, root->left);
      else {
        return sumRange(i, mid, b, mid, root->left) +
               sumRange(mid + 1, j, mid + 1, e, root->right);
      }
    }
  }

  int sumRange(int i, int j) {
    return sumRange(i, j, 0, size - 1, root);
  }
  Node * root;
  int size;
};

}  // namespace algorithm


using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(3);
  vec.push_back(5);

  NumArray foo(vec);
  LOG(INFO) << foo.sumRange(0, 2);
  foo.update(1, -1);
  LOG(INFO) << foo.sumRange(0, 2);
  return 0;
}
