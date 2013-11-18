// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-18 00:10:50
// File  : code.cc
// Brief :

/*
 * Accepted 4880K 4219MS
 *
 * */

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 100010;
int N = 0;
struct Node {
  double x;
  double y;
  int type;
} data[MAXN * 2];

bool Cmpx(const Node & a, const Node & b) {
  return a.x < b.x;
}

bool Cmpy(const Node & a, const Node & b) {
  return a.y < b.y;
}

double Dis(const Node & a, const Node & b) {
  double t = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
  return sqrt(t);
}

double Clost(int b, int e) {
  if (b == e) return INF;
  int k = 0;
  for (k = b + 1; k <= e; k++) {
    if (data[k].type != data[b].type) break;
  }
  if (k > e) return INF;
  if (b == e - 1) {
    return Dis(data[b], data[e]);
  } else {
    int mid = b + (e - b) / 2;
    double min = std::min(Clost(b, mid), Clost(mid + 1, e));
    std::vector<Node> vec;
    for (int i = b; i <= e; i++) {
      if (fabs(data[i].x - data[mid].x) < min) vec.push_back(data[i]);
    }
    std::sort(vec.begin(), vec.end(), Cmpy);
    for (int i = 0; i < vec.size(); i++) {
      for (int j = i + 1; j < vec.size() && (vec[j].y - vec[i].y < min); j++) {
        if (vec[i].type == vec[j].type) continue;
        double t = Dis(vec[i], vec[j]);
        min = std::min(t, min);
      }
    }
    return min;
  }
}

double Clost() {
  std::sort(data, data + N, Cmpx);
  return Clost(0, N - 1);
}

void Read() {
  int c = 0;
  while (scanf("%d", &c) != EOF) {
    getchar();
    for (int i = 0; i < c; i++) {
      int n = 0;
      scanf("%d", &n);
      getchar();
      N = n * 2;
      for (int i = 0; i < n * 2; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        getchar();
        data[i].x = x;
        data[i].y = y;
        data[i].type = i < n ? 0 : 1;
      }
      double rs = Clost();
      printf("%.3f\n", rs);
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

