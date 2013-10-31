// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-31 01:41:16
// File  : code.cc
// Brief :

#include "base/public/common_head.h"
#include "../base/base.h"

/*
 * 1.计算三角形面积
 *   已知道坐标用向量叉积公式。（向量叉积的结果向量的长度 *0.5便是三角形的面积，使用坐标法可计算出叉积结果向量的长度）
 *   已知道三个边的长度(a, b, c)，用海伦公式.
 *   p = (a + b + c) / 2;
 *   s = sqrt(p * (p - a) * (p - b) * (p - c))
 *
   http://hi.baidu.com/luosiyong/item/86437e4d9688eaeda5c066f3
   http://wenku.baidu.com/link?url=lMzxjoeNCkrMITPr-nxl9nHsIUCti8e6AB0UD6-eXdd5lZ1ju5gudWGTwlp301XA23bwMkT76enEwYUMe7nEkVlo9sR9I5c-MnqR_aiVHUq
 * 2.费马点(该点到三角形三个顶点的距离之和最小)
     有个有趣的结论：若三角形的三个内角均小于120度，那么该点连接
     三个顶点形成的三个角均为120度；若三角形存在一个内角大于120度，
     则该顶点就是费马点)计算公式如下:
     若有一个内角大于120度（这里假设为角C)，则距离为a + b
     若三个内角均小于120度，则距离为
     sqrt((a * a + b * b + c * c + 4 * sqrt(3.0) * s) / 2)
     // 此问题可以推广到n边形中

3. 内心----角平分线的交点
   令x = (a + b - c) / 2, y = (a - b + c) / 2,
     z = (-a + b + c) / 2, h = s / p. 
  计算公式为
  sqrt(x*x + h*h) + sqrt(y*y + h*h) + sqrt(z*z + h*h)

4. 重心----中线的交点, 计算公式如下：
   2.0 / 3 * (sqrt((2 * (a * a + b * b) - c * c) / 4) +
              sqrt((2 * (a * a + c * c) - b * b) / 4) +
              sqrt((2 * (b * b + c * c) - a * a) / 4))
5. 垂心----垂线的交点, 计算公式如下：
   3 * (c / 2 / sqrt(1 - cosC * cosC))

 * */

namespace algorithm {
}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
