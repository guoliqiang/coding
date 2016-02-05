// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 21:44:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace st {
struct Node {
  long long int b;
  long long int e;
  int cnt;
  Node * left;
  Node * right;
  Node() : b(0), e(0), cnt(0), left(NULL), right(NULL) {}
};

void Build(std::vector<long long int> & v, int b, int e, Node * & root) {
  if (b > e) return;
  root = new Node();
  root->b = v[b];
  root->e = v[e];
  if (b < e) {
    int mid = b + (e - b) / 2;
    Build(v, b, mid, root->left);
    Build(v, mid + 1, e, root->right);
  }
}

bool Query(Node * root, long long int b, long long int e) {
  if (root == NULL || root->b > e || root->e < b) return false;

  if (root->b >= b && root->e <= e) return root->cnt > 0;
  else if (root->b <= b && root->e >= e) {
    return Query(root->left, b, e) || Query(root->right, b, e);
  } else if (b >= root->b && b <= root->e) {
    return Query(root, b, root->e);
  } else {
    return Query(root, root->b, e);
  }
}

void Update(Node * root, long long int k, int val) {
  if (root == NULL || root->b > k || root->e < k) return;
  root->cnt += val;
  Update(root->left, k, val);
  Update(root->right, k, val);
}

bool ContainsNearbyAlmostDuplicate(std::vector<int>& nums, int k, int t) {
  std::vector<long long int> sorted(nums.begin(), nums.end());
  std::sort(sorted.begin(), sorted.end());
  std::vector<long long int>::iterator it = std::unique(sorted.begin(), sorted.end());
  sorted.resize(it - sorted.begin());

  Node * root = NULL;
  Build(sorted, 0, sorted.size() - 1, root);

  for (int i = 0; i < nums.size(); i++) {
    if (i > k) {
      LOG(INFO) << "delete " << nums[i - k - 1];
      Update(root, nums[i - k - 1], -1);
    }
    long long int begin = (long long int)nums[i] - t;
    long long int end = (long long int)nums[i] + t;
    LOG(INFO) << begin << " "  << end;
    if (Query(root, begin, end)) {
      LOG(INFO) << begin << "~" << end << " " << nums[i];
      return true;
    }
    Update(root, nums[i], 1);
  }
  return false;
}
}  // namespace st

namespace algorithm {
bool ContainsNearbyAlmostDuplicate(std::vector<int>& nums, int k, int t) {
  int n = nums.size();
  std::multimap<int, int> m;
  for (int i = 0; i < n; i++) {
    m.insert(std::make_pair(nums[i], i));
  }
  for (int i = 0; i < n; i++) {
    std::multimap<int, int>::iterator it = m.lower_bound(nums[i] - t);
    while(it != m.end() && it->second != i && fabs(it->first - nums[i]) <= t) {
      if (it->second != i && fabs(it->second - i) <= k) return true;
      it++;
    }
  }
  return false;
}
} // namespace algorithm

namespace twice {
using namespace std;

class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int k, int t) {
        std::map<int, int> idx;
        for (int i = 0; i < nums.size(); i++) {
            std::map<int, int>::iterator it = idx.lower_bound(nums[i] - t);
            while (it != idx.end()) {
                if (fabs(it->first - nums[i]) <= t && fabs(it->second - i) <= k) return true;
                if (it->first - nums[i] > t) break;
                it++;
            }
            idx[nums[i]] = i;
        }
        return false;
    }
};
}  // namespace twice

namespace NB {
bool ContainsNearbyAlmostDuplicate(std::vector<int>& nums, int k, int t) {
  std::set<int> set;
  for (int i = 0; i < nums.size(); i++) {
    if (i > k) set.erase(nums[i - k - 1]);
    std::set<int>::iterator it = set.lower_bound(nums[i] - t);
    if (it != set.end() && fabs(nums[i] - *it) <= t) return true;
    set.insert(nums[i]);
  }
  return false;
}
}  // namespace NB

namespace st2 {
struct Node {
    long long int b;
    long long int e;
    int cnt;
    Node * left;
    Node * right;
    Node() : b(0), e(0), cnt(0), left(NULL), right(NULL) {}
};

void Build(std::vector<long long int> & vec, int b, int e, Node * & root) {
    root = new Node();
    root->b = vec[b];
    root->e = vec[e];
    if (b != e) {
        int mid = b + (e - b) / 2;
        Build(vec, b, mid, root->left);
        Build(vec, mid + 1, e, root->right);
    }
}

void Update(Node * root, long long int key, int val) {
    if (root == NULL || root->b > key || root->e < key) return;
    if (root->b == key && root->e == key) {
        root->cnt += val;
    } else {
        root->cnt += val;
        Update(root->left, key, val);
        Update(root->right, key, val);
    }
}

bool Find(Node * root, long long int b, long long int e) {
    if (root == NULL || root->b > e || root->e < b) return false;
    if (root->b >= b && root->e <= e) {
        return root->cnt > 0;
    } else if (b >= root->b && e<= root->e) {
        return Find(root->left, b, e) || Find(root->right, b, e);
    } else if (b >= root->b) {
        return Find(root, b, root->e);
    } else {
        return Find(root, root->b, e);
    }
}

bool containsNearbyAlmostDuplicate(std::vector<int>& nums, int k, int t) {
    if (nums.size() == 0) return false;
    
    std::vector<long long int> vec(nums.begin(), nums.end());
    std::sort(vec.begin(), vec.end());
    vec.resize(std::unique(vec.begin(), vec.end()) - vec.begin());
    
    Node * root = NULL;
    Build(vec, 0, vec.size() - 1, root);
    for (int i = 0; i < nums.size(); i++) {
        if (i > k) {
            Update(root, nums[i - k - 1], -1);
        }
        long long int low = (long long int)nums[i] - t;
        long long int up = (long long int)nums[i] + t;
        if (Find(root, low, up)) return true;
        Update(root, nums[i], 1);
    }
    return false;
}
}  // namespace st2
int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(0);
  vec.push_back(2147483647);
  LOG(INFO) << st::ContainsNearbyAlmostDuplicate(vec, 1, 2147483647);
  LOG(INFO) << NB::ContainsNearbyAlmostDuplicate(vec, 1, 2147483647);
  return 0;
}


