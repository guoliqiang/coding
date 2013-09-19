// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-19 13:21:48
// File  : max_flow_unittest.cc
// Brief :
#include "../../public/max_flow.h"
#include "../../public/min_cost_flow.h"
using namespace util;

int main(int argc, char** argv) {
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
  return 0;
}
