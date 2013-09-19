// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-19 13:21:48
// File  : max_flow_unittest.cc
// Brief :
// 
// 于2013中秋，写代码到第二日凌晨，加油。 北京.南新园
// 观看《武林外传》40集，关于中秋，心甚悲。加油

#include "../../public/max_flow.h"
#include "../../public/min_cost_flow.h"
using namespace util;

int main(int argc, char** argv) {
 /*
  Graph g(6, true);
  g.Insert(0, 1, 16);
  g.Insert(0, 2, 13);

  g.Insert(1, 2, 10);
  g.Insert(2, 1, 4);
  g.Insert(1, 3, 12);
  g.Insert(3, 2, 9);
  g.Insert(2, 4, 14);
  g.Insert(4, 3, 7);

  g.Insert(3, 5, 20);
  g.Insert(4, 5, 4);
  
  MaxFlow max_flow(&g, 0, 5);
  LOG(INFO) << max_flow.GetMaxFlow();
  
  // use the example of:
  // http://wenku.baidu.com/view/bb623343b307e87101f6964f.html
  Graph g2(7, true);
  g2.Insert(0, 1, 5, 4);
  g2.Insert(0, 2, 6, 3);
  g2.Insert(0, 3, 5, 3);
  
  g2.Insert(1, 2, 2, 2);
  g2.Insert(1, 4, 3, 3);

  g2.Insert(2, 3, 3, 2);
  g2.Insert(2, 4, 3, 3);
  g2.Insert(2, 5, 7, 4);

  g2.Insert(3, 5, 5, 3);

  g2.Insert(4, 5, 1, 2);
  g2.Insert(4, 6, 8, 1);

  g2.Insert(5, 6, 7, 2);

  MinCostFlow min_cost_flow(&g2, 0, 6);
  LOG(INFO) << min_cost_flow.FindMinCostFlow();
  */
  // 二分图最大匹配转换成最大流
  Graph g3(8, true);

  g3.Insert(0, 1, 1);
  g3.Insert(0, 2, 1);
  g3.Insert(0, 3, 1);
  

  g3.Insert(1, 4, 1);
  g3.Insert(1, 5, 1);
  
  g3.Insert(2, 5, 1);
  g3.Insert(2, 6, 1);
  
  g3.Insert(3, 4, 1);
  
  g3.Insert(4, 7, 1);
  g3.Insert(5, 7, 1);
  g3.Insert(6, 7, 1);

  MaxFlow bm(&g3, 0, 7);
  LOG(INFO) << bm.GetMaxFlow();
  g3.Out();

  Graph g4(7, true);
  g4.Insert(0, 1, 1, 0);
  g4.Insert(0, 2, 1, 0);
  g4.Insert(0, 3, 1, 0);

  g4.Insert(1, 4, 1, 2);
  g4.Insert(2, 5, 1, 1);
  g4.Insert(3, 5, 1, 1);

  g4.Insert(4, 6, 1, 0);
  g4.Insert(5, 6, 1, 0);

  MinCostFlow bm2(&g4, 0, 6);
  LOG(INFO) << bm2.FindMinCostFlow();
  g4.Out();
  return 0;
}
