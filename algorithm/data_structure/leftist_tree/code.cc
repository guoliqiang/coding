// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 21:50:52
// File  : code.cc
// Brief :

/*
 * 左偏树：一种可并堆，满足堆的性质，根节点不大于字节点。并给出了一套堆合并
 * 的策略，合并过程中保证了树不至于变的太不平衡。
 *
 * 左偏树距离：该节点一直往右儿子走，到达空节点的距离。
 *
 * [性质1] 节点的键值小于或等于它的左右子节点的键值。
 * [性质2] 节点的左子节点的距离不小于右子节点的距离。
 * [性质3] 节点的左子节点右子节点也是一颗左偏树。
 *
 * 因为合并过程中总是和右子树进行合并的，如果没有性质2，未造成数的严重不平衡（右子树很大）
 * 
 * 堆在合并过程中不能直接合并，因为不能产生新的节点，因为元素的数量并没有发生
 * 改变
 * */


#include "base/public/common_head.h"

namespace algorithm {

int const MAX = 1000;
int N = 0;
int root = -1;

struct Node {
  int key;
  int l;
  int r;
  int f;
  int dis;
  Node(int k = 0) : key(k), l(-1), r(-1), f(-1), dis(0){}
} data[MAX];

int iroot(int k) {
  if (data[k].f == -1) return k;
  return iroot(data[k].f);
}

int merge(int x, int y) {
  if (x == -1) return y;
  if (y == -1) return x;
  if (data[x].key > data[y].key) std::swap(x, y);
  int t = merge(data[x].r, y);
  data[x].r = t;
  data[t].f = x;

  if (data[x].l == -1) {
    data[x].l = data[x].r;
    data[x].r = -1;
    data[x].dis = 0;
  } else {
    if (data[data[x].l].dis < data[data[x].r].dis) {
      std::swap(data[x].l, data[x].r);
      data[x].dis = data[data[x].r].dis + 1;
    }
  }
  return x;
}

int insert(int i, int key) {
  data[i].key = key;
  data[i].l = data[i].r = data[i].f = -1;
  root = merge(i, root);
  return root;
}

int del(int i) {
  int l = data[i].l;
  int r = data[i].r;
  int f = data[i].f;
  data[i].l = data[i].r = -1;
  int t = merge(l, r);
  data[t].f = f;
  if (f != -1 && data[f].r == i) data[f].r = t;
  if (f != -1 && data[f].l == i) data[f].l = t;
  while (f != -1) {
    if (data[data[f].l].dis < data[data[f].r].dis) {
      std::swap(data[f].l, data[f].r);
      data[f].dis = data[data[f].r].dis + 1;
      f = data[f].f;
    } else break;
  }
  root = iroot(t);
  return root;
}

int top() {
  return root == -1 ? -1 : data[root].key;
}

void pop() {
  if (root != -1) {
    int l = data[root].l;
    int r = data[root].r;
    data[root].l = data[root].r = data[root].f = -1;
    data[l].f = data[r].f = -1;
    root = merge(l, r);
  }
}

void Read(std::vector<int> & v) {
  N = v.size();
  for (int i = 0; i < N; i++) {
    insert(i, v[i]);
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(10);
  v.push_back(3);
  v.push_back(31);
  v.push_back(2);
  v.push_back(7);
  Read(v);
  while (root != -1) {
    LOG(INFO) << top();
    pop();
  }
  return 0;
}
