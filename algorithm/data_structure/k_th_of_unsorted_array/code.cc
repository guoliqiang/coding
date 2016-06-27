// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 04:13:05
// File  : code.cc
// Brief :

/*
注意:
基于快速排序的算法时间复杂度是O(n)

*/
#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int data[MAX] = {0};

int Kth(int k, int b, int e) {
  if (k > e) return -1;
  int mid = b + (e - b) / 2;
  std::swap(data[mid], data[e]);
  int t = -1;
  for (int i = b; i < e; i++) {
    if (data[i] < data[e]) std::swap(data[i], data[++t]);
  }
  std::swap(data[++t], data[e]);
  if (t == k) return data[t];
  else {
    if (t > k) return Kth(k, b, mid);
    else {
      return Kth(k - t - 1, mid + 1, e);
    }
  }
}

int Kth(int k) {
  return Kth(k - 1, 0, N - 1);
}

void Read(std::vector<int> & v) {
  memset(data, 0, sizeof(data));
  N = v.size();
  for (int i = 0; i < N; i++) {
    data[i] = v[i];
  }
}
}  // namespace algorithm

// a intresting algorithm
// 中位数，就是数组排序后处于数组最中间的那个元素。说来有些麻烦，如果数组长度是
// 奇数，最中间就是位置为（n+1）／2的那个元素。如果是偶数呢，标准的定义是位置为
// n/2和位置为n/2+1的两个元素的和除以2的结果，有些复杂。为了解答的方便，
// 我们假设数组长度暂且都为奇数吧。
//
// 面试时，大家是不是经常被问到，怎么求一个无序数组（长度为n）的中位数？
//
// 不假思索的算法是，首先将数组排序，然后直接从排序数组中找出中位数。
// 这个算法的复杂度是O（nlogn），就是排序的复杂度。当然，答案是有了，
// 但不会impress面试官的：）。but it is ok, 如果你能写出代码。
//
// 另外一个优化的快速算法是快速中位数算法，类似于快速排序，
// 采用的是分而治之的思想。基本思路是：任意挑一个元素，以该元素为支点，
// 将数组分成两部分，左部分是小于等于支点的，右部分是大于支点的。如果你的运气
// 爆棚，左部分正好是（n－1）／2个元素，那么支点的那个数就是中位数。否则的话，
// 相信你知道怎么推理了。写出没有bug的代码还是需要一点点功力的。作为家庭作业好了
//
// 这里，给大家介绍一个让人眼前一亮的算法，至少，看着很美妙，可以细细品味。
// 算法的核心是使用最小堆（heap），你想到了吗？
//
// 。。。思考ing 。。。
//
// 首先将数组的前（n+1）／2个元素建立一个最小堆。
//
// 然后，对于下一个元素，和堆顶的元素比较，如果小于等于，丢弃之，
// 接着看下一个元素。如果大于，则用该元素取代堆顶，再调整堆，接着看下一个元素。
// 重复这个步骤，直到数组为空。
//
// 当数组都遍历完了，那么，堆顶的元素即是中位数。
//
// 可以看出，长度为（n＋1）／2的最小堆是解决方案的精华之处。如果你没看过想到了，
// 面试官会有一点点surprised：）
//
// 接下来，你是否可以完成这个算法的code？
//
// https://mp.weixin.qq.com/s?__biz=MjM5ODIzNDQ3Mw==&mid=2649965551&idx=1&sn=bc769eb3fbd2f4075c58524f4cc8332d&scene=0&key=77421cf58af4a6536088b032cad8fe69930bbffceac20ffce6464c29f41f5e8dec753f69badd1e357c7997fa0aed178e&ascene=7&uin=MjEwMTkyMTEyMA%3D%3D&devicetype=android-19&version=26031233&nettype=WIFI&pass_ticket=06SVKWHRvqCEz1WijocH1fhm9EpJYk%2F4BSsSn7u0Ftt1coB9ZRYNdDNeStCg250q
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(10);
  v.push_back(3);
  v.push_back(5);
  v.push_back(1);
  Read(v);
  LOG(INFO) << Kth(1);
  LOG(INFO) << Kth(3);
  return 0;
}
