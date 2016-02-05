// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 16:33:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// from top to bottom, from left to right
namespace algorithm {
struct TreeNode {
  TreeNode(int v) : val(v), left(NULL), right(NULL) {}
  int val;
  TreeNode * left;
  TreeNode * right;
};

std::vector<std::vector<int> > VerticleOrder(TreeNode * root) {
  std::map<int, std::vector<int> > tmp;
  std::queue<std::pair<TreeNode *, int> > queue;
  if (root != NULL) {
    queue.push(std::make_pair(root, 0));
  }

  while (queue.size()) {
    std::pair<TreeNode *, int> cur = queue.front();
    queue.pop();
    if (!tmp.count(cur.second)) tmp[cur.second] = std::vector<int>();
    tmp[cur.second].push_back(cur.first->val);

    if (cur.first->left != NULL) {
      queue.push(std::make_pair(cur.first->left, cur.second + 1));
    }
    if (cur.first->right != NULL) {
      queue.push(std::make_pair(cur.first->right, cur.second - 1));
    }
  }
  std::vector<std::vector<int> > ans;
  for (std::map<int, std::vector<int> >::iterator i = tmp.begin();
       i != tmp.end(); i++) {
    ans.push_back(i->second);
  }
  return ans;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
