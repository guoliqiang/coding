// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-19 11:55:09
// File  : max_flow.h
// Brief :

#ifndef  __MAX_FLOW_H_
#define  __MAX_FLOW_H_

#include <algorithm>
#include <map>
#include <set>
#include "graph.h"
#include "stdlib.h"
#include "base/public/string_util.h"
#include "third_part/gtl/public/map-util.h"

using namespace gtl;  // NOLINT

namespace util {

class MaxFlow {
 public:
  struct PNode {
   PNode(){}
   PNode(Edge * v, int w, int from) {
     value = v;
     weight = w;
     now_from = from;
   }
   int now_from;
   Edge * value;
   int weight;
  };

  static bool Cmp(const PNode &a, const PNode &b) {
    return a.weight < b.weight;
  }
  
 public:
  bool PFS(std::map<int, Edge *> & path) {
    path.clear();
    std::set<int> visited;
    std::vector<PNode> pqueue;
    visited.insert(s_);
    GraphIterator it(g_, s_);
    Edge * ptr = NULL;
    while (it.Next(&ptr)) {
      if (visited.count(ptr->Other(s_))) continue;
      visited.insert(s_);
      if (ptr->CapRto(ptr->Other(s_)) > 0) {
        // LOG(INFO) << " from " << s_ << " to " << ptr->Other(s_)
        //           << " value:" << ptr->CapRto(ptr->Other(s_));
        PNode tmp(ptr, ptr->CapRto(ptr->Other(s_)), s_);
        pqueue.push_back(tmp);
      }
    }
    std::sort(pqueue.begin(), pqueue.end(), Cmp);
    
    while (pqueue.size() != 0) {
      PNode max = pqueue.back();
      pqueue.pop_back();
      int curr_from = max.value->Other(max.now_from);
      InsertOrUpdate(&path, curr_from,  max.value);
      // LOG(INFO) << "pop:" << max.now_from << " to " << curr_from;
      if (curr_from == d_) break;
      GraphIterator it(g_, curr_from);
      while(it.Next(&ptr)) {
        if (visited.count(ptr->Other(curr_from))) {
          // LOG(INFO) << ptr->Other(curr_from) << " has visited";
          continue;
        }
        visited.insert(curr_from);
        if (ptr->CapRto(ptr->Other(curr_from)) > 0) {
          // LOG(INFO) << " from " << curr_from << " to " << ptr->Other(curr_from)
          //           << " value:" << ptr->CapRto(ptr->Other(curr_from));
          PNode tmp(ptr, ptr->CapRto(ptr->Other(curr_from)), curr_from);
          pqueue.push_back(tmp);
        }
      }
      std::sort(pqueue.begin(), pqueue.end(), Cmp);
    }
    return path.count(d_);
  }

  int GetMaxFlow() {
    int rs = 0;
    std::map<int, Edge *> path;
    while (PFS(path)) {
      // LOG(INFO) << "path:" << JoinKeys(&path);
      // LOG(INFO) << "begin real path:";
      // LOG(INFO) << d_;
      int foo = d_;
      CHECK(path.count(foo));
      int avalable_flow = path[foo]->CapRto(foo);
      foo = path[foo]->Other(foo);
      while (foo != s_) {
        // LOG(INFO) << foo;
        CHECK(path.count(foo));
        if (avalable_flow > path[foo]->CapRto(foo)) {
          avalable_flow = path[foo]->CapRto(foo);
        }
        foo = path[foo]->Other(foo);
      }
      // LOG(INFO) << foo;
      // LOG(INFO) << "begin real end with avalableflow :" << avalable_flow;
      rs += avalable_flow;
      
      foo = d_;
      path[foo]->AddFlow(foo, avalable_flow);
      foo = path[foo]->Other(foo);
      while (foo != s_) {
        path[foo]->AddFlow(foo, avalable_flow);
        foo = path[foo]->Other(foo);
      }
    }
    return rs;
  }

  MaxFlow(Graph * g, int s, int d) {
    g_ = g;
    s_ = s;
    d_ = d;
  }

 public:
  Graph * g_;
  int s_;  // source
  int d_;  // destination
  
  private:
};
}  // namespace util

#endif  //  __MAX_FLOW_H_
