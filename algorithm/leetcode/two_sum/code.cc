#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <map>

/*
 * Given an array of integers, find two numbers such that they add up to a specific target number.
 * The function twoSum should return indices of the two numbers such that they add up to the target,
 * where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.
 * You may assume that each input would have exactly one solution.
 * Input: numbers={2, 7, 11, 15}, target=9
 * Output: index1=1, index2=2
 * */

#include "base/public/random.h"
#include "base/public/common_head.h"

namespace algorithm {

class Solution {
 public:
  std::vector<int> twoSum(std::vector<int> &numbers, int target) {
    std::map<int, int> foo;
    std::vector<int> rs;
    int index1 = 0;
    int index2 = 0;
    for (int i = 0; i< numbers.size(); i++) {
      foo.insert(std::make_pair(numbers[i], i));
    }
    for (int i = 0; i < numbers.size(); i++) {
      if (foo.count(target - numbers[i]) && i != foo[target - numbers[i]]) {
        index1 = i + 1;
        index2 = foo[target - numbers[i]] + 1;
        break;
      }
    }
    if (index1 > index2) std::swap(index1, index2);
    rs.push_back(index1);
    rs.push_back(index2);
    return rs;
  }
};

}  // namespace algorithm

/*
 * 10 / 10 test cases passed.
 * Status: Accepted
 * Runtime: 16 ms
 * Submitted: 0 minutes ago
 *
 * */


namespace NB {
class Solution {
 public:
  struct Node {
    int value;
    int index;
  };
  static bool Cmp(const Node & a, const Node & b) {
    return a.value < b.value;
  }
  std::vector<int> twoSum(std::vector<int> &numbers, int target) {
    std::vector<Node> foo;
    std::vector<int> rs;
    for (int i = 0; i < numbers.size(); i++) {
      Node n;
      n.value = numbers[i];
      n.index = i;
      foo.push_back(n);
    }
    sort(foo.begin(), foo.end(), Cmp);
    int index1 = 0;
    int index2 = foo.size() - 1;
    while (foo[index1].value + foo[index2].value != target) {
      if (foo[index1].value + foo[index2].value < target) index1++;
      if (foo[index1].value + foo[index2].value > target) index2--;
    }
    index1 = foo[index1].index + 1;
    index2 = foo[index2].index + 1;
    if (index1 > index2) std::swap(index1, index2);
    rs.push_back(index1);
    rs.push_back(index2);
    return rs;
  }
};
}  // namespace NB

// k1 + k2 < target
// k1 + k2 > target
namespace Extend {

struct Node {
  int b;
  int e;
  int cnt;
  Node * left;
  Node * right;
  Node() : b(0), e(0), cnt(0), left(NULL), right(NULL){}
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

int Query(Node * root, int v) {
  if (root == NULL || v < root->b) return 0;
  if (root->e < v) return root->cnt;
  else {
    return Query(root->left, v) + Query(root->right, v);
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


int FindSmaller(std::vector<int> & vec, int target) {
  int ans = 0;
  std::vector<int> num;
  for (int i = 0; i < vec.size(); i++) {
    num.push_back(vec[i]);
  }
  std::sort(num.begin(), num.end());
  std::vector<int>::iterator it = std::unique(num.begin(), num.end());
  num.resize(it - num.begin());
  Node * root = NULL;
  Build(num, 0, num.size() - 1, root);
  for (int i = 0; i < vec.size(); i++) {
    int t = target - vec[i];
    ans += Query(root, t);
    Update(root, vec[i]);
  }
  return ans;
}

std::vector<std::pair<int, int> > BruteForce(std::vector<int> & vec, int target) {
  std::vector<std::pair<int, int> > ans;
  int n = vec.size();
  if (n == 0) return ans;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (vec[i] + vec[j] < target) ans.push_back(std::make_pair(j, i));
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}
}  // namespace Extend

namespace Extend2 {
std::vector<std::pair<int, int> > FindSmaller(std::vector<int> vec, int target) {
  int n = vec.size();
  std::vector<std::pair<int, int> > ans;
  std::vector<std::pair<int, int> > tmp;
  for (int i = 0; i < n; i++) {
    tmp.push_back(std::make_pair(vec[i], i));
  }
  std::sort(tmp.begin(), tmp.end());
  int b = 0;
  int e = n - 1;
  while (b < e) {
    if (tmp[b].first + tmp[e].first < target) {
      for (int i = b + 1; i <= e; i++) {
        ans.push_back(std::make_pair(std::min(tmp[b].second, tmp[i].second),
                                     std::max(tmp[b].second, tmp[i].second)));
      }
      b++;
    } else {
      e--;
    }
  }
  std::sort(ans.begin(), ans.end());
  return ans;
}
}  // namespace extend2


int main(int argc, char** argv) {
  std::vector<int> vec;
  for (int i = 0; i < 5; i++) {
    vec.push_back(base::Random(10));
  }
  LOG(INFO) << JoinVector(vec);
  LOG(INFO) << JoinVectorP(Extend::BruteForce(vec, 10)) << "size= " << Extend::BruteForce(vec, 10).size();
  // LOG(INFO) << (Extend::FindSmaller(vec, 10));
  LOG(INFO) << JoinVectorP(Extend2::FindSmaller(vec, 10));
  return 0;
}
