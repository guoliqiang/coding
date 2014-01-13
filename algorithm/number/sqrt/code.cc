// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-29 23:58:37
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

/*
1156 = (30 + a)^2 = 900 + 30 * 2 * a + a^2
==>
256 = 3 * 20 * a + a^2 = (3 * 20 + a) * a
==> a = 4
*/

namespace algorithm {
// 手动开平方根,求平方根整数部分
// http://wenku.baidu.com/view/8e4088a7f524ccbff121845f
std::string SqrtByHand(std::string & str) {
  int n = str.size();
  std::string rs = "";
  int b = 0;
  b = str.size() % 2 == 0 ? 2 : 1;
  int cur = 0;  // 当前需要开平方的数
  if (b == 1) {
    cur = str[0] - '0';
  } else {
    cur = (str[0] - '0') * 10 + (str[1] - '0');
  }

  // cur >= (初商 * 20 + 试商) * 试商  且 试商尽可能大
  int r = 0;  // 初商
  while (true) {
    int i = 0;  // 试商
    // LOG(INFO) << "r:" << r << " cur:" << cur;
    while ((r * 20 + i) * i <= cur) i++;
    i--;
    // LOG(INFO) << "i:" << i << " r :" << r;
    rs.push_back('0' + i);
    if (b == n) break;
    cur -= (r * 20 + i) * i;  // 相当于新加入的数对当前的影响只有这么多
    r = r * 10 + i;  // 更新初商
    cur = cur * 100 + (str[b] - '0') * 10 + (str[b + 1] - '0');
    b += 2;
  }
  return rs;
}

// from pdf doc
void Sqrt(const char * str) {
  double i, r, n;
  int j, l, size, num, x[1000];
  size = strlen(str);
  if (size == 1 && str[0] == '0') {
    LOG(INFO) << 0;
    return;
  }
  if (size % 2 == 1) {
    n = str[0] - '0';
    l = - 1;
  } else {
    n = (str[0] - '0') * 10 + (str[1] - '0');
    l = 0;
  }
  r = 0;
  num = 0;
  while (true) {
    i = 0;
    while (i * (r * 20 + i) <= n) i++;
    LOG(INFO) << i;
    i--;
    n -= i * (i + 20 * r);
    r = r * 10 + i;
    x[num] = (int) i;
    num++;
    l += 2;
    if (l >= size) break;
    n = n * 100 + (double)(str[l] - '0') * 10 + (double)(str[l + 1] - '0');
  }
  for (j = 0; j < num; j++) {
    std::cout << x[j];
  }
  std::cout << std::endl;
}


// 牛顿迭代法求平方根,返回的是准确的平方根
// http://blog.csdn.net/dawnbreak/article/details/3308413
// x = (x + n/x) / 2
// 设r是f(x) = 0的根，选取x0作为r初始近似值，过点（x0,f(x0)）做曲线y = f(x)的切线L
// L的方程为y = f(x0)+f'(x0)(x-x0)，求出L与x轴交点的横坐标 x1 = x0-f(x0)/f'(x0)，称x1为r的一次近似值。
// 
// 也可以这样理解：
// x  > sqrt(n) 时，由于除以2的效果，其会被缩小
// x  < sqrt(n) 时，由于加n/rs的效果，其会被放大
// x == sqrt(n) 时，其值不变
//
double Newton(double n) {
  double rs = 1;
  while (fabs(rs * rs - n) > 1e-9) {
    rs = (rs + n / rs) / 2;
  }
  return rs;
}

// 直接二分查找
double Sqrt(double n) {
  double b = 0;
  double e = n;
  while (fabs(e - b) > 1e-9) {
    double mid = b + (e - b) / 2;
    if (fabs(n - mid * mid) < 1e-9) return mid;
    else if (mid * mid > n) e = mid;
    else b = mid;
  }
  return b;
}

// leetcode中利用二分查找搜索平方根整数部分
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << Newton(2);
  LOG(INFO) << Sqrt(2);
  return 0;
  std::string str = "1156";
  // LOG(INFO) << str << ":" << SqrtByHand(str);
  str = "10000";
  LOG(INFO) << str << ":" << SqrtByHand(str);
  // Sqrt(str.c_str());
  str = "1";
  LOG(INFO) << str << ":" << SqrtByHand(str);
  str = "0";
  LOG(INFO) << str << ":" << SqrtByHand(str);
  str = "923456789000845";
  LOG(INFO) << str << ":" << SqrtByHand(str);
  return 0;
}
