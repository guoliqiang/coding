// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 01:08:14
// File  : solver.h
// Brief :

#ifndef  __SOLVER_H_
#define  __SOLVER_H_

#include "base/public/util.h"
#include "model.h"
#include "kernel.h"
#include "cache.h"

namespace nltk {
namespace svm {

using base::max;
using base::min;

class Solver {
 public:
  Solver() {}
  virtual ~Solver() {}
  virtual void Do(ModelNode * ptr) = 0;
};

class SMO : public Solver {
 public:
  SMO(const std::vector<base::shared_ptr<ProblemNode> > & a,
      const std::vector<base::shared_ptr<ProblemNode> > & b,
      base::shared_ptr<Parameter> para) {
    CHECK_GT(a.size(), 0);
    CHECK_GT(b.size(), 0);
    a_ = a;
    b_ = b;
    para_ = para;
    Ca_ = para_->c_;
    Cb_ = para_->c_;
    if (para_->weights_.count(a.front()->lable)) {
      Ca_ *= para_->weights_[a.front()->lable];
    }
    if (para_->weights_.count(b.front()->lable)) {
      Cb_ *= para_->weights_[b.front()->lable];
    }
    VLOG(3) << a.front()->lable << " class alpha range: 0 ~ " << Ca_
            << b.front()->lable << " class alpha range 0 ~ " << Cb_;
    
    node_count_ = a_.size() + b_.size();
    cache_.reset(new Cache(node_count_, max(node_count_, para_->mem_size_)));
    alpha_.assign(node_count_, 0);
    G_.assign(node_count_, -1);
    QD_.assign(node_count_, 0);
    
    for (int i = 0; i < node_count_; i++) {
      QD_[i] = Kernel::GetInstance().Do(GetNode(i), GetNode(i));
      VLOG(3) << i << " selef kernel value:" << QD_[i];
      GetNode(i).LogContent(3);
      GetNode(i).LogContent(3);
    }
  }
  
  int32_t y(int32_t i) {
    if (i < a_.size()) return 1;
    return -1;
  }

  ProblemNode & GetNode(int32_t i) {
    CHECK_LT(i, node_count_);
    if (i < a_.size()) return *(a_[i].get());
    else return *(b_[i - a_.size()].get());
  }

  double GetC(int32_t i) {
    CHECK_LT(i, node_count_);
    if (i < a_.size()) return Ca_;
    return Cb_;
  }

  bool UpperBound(int32_t i) {
    if (alpha_[i] >= GetC(i)) return true;
    return false;
  }

  bool LowerBound(int32_t i) {
    if (alpha_[i] <= 0) return true;
    return false;
  }

  bool Free(int32_t i) {
    if(!UpperBound(i) && !LowerBound(i)) return true;
    return false;
  }

   base::shared_ptr<CacheNode> GetQ(int32_t i, int32_t len) {
    base::shared_ptr<CacheNode> rs;
    int start = cache_->GetData(i, len, &rs);
    for (int j = start; j < len; j++) {
      rs.get()->data[j] =
          y(i) * y(j)* Kernel::GetInstance().Do(GetNode(i), GetNode(j));
    }
    return rs;
  }
  
  bool SelectWorkingSet(int * i, int * j);
  double CalculateB();
  void Do(ModelNode * ptr);
 
 private:
  std::vector<base::shared_ptr<ProblemNode> > a_;
  std::vector<base::shared_ptr<ProblemNode> > b_;
  double Ca_;
  double Cb_;
  scoped_ptr<Cache> cache_;
  base::shared_ptr<Parameter> para_;
  // G_[i] = sum(alpha_[m] * y[m] * y[i] * kernel(node[m], node[m])) -1
  // min(f) 的梯度
  // E[i] = y[i]G_[i] , E[i] was variable in book.
  std::vector<double> G_;
  std::vector<double> QD_;
  int32_t node_count_;
 
 private:
  std::vector<double> alpha_;
};

}  // namespace svm
}  // namespace nltk

#endif  //  __SOLVER_H_
