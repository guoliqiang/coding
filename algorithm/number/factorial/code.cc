// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-30 02:18:30
// File  : code.cc
// Brief :
// OJ 不能用string或Ｔ

#include "base/public/common_head.h"

namespace algorithm {

int ZeroNum(int n) {
  int rs = 0;
  while (n) {
    rs += n / 5;
    n /= 5;
  }
  return rs;
}

// 5替换成1后阶乘的末尾
int a[] = {1, 1, 2, 6, 4, 4, 4, 8, 4, 6};
// 保留５的阶乘的末尾
int b[] = {1, 1, 2, 6, 4, 2, 2, 4, 2, 8};
// １～１0  11 ~ 20 21 ~ 30　．．．　中先把５的倍数提取了出来
//　先算这些数的阶乘末尾值，由于其每个段需要提取出两个２出来和提取出来的５
//　组成10, 可以看到 （（1 * 2 * 3 * 4 * 1 * 6 * 7 * 8 * 9） / 4） % 10 = 4
// 　所以这些段，末尾值为　４　＊ 4 * 4 * 4  .. 
//  (之所以除以4后余4，是因为由a知道，最末数为6，而这个阶乘是能被4整除的，所以结果一定是4, 或 9，而结果一定不是奇数，所以必然是4)
//  模10后是4，6，4，6，循环，也就是段个数为奇数时末尾是4，段个数是偶数是末尾是6
//  
//  最终结果是 ( k个段的末尾元素 * 不完整段的末尾元素 * Last(N / 5) ) % 10;
//  Last(N / 5) 是提取出的5个倍数在，剔除5后算阶乘的末尾元素
//
// http://blog.csdn.net/anchor89/article/details/5913246
//
int head[2] = {4, 6};

int Last(std::string str) {
  // LOG(INFO) << "str:" << str;
  int n = str.size();
  int tail = str[n - 1] - '0';
  if (tail < 5) tail = a[tail];
  else {
    tail = b[tail];
  }
  if (n == 1) return tail;
  std::string tmp = "";
  int b = 0;
  int cur = 0;
  if (str[0] < '5') {
    cur = str[0] - '0';
    b = 1;
  }
  for (int i = b; i < n; i++) {
    cur = cur * 10 + (str[i] - '0');
    tmp.push_back(cur / 5 + '0');
    cur %= 5;
  }
  int t = head[(str[n - 2] - '0') % 2 == 0 ? 1 : 0];
  return (tail * t * Last(tmp)) % 10;
}

int LastOJ(const char * str) {
  int n = strlen(str);
  int tail = str[n - 1] - '0';
  if (tail < 5) tail = a[tail];
  else {
    tail = b[tail];
  }
  if (n == 1) return tail;
  char  tmp[1005];
  int j = 0;
  int b = 0;
  int cur = 0;
  if (str[0] < '5') {
    cur = str[0] - '0';
    b = 1;
  }
  for (int i = b; i < n; i++) {
    cur = cur * 10 + (str[i] - '0');
    tmp[j++] = cur / 5 + '0';
    cur %= 5;
  }
  tmp[j] = '\0';
  int t = head[(str[n - 2] - '0') % 2 == 0 ? 1 : 0];
  return (tail * t * LastOJ(tmp)) % 10;
}

void Read() {
  char n[10005];
  while (gets(n)) {
    printf("%d\n", LastOJ(n));
  }
}

/*
 * 求n！的位数
 * 求n的阶乘的位数即求log10(n!)=log10(1)+log10(2)+......log10(n);
 * 《计算机程序设计艺术》中给出了另一个公式
 *  n! = sqrt(2*π*n) * ((n/e)^n) * (1 + 1/(12*n) + 1/(288*n*n) + O(1/n^3))
 *  π = acos(-1)
 *  e = exp(1)
 *  两边对10取对数
 *  忽略log10(1 + 1/(12*n) + 1/(288*n*n) + O(1/n^3)) ≈ log10(1) = 0
 *  得到公式
 *  log10(n!) = log10(sqrt(2 * pi * n)) + n * log10(n / e)。
 * */

int Long(int n) {
  double e = exp(1.0);
  double pi = acos(-1.0);
  int rs = int(log(sqrt(2 * pi * n)) / log(10.0) + n * (log(n / e) / log(10.0))) + 1;
  return rs;
}

}  // namespace algorithm



using namespace algorithm;


int main(int argc, char** argv) {
  // LOG(INFO) << ZeroNum(26);
  // std::string str = "9999999";
  // LOG(INFO) << str << ":" << Last(str);
  Read();
  return 0;
}
