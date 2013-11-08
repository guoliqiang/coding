// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-07 00:55:13
// File  : code.cc
// Brief :

/*
 * WA
 * */

#include "base/public/common_ojhead.h"

namespace NB {
const int V = 110;
const int E = V * V;

#define typec float // type of res
typec coor[V][2];
const typec inf = 0x3f3f3f3f; // max of res
typec res, dis[V];
int to[V], cp[V], tag[V];
struct Edge {
  int u, v;
  typec c;
} eg[E];

int iroot(int i){
  if (cp[i] == i) return i;
  return cp[i] = iroot(cp[i]);
}

float Distance(int x, int y) {
  float t = (coor[x][0] - coor[y][0]) * (coor[x][0] - coor[y][0]) +
            (coor[x][1] - coor[y][1]) * (coor[x][1] - coor[y][1]);
  return sqrt(t);
}

int dirtree(int root, int nv, int ne) { // vertex: 0 ~ n-1
  int i, j, k, circle = 0;
  memset(tag, -1, sizeof(tag));
  memset(to, -1, sizeof(to));
  for (i = 0; i < nv; ++i) {
    dis[i] = inf;
  }
  for (j = 0; j < ne; ++j) {
    i = iroot(eg[j].u);
    k = iroot(eg[j].v);
    if (k != i && dis[k] > eg[j].c) {
      dis[k] = eg[j].c;
      to[k] = i;
    }
  }
  to[root] = -1; 
  dis[root] = 0;
  tag[root] = root;
  for (i = 0; i < nv; ++i) {
    if (cp[i] == i && -1 == tag[i]){
      j = i;
      for (; j != -1 && tag[j] == -1; j = to[j]) tag[j] = i;
      if (j == -1) return 0;
      if (tag[j] == i) {
        circle = 1; 
        tag[j] = -2;
        for (k = to[j]; k != j; k = to[k]) tag[k] = -2;
      }
    }
  }

  if (circle) {
   for (j = 0; j < ne; ++j) {
     i = iroot(eg[j].u);
     k = iroot(eg[j].v);
     if (k != i && tag[k] == -2) eg[j].c -= dis[k];
   }
   for (i = 0; i < nv; ++i) {
     if (tag[i] == -2) {
       res += dis[i];
       tag[i] = 0;
       for (j = to[i]; j != i; j = to[j]) {
         res += dis[j];
         cp[j] = i;
         tag[j] = 0;
       }
     }
   }
   if (0 == dirtree(iroot(root), nv, ne)) return 0;
  } else {
    for (i = 0; i < nv; ++i) if (cp[i] == i) res += dis[i];
  }
  return 1; // 若返回0代表原图不连通
}

void Read() {
  int n = 0;
  int m = 0;
  while (scanf("%d%d", &n, &m) != EOF) {
    int nv = n;;
    int ne = 0;
    getchar();
    memset(coor, 0, sizeof(coor));
    for (int i = 0; i < n; i++) {
      float x, y;
      scanf("%f%f", &x, &y);
      coor[i][0] = x;
      coor[i][1] = y;
      getchar();
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      if (u != v) {
        eg[ne].u = u - 1;
        eg[ne].v = v - 1;
        eg[ne].c = Distance(u - 1, v - 1);
        ne++;
      }
      getchar();
    }
    for (int i = 0; i < nv; i++) cp[i] = i;
    res = 0;
    float t = dirtree(0, nv, ne);
    if (t == 0) printf("poor snoopy\n");
    else printf("%.2lf\n", res);
  }
}

}  // namespace NB

namespace algorithm {
#define INF 0x3f3f3f3f
const int MAX = 105;
int N = 0;
double matrix[MAX][MAX];
float coor[MAX][2];

int to[MAX];
double dis[MAX];
int tag[MAX];
int root[MAX];

struct Node {
  int u;
  int v;
  float dis;
};
int V = 0;
Node edge[MAX * MAX];

float rs = 0;

float Distance(int x, int y) {
  float t = (coor[x][0] - coor[y][0]) * (coor[x][0] - coor[y][0]) +
          (coor[x][1] - coor[y][1]) * (coor[x][1] - coor[y][1]);
  return sqrt(t);
}

int Root(int k) {
  if (root[k] == k) return k;
  return root[k] = Root(root[k]);
}

int ZhuLiuE(int r) {
  memset(tag, -1, sizeof(tag));
  memset(to, -1, sizeof(to));
  for (int i = 1; i <= N; i++) dis[i] = INF;

  for (int i = 1; i <= V; i++) {
    int u = root[edge[i].u];
    int v = root[edge[i].v];
    if (u == v) continue;
    if (edge[i].dis < dis[v]) {
      dis[v] = edge[i].dis;
      to[v] = u;
    }
  }
  dis[r] = 0;
  to[r] = -1;
  tag[r] = 0;
  
  bool circle = false;
  for (int i = 1; i <= N; i++) {
    if (root[i] != i || tag[i] != -1) continue;
    int j = i;
    for (j = i; j != -1 && tag[j] == -1; j = to[j]) tag[j] = 0;
    if (j == -1) return -1;
    if (j == i) {
      circle = true;
      for (j = to[j]; j != i; j = to[j]) tag[j] = 1;
      tag[i] = 1;
    }
  }
  if (circle) {
    for (int i = 1; i <= V; i++) {
      int u = root[edge[i].u];
      int v = root[edge[i].v];
      if (u != v && tag[v] == 1)  edge[i].dis -= dis[v];
    }
    for (int i = 1; i <= N; i++) {
      if (tag[i] == 1) {
        for (int j = to[i]; j != i; j = to[j]) {
          root[j] = i;
          tag[j] = 0;
          rs += dis[j];
        }
        tag[i] = 0;
        rs += dis[i];
      }
    }
    if (ZhuLiuE(r) == -1) return -1;
  } else {
    for (int i = 1; i <= N; i++) {
      if (root[i] == i) rs += dis[i];
    }
  }
  return 0;
}

double ZLE(int r) {
  rs = 0;
  memset(root, 0, sizeof(root));
  for (int i = 1; i <= N; i++) root[i] = i;
  if (ZhuLiuE(r) == -1) return -1;
  else return rs;
}

void ReadE() {
  int n = 0;
  int m = 0;
  while (scanf("%d%d", &n, &m) != EOF) {
    N = n;
    V = 0;
    getchar();
    memset(coor, 0, sizeof(coor));
    for (int i = 0; i < n; i++) {
      float x, y;
      scanf("%f%f", &x, &y);
      coor[i + 1][0] = x;
      coor[i + 1][1] = y;
      getchar();
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      edge[++V].u = u;
      edge[V].v = v;
      if (u != v) {
        edge[V].dis = Distance(u, v);
      };
      getchar();
    }
    float t = ZLE(1);
    if (t < 0) printf("poor snoopy\n");
    else printf("%.2lf\n", t);
  }
}

int ZhuLiu(int r) {
  memset(tag, -1, sizeof(tag));
  memset(to, -1, sizeof(to));
  for (int i = 1; i <= N; i++) dis[i] = INF;
  dis[r] = 0;

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= N; j++) {
      int u = Root(i);
      int v = Root(j);
      if (u == v || matrix[i][j] <= 0) continue;
      if (matrix[i][j] < dis[v]) {
        dis[v] = matrix[i][j];
        to[v] = u;
      }
    }
  }
  to[r] = -1;
  tag[r] = 0;
  bool circle = false;
  for (int i = 1; i <= N; i++) {
    if (root[i] != i || tag[i] != -1) continue;
    int j = 0;
    for (j = i; j != -1 && tag[j] == -1; j = to[j]) tag[j] = 0;
    if (j == -1) return -1;
    if (j == i) {
      circle = true;
      for (j = to[i]; j != i; j = to[j]) tag[j] = 1;
      tag[i] = 1;
    }
  }
  if (circle) {
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= N; j++) {
        int u = Root(i);
        int v = Root(j);
        if (u != v && tag[v] == 1)  matrix[i][j] -= dis[v];
      }
    }
    for (int i = 1; i <= N; i++) {
      if (tag[i] == 1) {
        tag[i] = 0;
        rs += dis[i];
        for (int j = to[i]; j != i; j = to[j]) {
          root[j] = i;
          tag[j] = 0;
          rs += dis[j];
        }
      }
    }
    if (ZhuLiu(r) == -1) return -1;
  } else {
    for (int i = 1; i <= N; i++) {
      if (root[i] == i) rs += dis[i];
    }
  }
  return 0;
}

double ZL(int r) {
  rs = 0;
  memset(root, 0, sizeof(root));
  for (int i = 1; i <= N; i++) root[i] = i;
  if (ZhuLiu(r) == -1) return -1;
  else return rs;
}

void Read() {
  int n = 0;
  int m = 0;
  while (scanf("%d%d", &n, &m) != EOF) {
    N = n;
    getchar();
    memset(matrix, 0, sizeof(matrix));
    memset(coor, 0, sizeof(coor));
    for (int i = 0; i < n; i++) {
      float x, y;
      scanf("%f%f", &x, &y);
      coor[i + 1][0] = x;
      coor[i + 1][1] = y;
      getchar();
    }
    for (int i = 0; i < m; i++) {
      int u, v;
      scanf("%d%d", &u, &v);
      if (u != v) matrix[u][v] = Distance(u, v);
      getchar();
    }
    float t = ZL(1);
    if (t < 0) printf("poor snoopy\n");
    else printf("%.2lf\n", t);
  }
}



}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  FROMFILE;
  NB::Read();
  return 0;
}
