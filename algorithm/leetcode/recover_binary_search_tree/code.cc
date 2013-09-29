// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-29 10:42:33
// File  : code.cc
// Brief :

#include "../ojbst/ojbst.h"


namespace NB {
// http://digiter.diandian.com/post/2013-06-11/40051522966
// 在中序遍历中，如果左子树的最大节点小于根节点，则此节点一定是出错的节点
//               如果右子树中某个节点的值小于根节点 则此节电一定是出错节点
//
// http://blog.csdn.net/niaokedaoren/article/details/8832124
// 例如
// 1 2 3 4 5 6 7
// 1 5 3 4 2 6 7
// 也就是记录第一次出现逆序(5, 3)的地方和最有一次出现逆序的地方(4, 2)
using namespace algorithm;

// 这不是 O(1) 是   O(N)
void Proc(TreeNode *root, TreeNode *&n1, TreeNode *&n2, TreeNode *&prev) {
    if(!root) return;
    Proc(root->left,n1,n2,prev);
    if(prev && prev->val > root->val) {
        n2 = root;
        if(!n1) n1 = prev;
    }
    prev = root;
    Proc(root->right,n1,n2,prev);
}

/*
 * Run Status: Accepted!
 * Program Runtime: 0 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 280 milli secs
 * Progress: 2516/2516 test cases passed.
 * 
 * */
// 线索二叉树，详情见 binary_tree_inorder_traversal 题目下的code
void MorrisInOrder(TreeNode * root, TreeNode * &n1, TreeNode *&n2, TreeNode *&prev) {
  if(root == NULL) return;

  while (root != NULL) {
    if(root->left == NULL) {
      // visit
      if (prev != NULL && prev->val > root->val) {
        n2 = root;
        if (n1 == NULL) n1 = prev;
      }
      prev = root;

      root = root->right;
    } else {
      TreeNode * foo = root->left;
      while (foo->right != NULL && foo->right != root) {
        foo  = foo->right;
      }
      if (foo->right == NULL) {
        foo->right = root;
        root = root->left;
      } else {
        // visit
        if (prev != NULL && prev->val > root->val) {
          n2 = root;
          if (n1 == NULL) n1 = prev;
        }
        prev = root;

        foo->right = NULL;
        root = root->right;
      }
    }
  }
}

void RecoverTree(TreeNode *root) {
    TreeNode * n1 = NULL;
    TreeNode * n2 = NULL;
    TreeNode * prev = NULL;
    MorrisInOrder(root,n1,n2,prev);
    if(n1 && n2) std::swap(n1->val, n2->val);
}
}  // namespace NB

namespace algorithm {

/*
 * Run Status: Accepted!
 * Program Runtime: 16 milli secs
 * Progress: 20/20 test cases passed.
 * Run Status: Accepted!
 * Program Runtime: 396 milli secs
 * Progress: 2516/2516 test cases passed.
 *
 * */

TreeNode * Min(TreeNode * p1, TreeNode * p2) {
  return p1->val < p2->val ? p1 : p2;
}

TreeNode * Max(TreeNode * p1, TreeNode * p2) {
  return p1->val > p2->val ? p1 : p2;
}

// Note used reference
bool FindMistake(TreeNode * root, TreeNode * & max, TreeNode * & min) {
  max = NULL;
  min = NULL;
  if (root == NULL) {
    return false;
  } else {
    TreeNode * p_left_max = NULL;
    TreeNode * p_right_max = NULL;
    TreeNode * p_left_min = NULL;
    TreeNode * p_right_min = NULL;

    if (FindMistake(root->left, p_left_max, p_left_min)) return true;
    if (FindMistake(root->right, p_right_max, p_right_min)) return true;

    if (p_left_max != NULL && p_left_max->val >= root->val) {
      std::swap(root->val, p_left_max->val);
      return true;
    }
    if (p_right_min != NULL && p_right_min->val <= root->val) {
      std::swap(root->val, p_right_min->val);
      return true;
    }
    min = (p_left_min == NULL) ? root : Min(root, p_left_min);
    min = (p_right_min == NULL) ? min : Min(min, p_right_min);

    max = (p_left_max == NULL) ? root : Max(root, p_left_max);
    max = (p_right_max == NULL) ? max : Max(max, p_right_max);

    return false;
  }
}

void RecoverTree(TreeNode * root) {
  TreeNode * max = NULL;
  TreeNode * min = NULL;
  while (FindMistake(root, max, min)) {}
}

}  // namespace algorithn




/*
 * 以下是尝试果的错误解法
 * */

namespace algorithm {

// Note used reference
void FindMistake(TreeNode * root, TreeNode * & max, TreeNode * & min,
                 std::vector<std::pair<TreeNode *, TreeNode *> > & rs) {
  if (root == NULL) {
    max = NULL;
    min = NULL;
    return;
  } else {
    LOG(INFO) << "iterator root:" << root->val << " " << root->left << " " << root->right;
    TreeNode * p_left_max = NULL;
    TreeNode * p_right_max = NULL;
    TreeNode * p_left_min = NULL;
    TreeNode * p_right_min = NULL;

    FindMistake(root->left, p_left_max, p_left_min, rs);
    FindMistake(root->right, p_right_max, p_right_min, rs);

    if (p_left_max != NULL && p_left_max->val >= root->val) {
      LOG(INFO) << "root:" << root->val;
      LOG(INFO) << "find:" <<  p_left_max->val;
      rs.push_back(std::make_pair(root, p_left_max));
    }
    if (p_right_min != NULL && p_right_min->val <= root->val) {
      LOG(INFO) << "root:" << root->val;
      LOG(INFO) << "find:" << p_right_min->val;
      rs.push_back(std::make_pair(root, p_right_min));
    }

    min = (p_left_min == NULL) ? root : Min(root, p_left_min);
    min = (p_right_min == NULL) ? min : Min(min, p_right_min);

    max = (p_left_max == NULL) ? root : Max(root, p_left_max);
    max = (p_right_max == NULL) ? max : Max(max, p_right_max);
  }
}

void RecoverTreeM1(TreeNode * root) {
  std::vector<std::pair<TreeNode *, TreeNode *> > mistake;
  TreeNode * max = NULL;
  TreeNode * min = NULL;

  FindMistake(root, max, min, mistake);

  if (mistake.size() == 2) {
    LOG(INFO) << "mistake is two";
    if (mistake[0].first == mistake[1].first) {
      int tmp = mistake[0].second->val;
      mistake[0].second->val = mistake[1].second->val;
      mistake[1].second->val = tmp;
    } else {
      int tmp = mistake[0].first->val;
      mistake[0].first->val = mistake[1].second->val;
      mistake[1].second->val = tmp;
    }
  }
  if (mistake.size() == 1) {
    LOG(INFO) << "mistake is one";
    int tmp = mistake[0].first->val;
    mistake[0].first->val = mistake[0].second->val;
    mistake[0].second->val = tmp;
  }
}

/*
 * M1解法无法解决被交换的节点是根节点和某个叶子节点
 * 其原因就是没有做循环修改
 * */

// Note used reference
// 想要返回 最大最小最好是 用TreeNode * 的max和min
void FindMistake(TreeNode * root, int * & max, int * & min, std::vector<TreeNode*> & rs) {
  if (root == NULL) {
    max = NULL;
    min = NULL;
    return;
  } else {
    int left_max = 0;
    int right_max = 0;
    int left_min = 0;
    int right_min = 0;

    int * p_left_max = &left_max;
    int * p_right_max = &right_max;
    int * p_left_min = &left_min;
    int * p_right_min = &right_min;

    FindMistake(root->left, p_left_max, p_left_min, rs);
    FindMistake(root->right, p_right_max, p_right_min, rs);

    if (p_left_max != NULL && *p_left_max >= root->val) {
      LOG(INFO) << "root:" << root->val;
      if (p_left_max != NULL) LOG(INFO) << "find:" <<  *p_left_max;
      rs.push_back(root);
    }
    if (p_right_min != NULL && *p_right_min <= root->val) {
      LOG(INFO) << "root:" << root->val;
      if (p_right_min != NULL) LOG(INFO) << "find:" << *p_right_min;
      rs.push_back(root);
    }

    *min = (p_left_min == NULL) ? root->val : std::min(root->val, *p_left_min);
    *min = (p_right_min == NULL) ? *min : std::min(*min, *p_right_min);

    *max = (p_left_max == NULL) ? root->val : std::min(root->val, *p_left_max);
    *max = (p_right_max == NULL) ? *min : std::min(*max, *p_right_max);
  }
}


void RecoverTreeM2(TreeNode * root) {
  std::vector<TreeNode *> mistake;
  int max = 0;
  int min = 0;

  int * p_max = &max;
  int * p_min = &min;
  FindMistake(root, p_max, p_min, mistake);

  if (mistake.size() == 2) {
    int tmp = mistake[0]->val;
    mistake[0]->val = mistake[1]->val;
    mistake[1]->val = tmp;
  }
  if (mistake.size() == 1) {
    LOG(INFO) << "mistake :" << mistake[0]->val;
    int * p = NULL;
    if (mistake[0]->left != NULL && mistake[0]->left->val >= mistake[0]->val) {
      p = &(mistake[0]->left->val);
    }
    if (mistake[0]->right != NULL && mistake[0]->right->val <= mistake[0]->val) {
      p = &(mistake[0]->right->val);
    }
    int tmp = *p;
    *p = mistake[0]->val;
    mistake[0]->val = tmp;
    
  }
}
/*
 * M2解法写代码时根据交换的两个节点是非叶节点写的
 * 起无法处理叶节点情况
 * */


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string str = "0,1";
  TreeNode * root = Make(str);
  LOG(INFO) << "InOrder:";
  InOrder(root);
  LOG(INFO) << "PreOrder:";
  PreOrder(root);
  
  NB::RecoverTree(root);
  
  LOG(INFO) << "InOrder:";
  InOrder(root);
  LOG(INFO) << "PreOrder:";
  PreOrder(root);
  return 0;
}
