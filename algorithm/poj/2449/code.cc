// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-08 00:14:45
// File  : code.cc
// Brief :
// MLE

#include "base/public/common_ojhead.h"

// AC Accepted  7476K 282MS
namespace NB {

const int MAXN = 1005;
const int MAXM = 200100;
  
struct Ed{
  int v;
  int c;
  int nxt;  
} Edge[MAXM];  
  
int head[MAXN];  
int tail[MAXN];  
int dis[MAXN];
int vis[MAXN];
int cnt[MAXN];

struct Node {  
  int v;
  int d;
  Node (int iv, int id) : v(iv), d(id){}
  bool operator < (const Node &a) const { 
    return a.d + dis[a.v] < d + dis[v];
  }  
};
  
void Dijstra(int n, int s, int t) {  
  memset(vis, 0, sizeof(vis));  
  for (int i = 0; i <= n; i++) dis[i] = INF;
  dis[t] = 0;  

  for(int i = 0; i < n; i++ ) {  
    int min = INF;  
    int k = -1;  
    for(int j = 0; j < n; j++) {  
      if (vis[j] == false && min > dis[j]) {
         min = dis[j];
         k = j;
      }
    }  
    if (k == -1) break;  
    vis[k] = true;  
    for (int j = tail[k]; j != -1; j = Edge[j].nxt) {  
       int v = Edge[j].v;  
       if (dis[v] > dis[k] + Edge[j].c) dis[v] = dis[k] + Edge[j].c;  
    }  
  }  
}  
  
int Astar(int n, int s, int t, int K) {  
  std::priority_queue<Node>queue;  
  memset(cnt,0,sizeof(cnt));  
  queue.push(Node(s, 0));  
  while(!queue.empty()) {  
    Node cur = queue.top();  
    queue.pop();  
    cnt[cur.v]++;  
    if(cnt[cur.v] > K) continue; // 防止死循环，不联通时可能出现死循环
    if(cnt[t] == K) return cur.d;  
    for (int i =head[cur.v]; i != -1; i = Edge[i].nxt) {  
      queue.push(Node(Edge[i].v, cur.d + Edge[i].c));
    }  
  }  
  return -1;  
}  
  
void Read() {  
  int n,m;  
  while(scanf("%d %d", &n, &m)!=EOF ) {  
    int u,v,c;  
    memset(head, -1, sizeof(head));  
    memset(tail, -1, sizeof(tail));  
    for (int i = 0; i < m; i++) {  
      scanf("%d %d %d",&u,&v,&c );  
      Edge[(i << 1) + 0].v = v - 1;
      Edge[(i << 1) + 0].c = c;
      Edge[(i << 1) + 0].nxt = head[u - 1];
      head[u - 1] = (i << 1) + 0;

      Edge[(i << 1) + 1].v = u - 1;
      Edge[(i << 1) + 1].c = c;
      Edge[(i << 1) + 1].nxt = tail[v - 1];
      tail[v - 1] = (i << 1) + 1;
    }  
    int s,t,k;  
    scanf("%d %d %d",&s,&t,&k );  
    s -= 1;
    t -= 1;
    if(s == t) k++;  
    Dijstra(n, s, t);  
    printf( "%d\n", Astar(n, s, t, k));  
  }  
}
  
}  // namespace NB

// MLE
// 这个居然过不了，代码几乎和NB一模一样
//
namespace bak {
const int MAXE = 200100;
const int MAXN = 1010;
int N = 0;

int S = 0;
int T = 0;
int K = 0;

int dis[MAXN];
int vis[MAXN];
int cnt[MAXN];

struct Edge {
  int v;
  int c;
  int next;
  Edge() : v(-1), c(0), next(-1) {}
} E[MAXE];

int head[MAXN];
int tail[MAXN];

void Dijkstra() {
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < N; i++) dis[i] = INF;
  dis[T] = 0;
  for (int k = 1; k < N; k++) {
    int min = INF;
    int idx = -1;
    for (int i = 0; i < N; i++) {
      if (vis[i] == 0 && dis[i] < min) {
        min = dis[i];
        idx = i;
      }
    }
    if (idx == -1) break;
    vis[idx] = 1;
    for (int i = tail[idx]; i != -1 ; i = E[i].next) {
      int v = E[i].v;
      int c = E[i].c;
      if (vis[v] == 0 && dis[v] > dis[idx] + c) {
        dis[v] = dis[idx] + c;
      }
    }
  }
}

struct Node {
  int u;
  int val;
  Node(int i_u, int i_val) : u(i_u), val(i_val) {}
  bool operator < (const Node & x)  const {
    return val + dis[u] < x.val + dis[x.u];
  }
};

int AStar() {
  memset(cnt, 0, sizeof(cnt));
  Dijkstra();
  std::priority_queue<Node> queue;
  queue.push(Node(S, 0));
  while (!queue.empty()) {
    Node t = queue.top();
    queue.pop();
    cnt[t.u]++;
    if (cnt[t.u] > K) continue;
    if (cnt[T] == K) return t.val;  // 防止死循环
    for (int i = head[t.u]; i != - 1; i = E[i].next) {
      queue.push(Node(E[i].v, t.val + E[i].c));
    }
  }
  return -1;
}

void Read() {
  int n, m;
  scanf("%d%d", &n, &m);
  getchar();
  N = n;
  memset(head, -1, sizeof(head));
  memset(tail, -1, sizeof(tail));
  for (int i = 0; i < m; i++) {
    int a, b, t;
    scanf("%d%d%d", &a, &b, &t);
    getchar();
    E[(i << 1) + 0].v = b - 1;
    E[(i << 1) + 0].c = t;
    E[(i << 1) + 0].next = head[a - 1];
    head[a - 1] = (i << 1) + 0;
    
    E[(i << 1) + 1].v = a - 1;
    E[(i << 1) + 1].c = t;
    E[(i << 1) + 1].next = tail[b - 1];
    tail[b - 1] = (i << 1) + 1;
  }
  scanf("%d%d%d", &S, &T, &K);
  S -= 1;
  T -= 1;
  if (S == T) K++;
  printf("%d\n", AStar());
}

}  // namespace bak


// MLE
namespace algorithm {
const int MAXN = 1009;
int N = 0;

int matrix[MAXN][MAXN];
int tmatrix[MAXN][MAXN];

int S = 0;
int T = 0;
int K = 0;

int dis[MAXN];
int vis[MAXN];
int cnt[MAXN];

void Dijkstra(int source) {
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < N; i++) {
    if (i == source) dis[i] = 0;
    else if (tmatrix[source][i] > 0) dis[i] = tmatrix[source][i];
    else dis[i] = INF;
  }
  vis[source] = 1;
  for (int k = 1; k < N; k++) {
    int min = INF;
    int idx = -1;
    for (int i = 0; i < N; i++) {
      if (vis[i] == 0 && dis[i] < min) {
        min = dis[i];
        idx = i;
      }
    }
    if (idx == -1) break;
    vis[idx] = 1;
    for (int i = 0; i < N; i++) {
      if (tmatrix[idx][i] > 0 && dis[i] > dis[idx] + tmatrix[idx][i]) {
        dis[i] = dis[idx] + tmatrix[idx][i];
      }
    }
  }
}

struct Node {
  int u;
  int val;
  Node(int i_u, int i_val) : u(i_u), val(i_val) {}
};

bool operator < (const Node & x, const Node & y) {
  return (x.val + dis[x.u]) < (y.val + dis[y.u]);
}

int AStar() {
  memset(cnt, 0, sizeof(cnt));
  Dijkstra(T);
  std::priority_queue<Node> queue;
  queue.push(Node(S, 0));
  while (!queue.empty()) {
    Node t = queue.top();
    queue.pop();
    cnt[t.u]++;
    if (cnt[t.u] > K) continue;
    if (cnt[T] == K) return t.val;  // 防止死循环
    for (int i = 0; i < N; i++) {
      if (matrix[t.u][i] > 0) {
        queue.push(Node(i, t.val + matrix[t.u][i]));
      }
    }
  }
  return -1;
}

void Read() {
  int n, m;
  scanf("%d%d", &n, &m);
  getchar();
  N = n;
  memset(matrix, 0, sizeof(matrix));
  memset(tmatrix, 0, sizeof(tmatrix));
  for (int i = 0; i < m; i++) {
    int a, b, t;
    scanf("%d%d%d", &a, &b, &t);
    getchar();
    matrix[a - 1][b - 1] = t;
    tmatrix[b - 1][a - 1] = t;
  }
  scanf("%d%d%d", &S, &T, &K);
  S -= 1;
  T -= 1;
  if (S == T) K++;
  printf("%d\n", AStar());
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  NB::Read();
  return 0;
}
