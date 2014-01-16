// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-05 19:42:21
// File  : code.cc
// Brief :

// http://www.cnblogs.com/Booble/archive/2010/10/09/1846911.html
// 线性表保存线段数需要开辟4*N的空间,因为有空间浪费，有些位置存储的是NULL指针应该占据的位置
// http://comzyh.tk/blog/archives/479/

#include "base/public/common_ojhead.h"

namespace algorithm {
const int MAXN = 1000;
int N;
// b, e必须有，其它的可根据需要增加相应的变量
struct Node{
  int b;
  int e;
  int cnt;
  Node (int bi = 0, int ei = 0) : b(bi), e(ei), cnt(0) {}
} data[MAXN * 4];

void Build(int b, int e, int k) {
  if (b <= e) {
    data[k].b = b;
    data[k].e = e;
    data[k].cnt = 0;
    if (b == e) return;
    int mid = b + (e - b) / 2;
    Build(b, mid, 2 * k);
    Build(mid + 1, e, 2 * k + 1);
  }
}

void Build(int b, int e) {
  memset(data, 0, sizeof(data));
  Build(b, e, 1);
}

int Count(int b, int e, int k) {
  if (data[k].b >= b && data[k].e <= e) return data[k].cnt;
  else {
    int mid = data[k].b + (data[k].e - data[k].b) / 2;
    if (e <= mid) return Count(b, e, 2 * k);
    else if (b > mid) return Count(b, e, 2 * k + 1);
    else return Count(b, mid, 2 * k) + Count(mid + 1, e, 2 * k + 1);
  }
}

int Count(int b, int e) {
  return Count(b, e);
}

void Insert(int v, int k) {
  if (data[k].b == v && data[k].e == v) {
    int t = k;
    while (t >= 1) {
      data[t].cnt++;
      t /= 2;
    }
  } else {
    int mid = data[k].b + (data[k].e - data[k].b) / 2;
    if (v <= mid) Insert(v, 2 * k);
    else Insert(v, 2 * k + 1);
  }
}

void Insert(int v) {
  Insert(v, 1);
}

void Delete(int v, int k) {
  if (data[k].b == v && data[k].e == v) {
    int t = k;
    while (t >= 1) {
      data[t].cnt--;
      t /= 2;
    }
  } else {
    int mid = data[k].b + (data[k].e - data[k].b) / 2;
    if (v <= mid) Delete(v, 2 * k);
    else Delete(v, 2 * k + 1);
  }
}

void Delete(int v) {
  Delete(v, 1);
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
