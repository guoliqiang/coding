// Copyright 2013 Jike Inc. All Rights Reserheaded.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-16 22:34:15
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
/*  can ac
namespace NB {

#define size 2100
#define esize 3000000
int head[size];
int head2[size];
int cnt;
int cnt2;

struct edge{
  int from, to, next;
} e[esize], e2[esize];

void insert(int from, int to) {
    e[cnt].from = from;
    e[cnt].to = to; 
    e[cnt].next = head[from];
    head[from] = cnt++;
}
void insert2(int from, int to) {
    e2[cnt2].from = from;
    e2[cnt2].to = to; 
    e2[cnt2].next = head2[from]; 
    head2[from] = cnt2++;
}

int index;
int dfn[size];
int low[size];
int sta[size];
int top;
int belong[size];
int cntnum;
int cf[size];
int rd[size];
int que[size];
int col[size];
bool ans[size];

int n;
char str[2][10];
struct elm {
  int from, to, len;
}x[size];


void tarjan(int id) {
  dfn[id] = low[id] = ++index;
  sta[top++] = id;
  int tmp = head[id];
  int min = size;
  while(tmp != -1) {
    if (!dfn[e[tmp].to]) {
      tarjan(e[tmp].to);
    };
    min = std::min(min, low[e[tmp].to]);
    tmp = e[tmp].next;
  }
  if (min < low[id]) low[id] = min;
  else {
    do {
      tmp = sta[--top];
      belong[tmp] = cntnum;
      low[tmp] = size;                                                                                                                                                  
    } while(tmp != id);
    cntnum++;
  }
}


bool solve(int n) {
  index = cntnum = top = 0;
  memset(dfn, 0, sizeof(dfn));
  for(int i = 0; i < 2 * n; i++) {
    if (!dfn[i]) tarjan(i);
  }

  for(int i = 0; i < n; i++) {
    if (belong[i] == belong[i + n]) return false;
    cf[belong[i]] = belong[i + n];
    cf[belong[i + n]] = belong[i];
  }
  memset(rd, 0, sizeof(rd));
  memset(head2, -1, sizeof(head2));
  memset(col, 0, sizeof(col));
  cnt2 = 0;
  for(int i = 0; i < cnt; i++) {
    if (belong[e[i].from] != belong[e[i].to]) {
      insert2(belong[e[i].to], belong[e[i].from]);
      rd[belong[e[i].from]]++;
    }
  }
  int head = 0, tail = 0;
  for(int i = 0; i < cntnum; i++) {
    if (rd[i] == 0) que[tail++] = i;
  }
  while(head < tail) {
    int tmp = que[head++];
    if (col[tmp] == 0) {
      col[tmp] = 1;
      col[cf[tmp]] = -1;
    }
    int id = head2[tmp];
    while(id != -1) {
      if (--rd[e2[id].to] == 0) {
        que[tail++] = e2[id].to;
      }
      id = e2[id].next;
    }
  }
  memset(ans, 0, sizeof(ans));
  for (int i = 0; i < 2 * n; i++) {
    if (col[belong[i]] == 1) ans[i] = 1;
  }
  return true;
}
int toint(int id) {
  int num = (str[id][0] - '0') * 10;
  num += (str[id][1] - '0'); num *= 60;
  num += (str[id][3] - '0') * 10 + str[id][4] - '0';
  return num;
}
bool judge(int from1, int len1, int from2, int len2) {
    return (from1 < from2 + len2 && from2 < from1 + len1);
}
void out(int xx) {
    printf("%02d:%02d", xx / 60, xx % 60);
}
int Read() {
  scanf("%d", &n);
  for(int i = 0; i < n; i++) {
    scanf("%s%s%d", str[0], str[1], &x[i].len);
    x[i].from = toint(0);
    x[i].to = toint(1);
  }
  memset(head, -1, sizeof(head));
  cnt = 0;
  for(int i = 0; i < n; i++) {
    for(int j = 0; j < n; j++) {
      if (i == j) continue;
      if (judge(x[i].from, x[i].len, x[j].from, x[j].len)) {
        insert(i, j + n);
      }
      if (judge(x[i].from, x[i].len, x[j].to - x[j].len, x[j].len)) {
        insert(i, j);
      }
      if (judge(x[i].to - x[i].len, x[i].len, x[j].from, x[j].len)) {
        insert(i + n, j + n);
      }
      if (judge(x[i].to - x[i].len, x[i].len, x[j].to - x[j].len, x[j].len)) {
        insert(i + n, j);
      }
    }
  }
  if(solve(n)) {
    printf("YES\n");
    for(int i = 0; i < n; i++)
    if (ans[i]) {
      out(x[i].from);
      putchar(' ');
      out(x[i].from + x[i].len);
      putchar('\n');
    } else {
      out(x[i].to - x[i].len);
      putchar(' ');
      out(x[i].to);
      putchar('\n');
    }
  } else {
    printf("NO\n");
  }
  return 0;
}

}  // namespace NB
*/

// WA
namespace algorithm {

const int MAXN = 1001;
int N = 0;

int matrix[MAXN * 2][MAXN * 2];
std::vector<int> matrix2[MAXN * 2];

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
  memset(matrix, 0, sizeof(matrix));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i == j) continue;
      if (Conflict(data[i].bi, data[i].t, data[j].bi, data[j].t)) {
        matrix[i][N + j] = 1;
      }
      if (Conflict(data[i].bi, data[i].t, data[j].ei - data[j].t, data[j].t)) {
        matrix[i][j] = 1;
      }
      if (Conflict(data[i].ei - data[i].t, data[i].t, data[j].ei - data[j].t, data[j].t)) {
        matrix[N + i][j] = 1;
      }
      if (Conflict(data[i].ei - data[i].t, data[i].t, data[j].bi, data[j].t)) {
        matrix[N + i][N + j] = 1;
      }
    }
  }
}

void Tarjan(int k) {
  dfsn[k] = low[k] = num++;
  stack[stop++] = k;
  int min = N * 2 + 10;
  for (int i = 0; i < N * 2; i++) {
    if (matrix[k][i] > 0) {
      if (dfsn[i] == -1) Tarjan(i);
      min = std::min(low[i], min);
    }
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
    id[stack[stop]] = tag++;
    low[stack[stop]] = N * 2 + 10;
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
  for (int i = 0; i < MAXN * 2; i++) matrix2[i].clear();
  for (int i = 0; i < N; i++) {
    other[id[i]] = id[N + i];
    other[id[N + i]] = id[i];
  }

  for (int i = 0; i < N * 2; i++) {
    for (int j = 0; j < N * 2; j++) {
      if (matrix[i][j] > 0 && id[i] != id[j]) {
        matrix2[id[j]].push_back(id[i]);
        du[id[i]]++;  // 自底向上的拓扑排序
      }
    }
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
    for (int i = 0; i < matrix2[t].size(); i++) {
      int to = matrix[t][i];
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
