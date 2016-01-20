// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-28 17:35:38
// File  : code.cc
// Brief :

/*
Given n, generate all structurally unique BST's (binary search trees) that store values 1...n.

For example,
Given n = 3, your program should return all 5 unique BST's shown below.

   1         3     3      2      1
    \       /     /      / \      \
     3     2     1      1   3      2
    /     /       \                 \
   2     1         2                 3
confused what "{1,#,2,3}" means? > read more on how binary tree is serialized on OJ.


OJ's Binary Tree Serialization:
The serialization of a binary tree follows a level order traversal, where '#' signifies a path terminator where no node exists below.

Here's an example:
   1
  / \
 2   3
    /
   4
    \
     5
The above binary tree is serialized as "{1,2,3,#,#,4,#,#,5}".

*/

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


struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

namespace algorithm {
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

namespace twice {
using namespace std;

std::vector<TreeNode *> Make(int v, std::vector<TreeNode *> & vec, bool right) {
   std::vector<TreeNode *> rs;
   for (int i = 0; i < vec.size(); i++) {
       TreeNode * t = new TreeNode(v);
       if (right) t->right = vec[i];
       else t->left = vec[i];
       rs.push_back(t);
   }
   return rs;
 }
 
 std::vector<TreeNode*> Make(int v, std::vector<TreeNode*> & left, std::vector<TreeNode*> right) {
     std::vector<TreeNode*> rs;
     for (int i = 0; i < left.size(); i++) {
         for (int j = 0; j < right.size(); j++) {
             TreeNode * t = new TreeNode(v);
             t->left = left[i];
             t->right = right[j];
             rs.push_back(t);
         }
     }
     return rs;
 }
 
 std::vector<TreeNode*> Generate(int n) {
   if (n == 0) {
       std::vector<TreeNode *> tmp;
       tmp.push_back(NULL);
       return tmp;
   }
   
   std::vector<std::vector<std::vector<TreeNode *> > > dp(n, std::vector<std::vector<TreeNode* > >(n, std::vector<TreeNode*>()));
   for (int k = 0; k < n; k++) {
       for (int j = 0; j < n - k; j++) {
           if (k == 0) dp[j][j + k].push_back(new TreeNode(j + 1));
           else {
               for (int l = j; l <= j + k; l++) {
                   std::vector<TreeNode *> foo;
                   if (l == j) {
                       foo = Make(l + 1, dp[j + 1][j + k], true);
                   } else if (l == j + k) {
                       foo = Make(l + 1, dp[j][j + k - 1], false);
                   } else {
                       foo = Make(l + 1, dp[j][l - 1], dp[l + 1][j + k]);
                   }
                   dp[j][j + k].insert(dp[j][j + k].end(), foo.begin(), foo.end());
               }
           }
       }
   }
   return dp[0][n - 1];
 }
}  // namespace twice

// 这个好写
namespace third {
void Generate(int b, int e, std::vector<std::vector<std::vector<TreeNode *> > > & dp) {
  if (b == e) {
    dp[b][e].push_back(new TreeNode(b + 1));
    return;
  }

  for (int i = b; i <= e; i++) {
    if (i == b) {
      if (dp[i + 1][e].size() == 0) Generate(i + 1, e, dp);
      for (int j = 0; j < dp[i + 1][e].size(); j++) {
        TreeNode * ptr = new TreeNode(b + 1);
        ptr->right = dp[i + 1][e][j];
        dp[b][e].push_back(ptr);
      }
    } else if (i == e) {
      if (dp[b][i- 1].size() == 0) Generate(b, i - 1, dp);
      for (int j = 0; j < dp[b][i - 1].size(); j++) {
        TreeNode * ptr = new TreeNode(e + 1);
        ptr->left = dp[b][i - 1][j];
        dp[b][e].push_back(ptr);
      }
    } else {
      if (dp[b][i - 1].size() == 0) Generate(b, i - 1, dp);
      if (dp[i + 1][e].size() == 0) Generate(i + 1, e, dp);
      for (int j = 0; j < dp[b][i - 1].size(); j++) {
        for (int k = 0; k < dp[i + 1][e].size(); k++) {
          TreeNode * ptr = new TreeNode(i + 1);
          ptr->left = dp[b][i - 1][j];
          ptr->right = dp[i + 1][e][k];
          dp[b][e].push_back(ptr);
        }
      }
    }
  }
}

std::vector<TreeNode *> Generate(int n) {
  if (n == 0) {
    std::vector<TreeNode *> foo;
    foo.push_back(NULL);
    return foo;
  }
  std::vector<std::vector<std::vector<TreeNode*> > > dp(n, std::vector<std::vector<TreeNode *> > (n, std::vector<TreeNode*>()));
  Generate(0, n - 1, dp);
  return dp[0][n - 1];
}
} // namespace third

namespace four {
std::vector<TreeNode *> Generate(std::vector<int> & vec, int b, int e) {
    std::vector<TreeNode *> rs;

    if (b == e) {
        rs.push_back(new TreeNode(vec[b]));
    } else {
        for (int i = b; i <= e; i++) {
            if (i == b) {
                std::vector<TreeNode *> right = Generate(vec, b + 1, e);
                for (int j = 0; j < right.size(); j++) {
                    TreeNode * root = new TreeNode(vec[i]);
                    root->right = right[j];
                    rs.push_back(root);
                }
            } else if (i == e) {
                std::vector<TreeNode *> left = Generate(vec, b, e - 1);
                for (int j = 0; j < left.size(); j++) {
                    TreeNode * root = new TreeNode(vec[i]);
                    root->left = left[i];
                    rs.push_back(root);
                }
            } else {
                std::vector<TreeNode *> left = Generate(vec, b, i - 1);
                std::vector<TreeNode *> right = Generate(vec, i + 1, e);
                for (int j = 0; j < left.size(); j++) {
                    for (int k = 0; k < right.size(); k++) {
                        TreeNode * root = new TreeNode(vec[i]);
                        root->left = left[j];
                        root->right = right[k];
                        rs.push_back(root);
                    }
                }
            }
        }
    }
    return rs;
}
}  // namespace four

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(1);
  vec.push_back(2);
  std::vector<TreeNode *> rs = four::Generate(vec, 0, 1);
  for (int i = 0; i < rs.size(); i++) {
    PreOrder(rs[i]);
    LOG(INFO) << "i=" << i;
  }
  return 0;
}
