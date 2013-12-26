// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-24 17:33:49
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
bool Find(std::vector<std::vector<int> > & young, int k) {
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  int i = 0;
  int j = n - 1;
  while (i < m && j >= 0) {
    if (young[i][j] == k) return true;
    else if (young[i][j] < k) i++;
    else j--;
  }
  return false;
}

void ReBuild(std::vector<std::vector<int> > & young, int x, int y) {
  int i = x;
  int j = y;
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  if (x + 1 < m && young[x + 1][y] < young[i][j]) {
    i = x + 1;
    j = y;
  }
  if (y + 1 < n && young[x][y + 1] < young[i][j]) {
    i = x;
    j = y + 1;
  }
  if (i != x || j != y) {
    std::swap(young[x][y], young[i][j]);
    ReBuild(young, i, j);
  }
}

int ExpectMin(std::vector<std::vector<int> > & young) {
  int rs = young[0][0];
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  young[0][0] = young[m - 1][n - 1];
  ReBuild(young, 0, 0);
  return rs;
}

// 返回前k小数的list,
// 此方法会破坏原始矩阵，不能重复调用
// k * log(m + n)
std::vector<int> MinKList(std::vector<std::vector<int> > & young, int k) {
  std::vector<int> rs;
  for (int i = 0; i < k; i++) {
    rs.push_back(ExpectMin(young));
  }
  return rs;
}

struct Node {
  int x;
  int y;
  int val;
  Node(int xi, int yi, int vi) : x(xi), y(yi), val(vi) {}
};

struct Cmp {
  bool operator()(const Node & x, const Node & y) const {
    return x.val > y.val;
  }
};

// 返回前k小数的list
// 不破华原始矩阵，但需要有一个额外的最小堆和标识矩阵
// O(k * log(2*k))
// 当然也可以备份下young矩阵,然后调用MinKList
std::vector<int> MinKListOp(std::vector<std::vector<int> > & young, int k) {
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  std::vector<int> rs;
  
  std::priority_queue<Node, std::vector<Node>, Cmp> queue;
  std::vector<std::vector<int> > flag(m, std::vector<int>(n, 0));
  queue.push(Node(0, 0, young[0][0]));
  flag[0][0] = 1;
  for (int i = 0; i < k; i ++) {
    Node t = queue.top();
    queue.pop();
    rs.push_back(t.val);
    int x = t.x;
    int y = t.y;
    if (x + 1 < m && flag[x + 1][y] == 0) {
      flag[x + 1][y] = 1;
      queue.push(Node(x + 1, y, young[x + 1][y]));
    }
    if (y + 1 < n && flag[x][y + 1] == 0) {
      flag[x][y + 1] = 1;
      queue.push(Node(x, y + 1, young[x][y + 1]));
    }
  }
  return rs;
}

int KthElement(std::vector<std::vector<int> > & young, int k) {
  std::vector<int> tmp = MinKList(young, k);
  // std::vector<int> tmp = MinKListOp(young, k);
  return tmp[k - 1];
}

int Count(std::vector<std::vector<int> > & young, int v) {
  int rs = 0;
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  int i = 0;
  int j = n - 1;
  while (i < m && j >= 0) {
    if (young[i][j] > v) {
      rs += m - i;
      j--;
    } else {
      i++;
    }
  }
  return rs;
}

// O(m + n) *log(max - min)
// 如果是两个数组A， B
// A[i] + B[j] 组成的数中找出第k大的数可以不使用额外的空间
// 因为young矩阵可以现算出来，但先要对数组AB排序
int KthElementOp(std::vector<std::vector<int> > & young, int k) {
  int m = young.size();
  int n = (m == 0) ? 0 : young[0].size();
  int b = young[0][0];
  int e = young[m - 1][n - 1];
  int mid = 0;
  while (true) {
    mid = b + (e - b) / 2;
    int t = Count(young, mid);
    if (t == k) break;
    else if (t > k) b = mid + 1;
    else e = mid - 1;
  }
  // mid 是稍微小于第k大的数，mid可能处于young中，也可能不在
  int i = 0;
  int j = n - 1;
  int rs = mid;
  while (i < m && j >= 0) {
    if (young[i][j] > mid) {
      rs = rs == mid ? young[i][j] : std::min(rs, young[i][j]);
      j--;
    } else {
      i++;
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  int n = 4;
  std::vector<std::vector<int> > v(n, std::vector<int>(n, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (i == n - 1) v[i][j] = (n + 1) * i + j;
      else v[i][j] = n * i + j;
    }
  }
  LOG(INFO) << JoinMatrix(&v);
  LOG(INFO) << Find(v, 10);
  LOG(INFO) << Find(v, 14);
  std::vector<int> sort = MinKListOp(v, 10);
  LOG(INFO) << JoinVector(sort);
  LOG(INFO) << JoinMatrix(&v);
  LOG(INFO) << KthElementOp(v, 10);
  return 0;
}
