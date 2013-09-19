// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-19 11:10:19
// File  : graph.h
// Brief :

#ifndef  __GRAPH_H_
#define  __GRAPH_H_

#include "base/public/logging.h"
#include <vector>

namespace util {

class Edge {
 public:
  Edge(int from, int to, int capcity,
       int cost = 0, int flow = 0) {
    from_ = from;
    to_ = to;
    capcity_ = capcity;
    cost_ = cost;
    flow_ = flow;
  }

  bool IsFrom(int v) {
    return from_ == v;
  }

  int From() {
    return from_;
  }

  // cost flow to v
  int CostRto(int v) {
    return IsFrom(v) ? -cost_ : cost_;
  }
  
  int Other(int v) {
    CHECK(v == from_ || v == to_);
    return IsFrom(v) ? to_ : from_;
  }

  int To() {
    return to_;
  }

  // the capcity flow to v
  int CapRto(int v) {
    CHECK(v == from_ || v == to_);
    return IsFrom(v) ? flow_ : capcity_ - flow_;
  }

  // add d value flow to v
  void AddFlow(int v, int d) {
    flow_ += (IsFrom(v) ? -d : d);
  }

 public:
  int from_;
  int to_;
  int capcity_;
  int cost_;
  int flow_;

 private:
};

class Graph {
 public:
  Graph(int v, bool di_graph = true) {
    v_cnt_ = v;
    e_cnt_ = 0;
    di_graph_ = di_graph;
    for (int i = 0; i < v; i++) {
      std::vector<Edge *> foo(v, NULL);
      adj_.push_back(foo);
    }
  }

  ~Graph() {
    for (int i = 0; i < adj_.size(); i++) {
      for (int j = 0; j < adj_.size(); j++) {
        delete adj_[i][j];
      }
    }
  }

  int VCnt() {
    return v_cnt_;
  }

  int ECnt() {
    return e_cnt_;
  }

  bool Directed() {
    return di_graph_;
  }
  void Insert(int from, int to, int capcity, int cost = 0) {
    Edge e(from, to, capcity, cost);
    Insert(e);
  }
  void Insert(Edge e) {
    Edge * ptr = new Edge(e);
    int from = ptr->From();
    int to = ptr->To();
    if (adj_[from][to] != NULL) {
      delete adj_[from][to];
     e_cnt_--;
    }
    adj_[from][to] = ptr;
    e_cnt_++;
    if (!di_graph_) {
      ptr = new Edge(e);
      from = ptr->From();
      to = ptr->To();
      if (adj_[to][from] != NULL) {
        delete adj_[to][from];
        e_cnt_--;
      }
      adj_[to][from] = ptr;
      e_cnt_++;
    }
  }

  Edge * Get(int i, int j) {
    return adj_[i][j];
  }

 public:
  std::vector<std::vector<Edge *> > adj_;
  int v_cnt_;
  int e_cnt_;
  bool di_graph_;  // 有向图
 private:
};

class GraphIterator {
  public:
   GraphIterator(Graph * g, int v) {
     g_ = g;
     v_ = v;
     i_ = -1;
     j_ = -1;
   }

   bool Next(Edge ** ptr) {
     for (i_++; i_ < g_->VCnt(); i_++) {
       if (g_->Get(v_, i_) != NULL) {
         *ptr = g_->Get(v_, i_);
         return true;
       }
     }
     for (j_++; j_ < g_->VCnt(); j_++) {
       if (g_->Get(j_, v_) != NULL) {
         *ptr = g_->Get(j_, v_);
         return true;
       }
     }
     return false;
   }

  public:
   Graph * g_;
   int i_;
   int j_;
   int v_;
};

}  // namespace util

#endif  //  __GRAPH_H_
