// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:35:38
// File  : code.cc
// Brief :

/*
 * Run Status: Accepted!
 * Program Runtime: 8 milli secs
 * Progress: 3/3 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 14/14 test cases passed.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// wrong preorder can not determin a bst
void Generate(std::string str, TreeNode ** p) {
  if (str.size() == 0) return;
  *p = new TreeNode(str[0] - '0');
  if (str.size() == 1) return;
  if (str[1] == '#') {
    Generate(str.substr(2), &((*p)->right));
  } else {
    Generate(str.substr(1), &((*p)->left));
  }
}

TreeNode * Generate(std::string & str) {
  TreeNode * head = NULL;
  Generate(str, &head);
  return head;
}

// 树形dp标准写法

void DP(std::vector<std::vector<std::set<std::string> > > & dp) {
  int  n = dp.size();
  for (int i = 0; i < n; i++) {
    dp[i][i].insert(std::string(1, (char)('1' + i)));
  }
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < n - i; j++) {
      for (int k = j; k <= j + i; k++) {
        if (k == j) {
          for (std::set<std::string>::iterator it = dp[k + 1][j + i].begin();
               it != dp[k + 1][j + i].end(); it++) {
            std::string tmp(1, (char)(k + '1'));
            dp[j][j + i].insert(tmp + "#" + *it);
           }
        } else if (k == j + i) {
          for (std::set<std::string>::iterator it = dp[j][j + i - 1].begin();
               it != dp[j][j + i - 1].end(); it++) {
            std::string tmp(1, (char)(k + '1'));
            dp[j][j + i].insert(tmp + *it);
           }
        } else {
          for (std::set<std::string>::iterator it1 = dp[j][k - 1].begin();
               it1 != dp[j][k - 1].end(); it1++) {
            for (std::set<std::string>::iterator it2 = dp[k + 1][j + i].begin();
                 it2 != dp[k + 1][j + i].end(); it2++) {
              std::string tmp(1, (char)(k + '1'));
              dp[j][j + i].insert(tmp + *it1 + "#" + *it2);
            }
          }
        }
      }   // for
    }
  }
}

/*
 * used DPNew and TreesNews
 * Run Status: Accepted!
 * Program Runtime: 12 milli secs
 * Progress: 4/4 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 112 milli secs
 * Progress: 9/9 test cases passed.
 *
 * */

void DPNew(std::vector<std::vector<std::set<TreeNode *> > > & dp) {
  int  n = dp.size();
  for (int i = 0; i < n; i++) {
    dp[i][i].insert(new TreeNode(i + 1));
  }
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < n - i; j++) {
      for (int k = j; k <= j + i; k++) {
        if (k == j) {
          for (std::set<TreeNode *>::iterator it = dp[k + 1][j + i].begin();
               it != dp[k + 1][j + i].end(); it++) {
            TreeNode *tmp = new TreeNode(k + 1);
            tmp->right = *it;
            dp[j][j + i].insert(tmp);
           }
        } else if (k == j + i) {
          for (std::set<TreeNode*>::iterator it = dp[j][j + i - 1].begin();
               it != dp[j][j + i - 1].end(); it++) {
            TreeNode * tmp = new TreeNode(k + 1);
            tmp->left = *it;
            dp[j][j + i].insert(tmp);
           }
        } else {
          for (std::set<TreeNode*>::iterator it1 = dp[j][k - 1].begin();
               it1 != dp[j][k - 1].end(); it1++) {
            for (std::set<TreeNode*>::iterator it2 = dp[k + 1][j + i].begin();
                 it2 != dp[k + 1][j + i].end(); it2++) {
              TreeNode * tmp = new TreeNode(k + 1);
              tmp->left = *it1;
              tmp->right = *it2;
              dp[j][j + i].insert(tmp);
            }
          }
        }
      }   // for
    }
  }
}

std::vector<TreeNode *> TreesNew(int n) {
  std::vector<TreeNode *> rs;
  if (n == 0) {
    rs.push_back(NULL);
    return rs;
  }
  std::vector<std::vector<std::set<TreeNode *> > > dp(n, std::vector<std::set<TreeNode *> >(n, std::set<TreeNode *>()));
  DPNew(dp);
  rs.insert(rs.end(), dp[0][n - 1].begin(), dp[0][n - 1].end());
  return rs;
}

std::vector<std::string> NumTrees(int n) {
  std::vector<std::string> rs;
  std::vector<std::vector<std::set<std::string> > > dp(n, std::vector<std::set<std::string> >(n, std::set<std::string>()));
  DP(dp);
  rs.insert(rs.end(), dp[0][n - 1].begin(), dp[0][n - 1].end());
  return rs;
}

std::vector<TreeNode*> Trees(int n) {
  std::vector<TreeNode *> rs;
  if (n == 0) {
    rs.push_back(NULL);
    return rs;
  }
  std::vector<std::string> v = NumTrees(n);
  for (int i = 0; i < v.size(); i++) {
    LOG(INFO) << v[i];
    rs.push_back(Generate(v[i]));
  }
  return rs;
}

void PreOrder(TreeNode * p) {
  if (p == NULL) return;
  LOG(INFO) << p->val;
  PreOrder(p->left);
  PreOrder(p->right);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<TreeNode *> rs = TreesNew(3);
  for (int i = 0; i < rs.size(); i++) {
    HERE(INFO);
    PreOrder(rs[i]);
  }
  // LOG(INFO) << JoinVector(rs);
  return 0;
}
