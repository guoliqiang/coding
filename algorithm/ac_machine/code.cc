// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-14 16:25:40
// File  : code.cc
// Brief :

// poj 1204
#include "base/public/common_ojhead.h"

namespace algorithm {

const int MAXN = 100;
int W;
int N;
char words[MAXN][MAXN];

struct Node {
 Node(char c) {
   ch = ch;
   value = NULL;
   fail = NULL;
   for (int i = 0; i < 256; i++) {
     child[i] = NULL;
   }
 }

 const char * value;
 char ch;
 Node * child[256];
 Node * fail;
};

Node * root = NULL;

void Build(Node * r, const char * str, int k, int n) {
  if (k == n) {
    r->value = str;
  } else {
    int index = str[k];
    if (r->child[index] == NULL) r->child[index] = new Node(str[k]);
    Build(r->child[index], str, k + 1, n);
  }
}

// 构建fail指针不能用DFS
void FailBFS() {
  std::queue<Node *> queue;
  queue.push(root);
  while (!queue.empty()) {
    Node * cur = queue.front();
    queue.pop();
    for (int i = 0; i < 256; i++) {
      if (cur->child[i] == NULL) continue;
      if (cur == root) {
        cur->child[i]->fail = root;
      } else {
        Node * tmp = cur->fail;
        while (tmp != NULL && tmp->child[i] == NULL) tmp = tmp->fail;
        if (tmp == NULL) cur->child[i]->fail = root;
        else cur->child[i]->fail = tmp->child[i];
      }
      queue.push(cur->child[i]);
    }
  }
}

void Build() {
  root = new Node(' ');
  for (int i = 0; i < W; i++) {
    Build(root, words[i], 0, strlen(words[i]));
  }
  FailBFS();
}

std::vector<const char *> Match(const char * str) {
  int n = strlen(str);
  int k = 0;
  Node * r = root;
  std::vector<const char *> rs;
  while (k < n) {
    int index = str[k++];
    while (r != root && r->child[index] == NULL) r = r->fail;
    if (r->child[index] != NULL) r = r->child[index];
    // bug fixed
    Node * tmp = r;
    while (tmp != NULL) {
      if (tmp->value != NULL) rs.push_back(tmp->value);
      tmp = tmp->fail;
    }
  }
  return rs;
}

void Read() {
  while (scanf("%d%d", &W, &N) != EOF) {
    getchar();
    for (int i = 0; i < W; i++) {
      scanf("%s", words[i]);
      getchar();
    }
    Build();
    char str[1000];
    for (int i = 0; i < N; i++) {
      scanf("%s", str);
      getchar();
      std::vector<const char *> rs = Match(str);
      for (int i = 0; i < rs.size(); i++) {
        LOG(INFO) << "find " << rs[i] << " in " << str;
      }
    }
  }
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
