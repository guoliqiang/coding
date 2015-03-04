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
      base::shared_ptr<Parameter> para);
       
  int32_t y(int32_t i);
  ProblemNode & GetNode(int32_t i);
  double GetC(int32_t i);
  bool UpperBound(int32_t i);
  bool LowerBound(int32_t i);
  bool Free(int32_t i);
  base::shared_ptr<CacheNode> GetQ(int32_t i, int32_t len);
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
  // G_[i] = sum(alpha_[m] * y[m] * y[i] * kernel(x[i], x[m])) -1
  // so G_[i] is min(L) 的梯度 of alpha[i], L is the objective function
  // Note: only less 1/2, you will find it will be / 2
  //
  // min(L) = min(alpha >=0)(1/2 * sum(alpha[i] * alpha[j] * y[i] * y[j] *
  //                                   kernal(x[i] * x[j]))
  //                         - sum(alpha[i]))
  // E[i] = y[i]G_[i] , E[i] was variable in book.
  std::vector<double> G_;
  // QD[i] = kernal(x[i], x[i])
  std::vector<double> QD_;
  int32_t node_count_;
 
 private:
  std::vector<double> alpha_;
};

}  // namespace svm
}  // namespace nltk

#endif  //  __SOLVER_H_
