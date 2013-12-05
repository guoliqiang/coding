// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-15 04:43:09
// File  : code.cc
// Brief :

/*
 * Accepted -784K 0MS
 *
 * 正常的思路是满足上下界的流就是最小流（即反悔边的流量）,  之所以要先做一次最大流
 * 是为了处理图存在有环的情况，解释见：http://www.cnblogs.com/kane0526/archive/2013/04/05/3001108.html
 *
 *
 * 而有源汇有上下界最小流问题则是：
 * 1.构造附加网络(不添加[t,s]边)
 * 2.对ss、tt求最大流 (因为有可能出现环)
 * 3.添加[t,s]边
 * 4.对ss、tt求最大流
 * 5.若ss、tt满流，则[t,s]的流量就是最小流
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 56;
int N = 0;
int sst = 54;
int ssd = 55;
int flow[MAXN][MAXN];
int low[MAXN][MAXN];
int du[MAXN];
int visited[MAXN];
int pre[MAXN];

int  MaxFlow(int s, int t) {
  int rs = 0;
  while (true) {
    memset(visited, 0, sizeof(visited));
    memset(pre, 0, sizeof(pre));
    std::queue<int> queue;
    queue.push(s);
    visited[s] = 1;
    while (!queue.empty()) {
      int cur = queue.front();
      queue.pop();
      if (cur == t) break;
      for (int i = 0; i < MAXN; i++) {
        if (flow[cur][i] > 0 && visited[i] == 0) {
          visited[i] = 1;
          queue.push(i);
          pre[i] = cur;
        }
      }
    }
    if (visited[t] == 0) break;
    int min = INF;
    for (int i = t; i != s; i = pre[i]) {
      min = std::min(min, flow[pre[i]][i]);
    }
    for (int i = t; i != s; i = pre[i]) {
      flow[pre[i]][i] -= min;
      flow[i][pre[i]] += min;
    }
    rs += min;
  }
  return rs;
}

void Read() {
  int n, m;
  while (scanf("%d%d", &n, &m) != EOF && (n + m)) {
    getchar();
    memset(flow, 0, sizeof(flow));
    memset(low, 0, sizeof(low));
    memset(du, 0, sizeof(du));
    N = n + 2;
    int st = N - 2;
    int sd = N - 1;
    for (int i = 0; i < m; i++) {
      int u, v, c;
      char from[10];
      char to[10];
      scanf("%s%s%d", from, to, &c);
      if (from[0] == '+') u = sd;
      else if (u == '-') u = st;
      else {
        sscanf(from, "%d", &u);
        u--;
      }

      if (to[0] == '+') v = sd;
      else if (to[0] == '-') v = st;
      else {
        sscanf(to, "%d", &v);
        v--;
      }
      
      flow[v][u] = INF - c;
      // LOG(INFO) << "flow[" << v << "][" << u << "]:" << flow[v][u];
      du[v] -= c;
      du[u] += c;
      low[v][u] = c;
    }
    for (int i = 0; i < N; i++) {
      if (du[i] > 0) {
        flow[sst][i] = du[i];
        // LOG(INFO) << "flow[" << sst << "][" << i << "]:" << flow[sst][i];
      } else if (du[i] < 0) {
        flow[i][ssd] = -du[i];
        // LOG(INFO) << "flow[" << i << "][" << ssd << "]:" << flow[i][ssd];
      }
    }
    MaxFlow(sst, ssd);
    flow[sd][st] = INF;
    MaxFlow(sst, ssd);
    bool flag = true;
    for (int i = 0; i < N; i++) {
      if (flow[sst][i] > 0) {
        flag = false;
        break;
      }
    }

    if (flag == false) {
      printf("impossible\n");
    } else {
      printf("%d\n", INF - flow[sd][st]);  // 反悔边的流量
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
