// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-29 01:22:08
// File  : code.cc
// Brief :
// http://wenku.baidu.com/view/348fc03231126edb6f1a1082.html
// http://wenku.baidu.com/view/f2de8deb172ded630b1cb61c.html
// http://wenku.baidu.com/view/15e00a0002020740be1e9bc6.html

/*
 * 首先不考虑任何置换（翻转或旋转）能得到的所有染色的情况共 s^c中，作为一个
 * 集合Ｃ，构造此集合上的一个置换群，群中个每个元素表示在Ｃ上的一次翻转或旋转
 * 这次翻转或旋转可能存在有不动点，比如所有点染同一元素的图（s^c个图中的一个）
 * 无论怎样翻转或旋转都不会变换成其它图。
 * 根据Burnside定理，等价类的个数（也就是不同染色的结果数，旋转或翻转后一样的图属于同一个等价类）
 * 等于：
 * sum(当前变换中不动点个数)　／　群的大小(==可以变换的种数)
 *
 *
 * polya定理进一步简化了上述计算步骤：
 * 首先求当前图所有置换（翻转或旋转），用循环对表示，比如 (1,3)(2,4) 其循环节为：2 (也就是有一个循环)
 * polya定理指出同一个循环内染同一种颜色就是一个不动点，因此当前置换得到的不动点书目为c^循环节数
 * 这样就可以计算出共有多少个不动点，最后除以置换的数目。
 *
 *
 * 思考：结果数包含两种情况（两种等价类），一种是当前等价类中只有一个元素（比如所有点都染了同一种颜色，其所在的
 * 等价类就它自己，因为无法通过它翻转或旋转转换成其它图），第二种是等价类中含有多个元素。
 *
 * 第一种情况在每个置换中都是不动点，所以其加了m次（m等于群的大小）
 * 第二种情况在不置换中加了n次（n是其等价类的大小, 通过上面的计算可以推算出n = m, 自己的猜测可能不对）
 * 所以最后需要除以群的大小.
 * */
#include "base/public/common_head.h"

namespace algorithm {

int gcd(int x, int y) {
  return y == 0 ? x : gcd(y, x % y);
}

// poj 1286
int polya(int s, int c) {
  std::vector<long long int> p(s + 1, 0);
  // p[i] 存储 pow(c, i)
  p[0] = 1;
  for (int i = 0; i < s; i++) {
    p[i + 1] = p[i] * c;
    // LOG(INFO) << "p[" << i + 1 << "]:" << p[i + 1];
  }
  // 翻转情况循环节为：
  // s为奇数： 1 + (s - 1) / 2  == s/2 + 1
  // 此时共ｓ个
  //
  // s偶数：   沿着某一点和圆心的连线翻转：1 + 1 + (s - 2) / 2  == s/2 + 1
  // 　　　　　此时共s / 2 个
  // 　　　　　沿着平分多有点的对称线翻转：s /2
  // 　　　　　此时共 s / 2 个
  // 此时循环节显然为 s/2 + 1, 因为对于abc,沿b与圆心连线翻转后的置换可能是这样的(ac)(b)
  long long count = s & 1 ? s * p[s / 2 + 1] :
                    s / 2 * (p[s / 2] + p[s / 2 + 1]);
  // LOG(INFO) << count;
  // 考虑旋转的情况
  for (int i = 1; i <= s; i++) {
    // LOG(INFO) << "count:" << count << " + p[gcd(" << i << ", " << s << ")]: p[" << gcd(i, s) << "]:" << p[gcd(i, s)];
    // gcd 是旋转ｉ后其置换的循环节
    count += p[gcd(i, s)];  // i = s时，旋转s此即不动的情况
  }
  count /= 2 * s;
  // 2*s 不同置换的个数,情况1：旋转，情况2：翻转
  return count;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  LOG(INFO) << polya(3, 2);
  return 0;
}

