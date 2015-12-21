// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-21 16:53:42
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  Node() : val(NULL) {}

  std::string * val;
  std::map<char, Node *> child;
};

void DFS(const std::vector<std::vector<char> > & board,
         int i, int j, int m, int n, Node * root,
         std::vector<std::vector<int> > & visit,
         std::set<std::string *> & set) {
  if (!(root->child.count(board[i][j]))) return;

  Node * next = root->child[board[i][j]];
  if (next->val != NULL) set.insert(next->val);
  visit[i][j] = 1;
  if (i - 1 >= 0 && visit[i - 1][j] == 0) {
    DFS(board, i - 1, j, m, n, next, visit, set);
  }
  if (i + 1 < m && visit[i + 1][j] == 0) {
    DFS(board, i + 1, j, m, n, next, visit, set);
  }
  if (j - 1 >= 0 && visit[i][j - 1] == 0) {
    DFS(board, i, j - 1, m, n, next, visit, set);
  }
  if (j + 1 < n && visit[i][j + 1] == 0) {
    DFS(board, i, j + 1, m, n, next, visit, set);
  }
  visit[i][j] = 0;
}

std::vector<std::string> FindWords(std::vector<std::vector<char> >& board,
  std::vector<std::string>& words) {
  std::vector<std::string> ans;
  int m = board.size();
  if (m == 0) return ans;
  int n = board[0].size();

  Node * root = new Node();
  for (int i = 0; i < words.size(); i++) {
    Node * node = root;
    for (int j = 0; j < words[i].size(); j++) {
      if (!(node->child.count(words[i][j]))) {
        node->child[words[i][j]] = new Node();
      }
      node = node->child[words[i][j]];
    }
    node->val = &words[i];
  }
  std::set<std::string *> tmp;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      std::vector<std::vector<int> > visit(m, std::vector<int>(n, 0));
      DFS(board, i, j, m, n, root, visit, tmp);
    }
  }

  for (std::set<std::string*>::iterator i = tmp.begin(); i != tmp.end(); i++) {
    ans.push_back(**i);
  }
  return ans;
}


}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<std::vector<char> > board;
  std::vector<char> one;
  one.push_back('o'); one.push_back('a'); one.push_back('a'); one.push_back('n');
  board.push_back(one);
  one.clear();
  one.push_back('e'); one.push_back('t'); one.push_back('a'); one.push_back('e');
  board.push_back(one);
  one.clear();
  one.push_back('i'); one.push_back('h'); one.push_back('k'); one.push_back('r');
  board.push_back(one);
  one.clear();
  one.push_back('i'); one.push_back('f'); one.push_back('l'); one.push_back('v');
  board.push_back(one);
  one.clear();

  std::vector<std::string> words;
  words.push_back("oath");
  words.push_back("pea");
  words.push_back("eat");
  words.push_back("rain");

  LOG(INFO) << JoinVector(FindWords(board, words));

  return 0;
}
