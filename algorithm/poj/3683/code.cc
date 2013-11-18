// Copyright 2013 Jike Inc. All Rights Reserheaded.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-16 22:34:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

// RE
namespace algorithm {

const int MAXN = 1001;
const int MAXM = 3000000;
int N = 0;

int head[MAXN * 2];
int head2[MAXN * 2];
int cnt;
int cnt2;

struct edge{
  int from, to, next;
} e[MAXM], e2[MAXN];

void Insert1(int from, int to) {
    e[cnt].from = from;
    e[cnt].to = to; 
    e[cnt].next = head[from];
    head[from] = cnt++;
}
void Insert2(int from, int to) {
    e2[cnt2].from = from;
    e2[cnt2].to = to; 
    e2[cnt2].next = head2[from]; 
    head2[from] = cnt2++;
}

int stack[MAXN * 2];
int queue[MAXN * 2];
int id[MAXN * 2];
int dfsn[MAXN * 2];
int low[MAXN * 2];

int tag = 0;
int stop = 0;
int num = 0;
int ans[MAXN * 2];
int du[MAXN * 2];
int other[MAXN * 2];

struct Node {
  int bi;
  int ei;
  int t;
} data[MAXN];

// NB
// 判断两个线段是否有重叠部分的方法
bool Conflict(int b1, int t1, int b2, int t2) {
  return (b1 < b2 + t2 && b2 < b1 + t1);
}

void Build() {
  memset(head, -1, sizeof(head));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) continue;
      if (Conflict(data[i].bi, data[i].t, data[j].bi, data[j].t)) {
        Insert1(i, N + j);
      }
      if (Conflict(data[i].bi, data[i].t, data[j].ei - data[j].t, data[j].t)) {
        Insert1(i, j);
      }
      if (Conflict(data[i].ei - data[i].t, data[i].t, data[j].ei - data[j].t, data[j].t)) {
        Insert1(N + i, j);
      }
      if (Conflict(data[i].ei - data[i].t, data[i].t, data[j].bi, data[j].t)) {
        Insert1(N + i, N + j);
      }
    }
  }
}

void Tarjan(int k) {
  dfsn[k] = low[k] = num++;
  stack[stop++] = k;
  int min = N * 2 + 10;
  for (int i = head[k]; i != -1; i = e[i].next) {
    int v = e[i].to;
    if (dfsn[v] == -1) {
      Tarjan(v);
    }
    min = std::min(low[v], min);
  }
  if (min < low[k]) {
    low[k] = min;
  } else {
    stop -= 1;
    while (stack[stop] != k) {
      id[stack[stop]] = tag;
      low[stack[stop]] = N * 2 + 10;
      stop--;
    }
    id[k] = tag++;
    low[k] = N * 2 + 10;
  }
}

void Tarjan() {
  memset(dfsn, -1, sizeof(dfsn));
  memset(id, -1, sizeof(id));
  memset(stack, 0, sizeof(stack));
  memset(low, 0, sizeof(low));
  stop = 0;
  tag = 0;
  num = 0;
  for (int i = 0; i < N * 2; i++) {
    if (dfsn[i] == -1) Tarjan(i);
  }
}

bool Check() {
  for (int i = 0; i < N; i++) {
    if (id[i] == id[i + N]) return false;
  }
  return true;
}

void Out(int k) {
  printf("%02d:%02d", k / 60, k % 60);
}

void Answer() {
  memset(du, 0, sizeof(du));
  memset(ans, 0, sizeof(ans));
  memset(other, 0, sizeof(other));
  memset(head2, -1, sizeof(head2));
  for (int i = 0; i < N; i++) {
    other[id[i]] = id[N + i];
    other[id[N + i]] = id[i];
  }

  for (int i = 0; i < cnt; i++) {
    int u = e[i].from;
    int v = e[i].to;
    if (id[u] == id[v]) continue;
    Insert2(id[v], id[u]);
    du[id[u]]++;
  }

  int tail = 0;
  for (int i = 0; i < tag; i++) {
    if (du[i] == 0) {
      queue[tail++] = i;
    }
  }
  int k = 0;
  while (k < tail) {
    int t = queue[k++];
    if (ans[t] == 0) {
      ans[t] = 1;
      ans[other[t]] = -1;
    }
    for (int i = head2[t]; i != -1; i = e2[i].next) {
      int to = e2[i].to;
      du[to]--;
      if (du[to] == 0) queue[tail++] = to;
    }
  }
  
  for (int i = 0; i < N; i++) {
    if (ans[id[i]] == 1) {
      Out(data[i].bi);
      printf(" ");
      Out(data[i].bi + data[i].t);
    }
    if (ans[id[N + i]] == 1) {
      Out(data[i].ei - data[i].t);
      printf(" ");
      Out(data[i].ei);
    }
    printf("\n");
  }
}

void Read() {
  int n = 0;
  while (scanf("%d", &n) != EOF) {
    N = n;
    getchar();
    char b[10];
    char e[10];
    int t;
    for (int i = 0; i < n; i++) {
      scanf("%s%s%d", b, e, &t);
      getchar();
      data[i].t = t;
      data[i].bi = ((b[0] - '0') * 10 + (b[1] - '0')) * 60 + (b[3] - '0') * 10 + (b[4] - '0');
      data[i].ei = ((e[0] - '0') * 10 + (e[1] - '0')) * 60 + (e[3] - '0') * 10 + (e[4] - '0');
    }
    Build();
    Tarjan();
    if (Check()) {
      printf("YES\n");
      Answer();
    } else {
      printf("NO\n");  
    }
  }
}

}  // namespace algorithm

using namespace algorithm;
// using namespace NB;

int main(int argc, char** argv) {
  FROMFILE;
  Read();
  return 0;
}
