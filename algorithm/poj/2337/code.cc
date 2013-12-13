// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-13 19:10:49
// File  : code.cc
// Brief :

/*
 * 单词作为边，节点是26个字符，这样的构图才是可以用
 * 欧拉路来做的
 * Accepted  424K  47MS
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 30;
const int MAXM = 1010;
const int N = MAXN;
int M = 0;
int begin = 0;

struct Edge {
  int v;
  int visited;
  std::string str;
  bool operator < (const Edge & e2) const {
    return str < e2.str;
  }
};
std::vector<Edge> head[MAXN];

int in[MAXN];
int out[MAXN];
int root[MAXN];
int used[MAXN];

std::stack<std::string> stack;

int find(int k) {
  if (root[k] == k) return root[k];
  return root[k] = find(root[k]);
}

void unit(int x, int y) {
  int tx = find(x);
  int ty = find(y);
  root[tx] = root[ty];
}


bool Connect() {
  int cnt = 0;
  for (int i = 0; i < N; i++) {
    if (used[i] && find(i) == i) cnt++;
  }
  return cnt == 1 ? true : false;
}

bool Key(int & key) {
  int in_num = 0;
  int out_num = 0;
  for (int i = 0; i < N; i++) {
    if (used[i]) {
      if (in[i] == out[i]) continue;
      else if (in[i] - out[i] == 1) in_num++;
      else if (out[i] - in[i] == 1) {
        out_num++;
        key = i;
      } else {
        return false;
      }
    }
  }

  if (in_num == 1 && out_num == 1) return true;
  else if (in_num == 0 && out_num == 0) {
    for (int i = 0; i < N; i++) {
      if (used[i]) {
        key = i;
        break;
      }
    }
    return true;
  } else {
    return false;
  }
}

// NB
void Fleury(int key) {
  for (int i = 0; i < head[key].size(); i++) {
    if (head[key][i].visited == 0) {
      head[key][i].visited = 1;
      Fleury(head[key][i].v);
      stack.push(head[key][i].str);
    }
  }
}

void Path() {
  printf("%s", stack.top().c_str());
  stack.pop();
  while(!stack.empty()) {
    printf(".%s", stack.top().c_str());
    stack.pop();
  }
  printf("\n");
}



void Read() {
  int c = 0;
  scanf("%d", &c);
  getchar();
  for (int i = 0; i < c; i++) {
    int n = 0;
    scanf("%d", &n);
    getchar();
    char temp[25];
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    memset(used, 0, sizeof(used));
    for (int i = 0; i < N; i++) {
      root[i] = i;
      head[i].clear();
    }
    for (int j = 0; j < n; j++) {
      scanf("%s", temp);
      getchar();
      int len = strlen(temp);
      int u = temp[0] - 'a';
      int v = temp[len - 1] - 'a';
      Edge t;
      t.v = v;
      t.visited = 0;
      t.str = std::string(temp);
      head[u].push_back(t);
      unit(v, u);
      in[v]++;
      out[u]++;
      used[u] = used[v] = 1;
    }
    for (int i = 0; i < N; i++) {
      if (used[i]) std::sort(head[i].begin(), head[i].end());
    }
    int key = 0;
    if (!Connect() || !Key(key)) {
      printf("***\n");
    } else {
      Fleury(key);
      Path();
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
