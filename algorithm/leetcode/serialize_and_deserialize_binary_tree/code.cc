// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-01-08 23:31:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
using namespace std;

struct TreeNode {
  int val;
  TreeNode *left;
  TreeNode *right;
  TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

std::string IntToString(int v) {
    char buff[100] = { 0 };
    sprintf(buff, "%d", v);
    return std::string(buff);
}

int StringToInt(std::string & str) {
    return atoi(str.c_str());
}

std::vector<std::string> Split(const std::string & str) {
    std::vector<std::string> ans;
    std::string cur;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            if (cur.size()) ans.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(str[i]);
        }
    }
    if (cur.size()) ans.push_back(cur);
    return ans;
}

class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        std::string rs;
        std::queue<TreeNode *> queue;
        queue.push(root);
        
        while (queue.size()) {
            TreeNode * cur = queue.front();
            queue.pop();
            if (cur == NULL) rs += " N";
            else {
                rs += " " + IntToString(cur->val);
                queue.push(cur->left);
                queue.push(cur->right);
            }
        }
        return rs.substr(1);
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        std::vector<std::string> parts = Split(data);
        TreeNode * root = NULL;
        
        std::queue<TreeNode ** > queue;
        queue.push(&root);
        for (int i = 0; i < parts.size(); i++) {
            TreeNode ** cur = queue.front();
            queue.pop();
            if (parts[i] == "N") *cur = NULL;
            else {
                *cur = new TreeNode(StringToInt(parts[i]));
                queue.push(&((*cur)->left));
                queue.push(&((*cur)->right));
            }
        }
        return root;
    }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Codec foo;
  LOG(INFO) << foo.serialize(NULL);
  foo.deserialize(foo.serialize(NULL));
  return 0;
}
