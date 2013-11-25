// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 16:51:28
// File  : code.cc
// Brief :

/*
 * Write a method to randomly generate a set of m integers from an array of size n.
 * Each element must have equal probability of being chosen.
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {
// 在不给定n大小情况下的方法：
// special case : m == 1
// 第i个出现在结果中，其概率为
// m/i * i/(i +1) * (i + 1) / (i + 2) *...* (n-1)/n
void SelectRand(std::vector<int> & v, int m) {
  int n = v.size();
  for (int i = m; i < n; i++) {
    int t = rand() % i + 1;
    if (t < m) std::swap(v[i], v[t]);
  }
  std::sort(v.begin(), v.begin() + m);
}


/* 要求：有序列表， 不允许重复。
 *
 * 大众方法， 必须提前知道n的大小
 *
 * 这道题目和随机洗牌问题类似，只需要随机选取1个元素， 然后在剩下的元素里面随机选取下一个元素，不断这样操作即可。
 * 这样做能保证每个元素选中的概率一样吗？也就是选中每个元素的概率都是1/n？ 答案是YES，让我们来做一下简单的计算。
 * 选第1个元素：在n个中随机选，因此概率为1/n
 * 选第2个元素：在剩下的n-1个中随机选：1/(n-1)，由于第1次没有选中它，
 *              而是在另外n-1个中选：(n-1)/n，因此概率为：(n-1)/n * 1/(n-1) = 1/n
 * 选第3个元素：同上：(n-1)/n * (n-2)/(n-1) * 1/(n-2) = 1/n
 *
 * 由于第一个元素可以在m中任意一个位置被选中所以其概率实际为 m/n
 * Ashley Shepherd和Alex Woronow发现，在这个问题中我们只需要打乱数组的前m个元素，然后再取前m个元素，就可以达到随机性。  
 * */
void SelectShuff(std::vector<int> &v, int m) {
  int n = v.size();
  for (int i = 0; i < m; i++) {
    int t = rand() % (n - i) + i;
    std::swap(v[i], v[t]);
  }
  std::sort(v.begin(), v.begin() + m);
}

/* http://m.blog.csdn.net/blog/huagong_adu/7617498
 * 第一个元素被选择的概率为 m/n
 * 第二个元素被选择的概率为：
 *   [(m - 1) / (n - 1)] * [m / n]  // 第一个元素被选中
 * + [m / n -1] * [(n - m) / n]     // 第一个元素未被选中
 * = m/n
 *
 * 所有元素被选择的概率均为m/n
 * */
// 时间负载度和n成正比,当n很大时效率比较低
std::vector<int> SelectKunuth(std::vector<int> & v, int m) {
  std::vector<int> rs;
  int n = v.size();
  // 可以保证总能选出m个元素
  for (int i = 0; i < v.size(); i++) {
    if (rand() % (n - i) < m) {
      rs.push_back(v[i]);
      m--;
    }
  }
  return rs;
}

// 这个方法最容易记忆
// 时间负载度之和m有关
// 这个算法的主要问题是，如果抛弃已存在的元素的次数过多，相当于多次产生随机数并进行集合操作，
// 性能将明显下降。比如当n=100而m=99，取第99个元素时，算法“闭着眼睛乱猜整数，直到偶然碰上正确的那个为止”
// http://www.cnblogs.com/wuyuegb2312/archive/2013/06/29/3141292.html
std::set<int> SelectSets(int n, int m) {
  std::set<int> rs;
  while (rs.size() < m) {
    rs.insert(rand() % n);  // 已经存在的回insert失败
  }
  return rs;
}
/*
 * http://book.51cto.com/art/201104/258180.htm
 * 证明：
 * Floyd开始寻找一个算法，对于S中每个随机数只恰好调用一次RandInt。Floyd算法的结构很容易递归地理解：
 * 为了从1..10中产生一个5元素样本，首先从1..9中产生一个4元素样本，然后加上第5个元素。这个递归算法概述为算法F1。
 * 我们用举例的方式来验证一下算法F1的正确性。当M=5且N=10时，算法先递归地在S中计算1..9范围内的
 * 一个4元素随机样本，然后给T指定一个1..10内的随机整数。在T的10个可能取值中，恰好有下列5个值使得10被放入S中：
 * S中已有的4个值以及10本身。因此，元素10以5/10的正确概率被放入S中。
 *
 * 我的证明
 * 
 * 倒数第一个被选中的概率：1/n + (m - 1)/n = n/m
 *
 * 倒数第二个数被选中的概率为：
 * 倒数第二次选择时就被选中  (m - 1)/ (n - 1)   +
 * 倒数第一次选择时被选中的  (n - 1 - (m - 1)) / (n - 1) * 1/n
 *
 * ==>  (m - 1)/ (n - 1) + (n - m) / (n - 1) * n  = m/n
 * 依次递归
 *
 * 证明完毕
 *
 * 相当于先给每个位置都找好一个备胎，这样一定能选出m个数，
 * 关键是选择更好的元素概率+不得不选择备胎的概率 = m/n
 *
 * */

std::set<int> SelectFloyd(int n, int m) {
  std::set<int> rs;
  for (int i = n - m; i < n; i++) {
    int t = rand() % (i + 1);
    if (!rs.count(t)) rs.insert(t);
    else rs.insert(i);
  }
  return rs;
}


}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  SelectRand(v, 3);
  LOG(INFO) << JoinVector(v);
  return 0;
}
