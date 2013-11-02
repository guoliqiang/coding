// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-31 05:10:09
// File  : code.cc
// Brief :


// 平面最近点对
// http://blog.csdn.net/zhulei19931019/article/details/9362087
// jlu模板上的代码只需要对y排序一次


// 高维数据,选择的时候，要选出n - 1维每一维距离mid都在dm中的点. 
// 论文：http://wenku.baidu.com/view/2061b121ccbff121dd3683c9.html1
// 排序时按照y轴z轴...的次序
//
// O(nlog(n)log(n))
//
// 点是动态加入的情况
// KD树 http://blog.csdn.net/qll125596718/article/details/8426458
// 每加入一个点重新计算一次
// (KD树：求指定点的最近k邻)
//
// 高维空间最近点对问题的ε网算法 王晓东，傅清祥,福州大学
// http://wenku.baidu.com/link?url=Q7x1fNT-2sJc3boC7A4vihshpEwpDahVYKnEEw-TZ1JAMj8FkQT3iT489Q7sJc7xfLLzHi0jo7k9IeSos6frElCrMa3S9xvg5La4QtmXHs7
//

// 平面最远点对
// 1. 先求凸包
// 2. 再求凸包的直径

#include "base/public/common_head.h"
#include "../base/base.h"

namespace algorithm {

const int MAX = 1000;
Point vec[MAX];
int index_x[MAX];
int index_y[MAX];


bool Cmpx(int a, int b) {
  return (vec[a].x < vec[b].x) ||
         (vec[a].x == vec[b].x && vec[a].y < vec[b].y);
}

bool Cmpy(int a, int b) {
  return (vec[a].y < vec[b].y) ||
         (vec[a].y == vec[b].y && vec[a].x < vec[b].x);
}

double Closest(int * v, int n) {
  if (n <= 1) return 0;
  if (n == 2) {
    int t = Distance(vec[v[0]], vec[v[1]]);
    return t;
  }
  if (n == 3) {
    double t = Distance(vec[v[0]], vec[v[1]]);
    t = std::min(t, Distance(vec[v[1]], vec[v[2]]));
    t = std::min(t, Distance(vec[v[0]], vec[v[2]]));
    return t;
  }
  int mid = n / 2;
  // 二分过程中需要保证左右两部分元素要么元素数相同，要么元素数差1
  double dml = Closest(v, mid);
  double dmr = Closest(v + mid, n - mid);
  double dm = std::min(dml, dmr);
  
  // 选出mid左右距离在dm之内的点，只有在这个子集内
  // 按y排序后，每个点最多和它之后的6个点比较就能
  // 判断出有没有更小的距离,如果有重复的点，比较
  // 的次数可能多于六个
  //
  // 比如当前距离当前点距离小于dm的一定在一个长度为dm，宽度为2dm的矩形
  // 中，分成相等的六块（2 * 3,因为分成4块无法证明），根据鸽巢原理，如果
  // 存在多于6个点在这个矩形中，必有两个点处于同一个小块中，其距离回小于dm
  int k = 0;
  int * ptr = index_y;
  for (int i = 0; i < n; i++) {
    if (abs(vec[v[i]].x - vec[v[mid]].x) < dm) ptr[k++] = v[i];
  }
  std::sort(ptr, ptr + k, Cmpy);
  for (int i = 0; i < k; i++) {
     LOG(INFO) << "(" << vec[ptr[i]].x << "," << vec[ptr[i]].y << ")";
  }
  HERE(INFO);
  
  for (int i = 0; i < k; i++) {
    for (int j = i + 1; j < k && vec[ptr[j]].y - vec[ptr[i]].y < dm; j++) {
      double t = Distance(vec[ptr[j]], vec[ptr[i]]);
      // if (t < dm) LOG(INFO) << "find min " << t << " " << vec[ptr[j]].x << ","
      //                       << vec[ptr[j]].y << " " << vec[ptr[i]].x << " " 
      //                       << vec[ptr[i]].y;
      dm = std::min(dm, t);
    }
  }
  return dm;
}

double Read(std::vector<Point> & v) {
  memset(index_x, 0, sizeof(index_x));
  memset(index_y, 0, sizeof(index_y));
  int n = v.size();
  for (int i = 0; i < n; i++) {
    vec[i] = v[i];
    index_x[i] = i;
  }
  int * ptr = index_x;
  std::sort(ptr, ptr + n, Cmpx);
  for (int i = 0; i < n; i++) {
    LOG(INFO) << "[" << vec[ptr[i]].x << "," << vec[ptr[i]].y << "]";
  }
  return Closest(ptr, n);
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<Point> v;
  v.push_back(Point(1, 1.5));
  v.push_back(Point(6, 0.5));
  v.push_back(Point(0.5, 6.5));
  v.push_back(Point(4, 8.5));
  v.push_back(Point(2, 5));
  v.push_back(Point(-2, 5));
  v.push_back(Point(10, 5));
  v.push_back(Point(21, -9));
  v.push_back(Point(-1, 4));
  v.push_back(Point(3, 8));
  v.push_back(Point(4, -3));
  v.push_back(Point(12, -2));
  v.push_back(Point(7, -5));
  v.push_back(Point(0, 0));
  LOG(INFO) << Read(v);
  return 0;
}
