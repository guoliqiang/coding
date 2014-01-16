// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-13 18:44:30
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// Accepted 15628K  2157MS
// poj1204
namespace algorithm {
const int MAXN = 1010;
int M;
int N;
int W;

struct RNode {
  int row;
  int col;
  char ch;
  RNode(int r = 0, int co = 0, char c = ' ') : row(r), col(co), ch(c) {}
};
char words[MAXN][MAXN];
char matrix[MAXN][MAXN];
std::map<std::string, RNode> rs;

struct Node {
  char ch;
  Node * failure;
  Node * child[26];
  const char * value;
  Node(char c) : ch(c), failure(NULL) {
    value = NULL;
    for (int i = 0; i < 26; i++) child[i] = NULL;
  }
};

Node * ac_root = NULL;

void Build(Node * root, const char * str, int k, int n) {
  if (k == n) {
    root->value = str;
  } else {
    int index = str[k] - 'A';
    if (root->child[index] == NULL) root->child[index] = new Node(str[k]);
    Build(root->child[index], str, k + 1, n);
  }
}

// BFS
void Failure() {
  std::queue<Node *> queue;
  queue.push(ac_root);
  while (!queue.empty()) {
    Node * t = queue.front();
    queue.pop();
    for (int i = 0; i < 26; i++) {
      if (t->child[i] == NULL) continue;
      if (t == ac_root) {
        t->child[i]->failure = ac_root;
      } else {
        Node * tmp = t->failure;
        while (tmp != NULL && tmp->child[i] == NULL) {
          tmp = tmp->failure;
        }
        if (tmp == NULL) {
          t->child[i]->failure = ac_root;
        } else {
          t->child[i]->failure = tmp->child[i];
        }
      }
      queue.push(t->child[i]);
    }
  }
}

// 错误，不能用DFS
void FailureDFS(Node * root) {
  for (int i = 0; i < 26; i++) {
    if (root->child[i] == NULL) continue;
    if (root == ac_root) {
      root->child[i]->failure = ac_root;
    } else {
      Node * tmp = root->failure;
      while (tmp != NULL && tmp->child[i] == NULL) tmp = tmp->failure;
      if (tmp == NULL) root->child[i]->failure = ac_root;
      else root->child[i]->failure = tmp->child[i];
    }
    FailureDFS(root->child[i]);
  }
}

void Build() {
  ac_root = new Node(' ');
  for (int i = 0; i < W; i++) {
    Build(ac_root, words[i], 0, strlen(words[i]));
  }
  ac_root->failure = NULL;
  Failure();
}

void Next(int & x, int &y, int ch) {
  if (ch == 'A' || ch == 'B' || ch == 'H') {
    x -= 1;
  }
  if (ch == 'F' || ch == 'E' || ch == 'D') {
    x += 1;
  }
  if (ch == 'H' || ch == 'G' || ch == 'F') {
    y -= 1;
  }
  if (ch == 'B' || ch == 'C' || ch == 'D') {
    y += 1;
  }
}

void Save(int x, int y, char ch, Node * root) {
  if (root->value != NULL) {
    if (!rs.count(root->value)) {
      int row, col;
      int n = strlen(root->value);
      if (ch == 'A') {
        row = x + n - 1;
        col = y;
      }
      if (ch == 'B') {
        row = x + n - 1;
        col = y - n + 1;
      }
      if (ch == 'C') {
        row = x;
        col = y - n + 1;
      }
      if (ch == 'D') {
        row = x - n + 1;
        col = y - n + 1;
      }
      if (ch == 'E') {
        row = x - n + 1;
        col = y;
      }
      if (ch == 'F') {
        row = x - n + 1;
        col = y + n - 1;
      }
      if (ch == 'G') {
        row = x;
        col = y + n - 1;
      }
      if (ch == 'H') {
        row = x + n - 1;
        col = y + n - 1;
      }
      rs[root->value] = RNode(row, col, ch);
    }
  }
}

void Match(Node * root, int x, int y, char ch) {
  while (true) {
    if (x < 0 || x >= M || y < 0 || y >= N) return;
    int index = matrix[x][y] - 'A';
    while (root != ac_root && root->child[index] == NULL) {
      root = root->failure;
    }
    if (root->child[index] != NULL) {
      root = root->child[index];
    }
    Node * t = root;
    while (t != ac_root) {
      Save(x, y, ch, t);
      t = t->failure;
    }
    Next(x, y, ch);
  }
}

void Solve() {
  for (int i = 0; i < M; i++) {
    Match(ac_root, i, 0, 'C');
    Match(ac_root, i, N - 1, 'G');
  }
  for (int i = 0; i < N; i++) {
    Match(ac_root, 0, i, 'E');
    Match(ac_root, M - 1, i, 'A');
  }
  for (int i = 0; i < M; i++) {
    Match(ac_root, i, 0, 'B');
    Match(ac_root, i, N - 1, 'F');
  }
  for (int i = 0; i < N; i++) {
    Match(ac_root, M - 1, i, 'B');
    Match(ac_root, 0, i, 'F');
  }
  for (int i = N - 1; i >= 0; i--) {
    Match(ac_root, 0, i, 'D');
    Match(ac_root, M - 1, i, 'H');
  }
  for (int i = 0; i < M; i++) {
    Match(ac_root, i, 0, 'D');
    Match(ac_root, i, N - 1, 'H');
  }
}

void Read() {
  while (scanf("%d%d%d", &M, &N, &W) != EOF) {
    rs.clear();
    getchar();
    for (int i = 0; i < M; i++) {
      scanf("%s", matrix[i]);
      getchar();
    }
    for (int i = 0; i < W; i++) {
      scanf("%s", words[i]);
      getchar();
    }
    Build();
    Solve();
    for (int i = 0; i < W; i++) {
      RNode & t = rs[words[i]];
      printf("%d %d %c\n", t.row, t.col, t.ch);
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


