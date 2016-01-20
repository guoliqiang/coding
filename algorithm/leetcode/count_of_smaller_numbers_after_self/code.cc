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

namespace msort {
void Sort(std::vector<std::pair<int, int> > & nums, int b, int e, std::map<int, int> & cnt) {
  if (b >= e) return;
  int mid = b + (e - b) / 2;
  Sort(nums, b, mid, cnt);
  Sort(nums, mid + 1, e, cnt);
  int i = b;
  int j = mid + 1;
  std::vector<std::pair<int, int> > t;
  while (i <= mid || j <= e) {
    if (j > e || (i <= mid && nums[i].first < nums[j].first)) {
      t.push_back(nums[i]);
      cnt[nums[i].second] += j - (mid + 1);
      i++;
    } else {
      t.push_back(nums[j]);
      j++;
    }
  }
  LOG(INFO) << "size=" << t.size();
  for (int i = 0; i < t.size(); i++) {
    nums[b + i] = t[i];
  }
}

std::vector<int> CountSmaller(std::vector<int>& nums) {
  std::vector<int> ans;
  int n = nums.size();
  if (n == 0) return ans;

  std::vector<std::pair<int, int> > vec;
  std::map<int, int> cnt;
  for (int i = 0; i < n; i++) {
    cnt[i] = 0;
    vec.push_back(std::make_pair(nums[i], i));
  }
  Sort(vec, 0, n - 1, cnt);
  for (std::map<int, int>::iterator i = cnt.begin(); i != cnt.end(); i++) {
    ans.push_back(i->second);
  }
  return ans;

}
}  // namespace msort

namespace stree {
struct TreeNode {
  int max;
  int min;
  int b;
  int e;
  TreeNode * left;
  TreeNode * right;
};

std::pair<int, int> Build(TreeNode * & root, int b, int e, std::vector<int> & num) {
  if (b > e) return std::make_pair(0, 0);

  root = new TreeNode();
  root->b = b;
  root->e = e;
  if (b == e) {
    root->max = num[b];
    root->min = num[b];
  } else {
    int mid = b + (e - b) / 2;
    std::pair<int, int> t1 = Build(root->left, b, mid, num);
    std::pair<int, int> t2 = Build(root->right, mid + 1, e, num);
    root->max = std::max(t1.first, t2.first);
    root->min = std::min(t1.second, t2.second);
  }
  return std::make_pair(root->max, root->min);
}

int Query(TreeNode * root, int b, int e, int k) {
  if (root == NULL || b > e || root->b > e || root->e < b || root->min > k) return 0;
  // LOG(INFO) << root->b << "/" << root->e << " " << b << "~" << e << " " << root->min << " to " << root->max;
  if (root->b == b && root->e == e) {
    if (root->max < k) return root->e - root->b + 1;
    else return Query(root->left, b, e, k) + Query(root->right, b, e, k);
  } if (root->b <= b && root->e >= e) {
    return Query(root->left, b, e, k) + Query(root->right, b, e, k);
  } else if (b <= root->b && e >= root->e) {
    return Query(root, root->b, root->e, k);
  } else if (b >= root->b && b <= root->e)  {
    return Query(root, b, root->e, k);
  } else {
    return Query(root, root->b, e, k);
  }
}

std::vector<int> CountSmaller(std::vector<int>& nums) {
  std::vector<int> ans;
  int n = nums.size();
  if (n == 0) return ans;

  TreeNode * root = NULL;
  Build(root, 0, n - 1, nums);
  for (int i = 0; i < n; i++) {
    int t = Query(root, i + 1, n - 1, nums[i]);
    ans.push_back(t);
  }
  return ans;
}
}  // namespace stree

using namespace algorithm;

namespace NB2 {
struct Node {
  int b;
  int e;
  int cnt;
  Node * left;
  Node * right;
  Node() : b(0), e(0), cnt(0), left(NULL), right(NULL) {}
};

void Build(std::vector<int> & vec, int b, int e, Node * & root) {
  if (b > e) return;
  root = new Node();
  root->b = vec[b];
  root->e = vec[e];
  if (b < e) {
    int mid = b + (e - b) / 2;
    Build(vec, b, mid, root->left);
    Build(vec, mid + 1, e, root->right);
  }
}

void Update(Node * root, int v) {
  if (root == NULL || v < root->b || v > root->e) return;
  if (root->b == v && root->e == v) {
    root->cnt++;
  } else {
    root->cnt++;
    Update(root->left, v);
    Update(root->right, v);
  }
}

int Query(Node * root, int v) {
  if (root == NULL || root->b > v) return 0;
  if (root->e < v) return root->cnt;
  else {
    return Query(root->left, v) + Query(root->right, v);
  }
}

std::vector<int> CountSmaller(std::vector<int>& nums) {
  int n = nums.size();
  if (n == 0) return std::vector<int>();
  std::vector<int> ans(n ,0);

  std::vector<int> tmp = nums;
  std::sort(tmp.begin(), tmp.end());
  std::vector<int>::iterator it = std::unique(tmp.begin(), tmp.end());
  tmp.resize(it - tmp.begin());

  Node * root = NULL;
  Build(tmp, 0, tmp.size() - 1, root);

  for (int i = n - 1; i >= 0; i--) {
    int t = Query(root, nums[i]);
    Update(root, nums[i]);
    ans[i] = t;
  }
  return ans;
}
}  // namespace NB2

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(-1);
  vec.push_back(-1);
  LOG(INFO) << JoinVector(CountSmaller(vec));
  LOG(INFO) << JoinVector(NB::CountSmaller(vec));
  LOG(INFO) << JoinVector(bound::CountSmaller(vec));
  LOG(INFO) << JoinVector(msort::CountSmaller(vec));
  LOG(INFO) << JoinVector(stree::CountSmaller(vec));
  LOG(INFO) << JoinVector(NB2::CountSmaller(vec));
  return 0;
}
