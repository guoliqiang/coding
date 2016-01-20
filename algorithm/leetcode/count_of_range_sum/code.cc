// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-10 15:51:26
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  int b;
  int e;
  long long int min;
  long long int max;
  Node * left;
  Node * right;
};

std::pair<long long int, long long int> Build(Node * & root, int b, int e, std::vector<long long int> & vec) {
  root = new Node();
  root->b = b;
  root->e = e;

  if (b == e) {
    root->min = vec[b];
    root->max = vec[e];
  } else {
    int mid = b + (e - b) / 2;
    std::pair<long long int, long long int> t1 = Build(root->left, b, mid, vec);
    std::pair<long long int, long long int> t2 = Build(root->right, mid + 1, e, vec);
    root->min = std::min(t1.first, t2.first);
    root->max = std::max(t1.second, t2.second);
  }
  return std::make_pair(root->min, root->max);
}

int Query(Node * root, int b, int e, long long int lower, long long int upper) {
  // LOG(INFO) << root->b << "~" << root->e << " " << b << "-" << e;
  if (b > e) return 0;
  if (root->e < b || root->b > e) return 0;
  if (root->max < lower || root->min > upper) return 0;
  else if (root->b >= b && root->e <= e) {
    if (root->min >= lower && root->max <= upper) {
      return root->e - root->b + 1;
    } else {
      return Query(root->left, b, e, lower, upper) + Query(root->right, b, e, lower, upper);
    }
  } else if (root->b <= b && root->e >= e) {
    return Query(root->left, b, e, lower, upper) + Query(root->right, b, e, lower, upper);
  }else if (b >= root->b && b <= root->e) {
    return Query(root, b, root->e, lower, upper);
  } else {
    return Query(root, root->b, e, lower, upper);
  }
}

int CountRangeSum(std::vector<int>& nums, int lower, int upper) {
  int n = nums.size();
  if (n == 0) return 0;

  std::vector<long long int> sum(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) sum[i] = nums[i];
    else sum[i] = sum[i - 1] + nums[i];
  }
  Node * root = NULL;
  sum.insert(sum.begin(), 0);
  Build(root, 0, n, sum);
  int ans = 0;
  for (int i = 0; i < n; i++) {
    long long int l = sum[i + 1] - upper;
    long long int u = sum[i + 1] - lower;
    int t = Query(root, 0, i, l, u);
    ans += t;
  }
  return ans;
}
}  // namespace algorithm

namespace msort {
void Sort(std::vector<long long int> & vec, int b, int e, int lower, int upper, int & ans) {
  if (b >= e) return;
  int mid = b + (e - b) / 2;
  Sort(vec, b, mid, lower, upper, ans);
  Sort(vec, mid + 1, e, lower, upper, ans);

  std::vector<long long int> t;
  int i = b;
  int j = mid + 1;
  int begin = mid + 1;
  int end = mid + 1;

  while (i <= mid || j <= e) {
    if (j > e || (i <= mid && vec[i] < vec[j])) {
      while (begin <= e && vec[begin] < vec[i] + lower) begin++;
      while (end <= e && vec[end] <= vec[i] + upper) end++;
      ans += (end - begin);

      t.push_back(vec[i]);
      i++;
    } else {
      t.push_back(vec[j]);
      j++;
    }
  }
  for (int i = 0; i < t.size(); i++) {
    vec[b + i] = t[i];
  }
}

int CountRangeSum(std::vector<int>& nums, int lower, int upper) {
  int n = nums.size();
  if (n == 0) return 0;

  int ans = 0;
  std::vector<long long int> sum(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) sum[i] = nums[i];
    else sum[i] = sum[i - 1] + nums[i];
    if (sum[i] >= lower && sum[i] <= upper) ans++;
  }

  Sort(sum, 0, n - 1, lower, upper, ans);
  return ans;
}
}  // namespace msort

namespace NB {
struct TreeNode {
  long long int b;
  long long int e;
  int cnt;
  TreeNode * left;
  TreeNode * right;
};

void Build(std::vector<long long int> & sum, int b, int e, TreeNode * & root) {
  if (b > e) return;
  root = new TreeNode();
  root->b = sum[b];
  root->e = sum[e];
  if (b < e) {
    int mid = b + (e - b) / 2;
    Build(sum, b, mid, root->left);
    Build(sum, mid + 1, e, root->right);
  }
}

void Update(TreeNode * root, long long int v) {
  if (root == NULL || root->b > v || root->e < v) return;
  root->cnt++;
  if (root->b == v && root->e == v) return;
  else {
    Update(root->left, v);
    Update(root->right, v);
  }
}

int Query(TreeNode * root, long long int b, long long int e) {
  if (root == NULL || root->b > e || root->e < b) return 0;
  if (root->b >= b && root->e <= e) return root->cnt;
  else {
    return Query(root->left, b, e) + Query(root->right, b, e);
  }
}

int CountRangeSum(std::vector<int>& nums, int lower, int upper) {
  int n = nums.size();
  if (n == 0) return 0;
  int ans = 0;
  long long int sum = 0;
  std::vector<long long int> vec(n, 0);
  for (int i = 0; i < n; i++) {
    if (i == 0) vec[i] = nums[i];
    else {
      vec[i] = vec[i - 1] + nums[i];
    }
    sum += nums[i];
  }
  std::sort(vec.begin(), vec.end());
  std::vector<long long int>::iterator it = std::unique(vec.begin(), vec.end());
  vec.resize(it - vec.begin());

  TreeNode * root = NULL;
  Build(vec, 0, vec.size() - 1, root);
  for (int i = n - 1; i >= 0; i--) {
    Update(root, sum);
    sum -= nums[i];
    ans += Query(root, sum + lower, sum + upper);
  }
  return ans;
}
}  // namespace NB

using namespace algorithm;
int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(-2147483647);
  vec.push_back(0);
  vec.push_back(-2147483647);
  vec.push_back(2147483647);
  LOG(INFO) << CountRangeSum(vec, -564, 3864);
  LOG(INFO) << msort::CountRangeSum(vec, -564, 3864);
  LOG(INFO) << NB::CountRangeSum(vec, -564, 3864);
  return 0;
}
