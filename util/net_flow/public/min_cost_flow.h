// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-19 16:23:07
// File  : min_cost_flow.h
// Brief :

#ifndef  __MIN_COST_FLOW_H_
#define  __MIN_COST_FLOW_H_

#include "graph.h"
#include "base/public/logging.h"
#include "third_part/gtl/public/map-util.h"
#include <queue>

using namespace gtl;  // NOLINT

namespace util {

class MinCostFlow {
 public:
 struct MNode {
   MNode(){}

   MNode(int from, Edge * v) {
     now_from = from;
     value = v;
   }

   int now_from;
   Edge * value;
 };

 public:
  MinCostFlow(Graph * g, int s, int d) {
    g_ = g;
    s_ = s;
    d_ = d;
  }

  int FindMinCostFlow() {
    int rs = 0;
    std::map<int, Edge *> path;
    while (FindPath(path)) {
      // LOG(INFO) << JoinKeys(&path);
      CHECK(path.count(d_));
      int min_flow = path[d_]->CapRto(d_);
      int foo = path[d_]->Other(d_);
      // LOG(INFO) << "path begin:";
      // LOG(INFO) << d_;
      while (foo != s_) {
        CHECK(path.count(foo));
        // LOG(INFO) << foo;
        if (path[foo]->CapRto(foo) < min_flow) {
          min_flow = path[foo]->CapRto(foo);
        }
        foo = path[foo]->Other(foo);
      }
      // LOG(INFO) << foo;
      // LOG(INFO) << "path end";
      // LOG(INFO) << "min_flow:" << min_flow;
      rs += min_flow;
      path[d_]->AddFlow(d_, min_flow);
      foo = path[d_]->Other(d_);
      while (foo != s_) {
        path[foo]->AddFlow(foo, min_flow);
        foo = path[foo]->Other(foo);
      }
    }
    return rs;
  }

  bool FindPath(std::map<int, Edge *> & path) {
    path.clear();
    MNode flag(-1, NULL);
    Edge * ptr = NULL;
    std::map<int, int> v_min;
    v_min[s_] = 0;
    
    std::queue<MNode> queue;
    GraphIterator it(g_, s_);
    while(it.Next(&ptr)) {
      int curr_to = ptr->Other(s_);
      if (ptr->CapRto(curr_to) > 0) {
        queue.push(MNode(s_, ptr));
      }
    }
    queue.push(flag);
    
    int round = 0;
    while (!queue.empty()) {
      MNode node = queue.front();
      queue.pop();
      while (node.now_from == -1) {
        if (round++ > g_->VCnt()) {
          // if (path.count(d_)) LOG(INFO) << "min cost" << v_min[d_];
          return path.count(d_);
        }
        queue.push(flag);
        node = queue.front();
        queue.pop();
      }

      int curr_from = node.value->Other(node.now_from);
      // LOG(INFO) << "now:" << node.now_from << "~" << curr_from;
      int value = node.value->CostRto(curr_from) +
                  (v_min.count(node.now_from) ? v_min[node.now_from] : 0);
      CHECK(value >= 0);
      // LOG(INFO) << "compute:" << node.now_from << "~" << curr_from << " value:" << value;
      if (!v_min.count(curr_from) || value < v_min[curr_from]) {
        InsertOrUpdate(&v_min, curr_from, value);
        // LOG(INFO) << "insert:" << curr_from << " : " << value;
        // LOG(INFO) << "key:" << JoinKeys(&v_min) << " values:" << JoinValues(&v_min);
        InsertOrUpdate(&path, curr_from, node.value);
      }

      GraphIterator it(g_, curr_from);
      while (it.Next(&ptr)) {
        int curr_to = ptr->Other(curr_from);
        if (ptr->CapRto(curr_to) > 0) {
          // 为了避免加入过多不必要的路径
          int value = ptr->CostRto(curr_to) +
                      (v_min.count(curr_from) ? v_min[curr_from] : 0);
          if ((v_min.count(curr_to) && value < v_min[curr_to]) || 
              !v_min.count(curr_to)) {
            queue.push(MNode(curr_from, ptr));
            // LOG(INFO) << "insert " "[" << curr_from << "]~"
            //           << "[" << curr_to << "]value:" << value;
          } else {
            // LOG(INFO) << "ignore" << curr_from << "~" << curr_to << " value:" << value;
          }
        }
      }
    }
    return path.count(d_);
  }

 public:
  Graph * g_;
  int s_;
  int d_;
};
}  // namespace util

#endif  //  __MIN_COST_FLOW_H_
