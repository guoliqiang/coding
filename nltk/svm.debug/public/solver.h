// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 01:08:14
// File  : solver.h
// Brief :

#ifndef  __SOLVER_H_
#define  __SOLVER_H_

#include "base/public/util.h"
#include "model.h"
#include "kernel.h"
#include "cache.h"

#define INF HUGE_VAL
#define TAU 1e-12

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
    VLOG(3) << "+1 class alpha range: 0~" << Ca_
            << " -1 class alpha range 0~" << Cb_;
    
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
    if (i < a_.size()) return +1;
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
      rs.get()->data[j] = y(i) * y(j)* Kernel::GetInstance().Do(GetNode(i), GetNode(j));
      LOG(INFO) << y(i) << " " <<  y(j) << " " << Kernel::GetInstance().Do(GetNode(i), GetNode(j));
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
  // G_[i] = sum(alpha_[m] * y[m] * y[i] kernel(node[m], node[m])) -1
  // min(f) 的梯度
  // E[i] = y[i]G_[i] , E[i] was variable in book.
  std::vector<double> G_;
  std::vector<double> QD_;
  int32_t node_count_;
 
 private:
  std::vector<double> alpha_;
};

bool SMO::SelectWorkingSet(int * out_i, int * out_j) {
  // return i,j such that
  // i: maximizes -y_i * grad(f)_i, i in I_up(alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(alpha)
  
  double Gmax = -INF;
  double Gmax2 = -INF;
  int Gmax_idx = -1;
  int Gmin_idx = -1;
  double obj_diff_min = INF;

  for(int t = 0; t< node_count_; t++) {
    if(y(t) == +1) {
      if(!UpperBound(t))
        if(-G_[t] >= Gmax) {
          Gmax = -G_[t];
          Gmax_idx = t;
        }
    } else {
      if(!LowerBound(t))
        if(G_[t] >= Gmax) {
          Gmax = G_[t];
          Gmax_idx = t;
        }
    }
  }
  LOG(INFO) << "Gmax_idx:" << Gmax_idx << " Gmax:" << Gmax;
  int i = Gmax_idx;
  base::shared_ptr<CacheNode> Q_i;
  if(i != -1) {
    Q_i = GetQ(i, node_count_);
  }

  for(int j = 0;j < node_count_; j++) {
    if(y(j) == +1) {
      if (!LowerBound(j)) {
        double grad_diff = Gmax + G_[j];
        if (G_[j] >= Gmax2) {
          Gmax2 = G_[j];
        }
        if (grad_diff > 0) {
          double obj_diff; 
          double quad_coef = QD_[i] + QD_[j] -2.0 * y(i) * (Q_i.get()->data)[j];
          if (quad_coef > 0) {
            obj_diff = -(grad_diff*grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff*grad_diff) / TAU;
          }
          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }  // if (grad_diff > 0)
      LOG(INFO) << j << ":" << "Gmin_idx:" << Gmin_idx << "obj_diff_min:" << obj_diff_min;
      }  //  if (!LowerBound(j))
    } else {
      if (!UpperBound(j)) {
        double grad_diff= Gmax - G_[j];
        LOG(INFO) << "grad_diff:" << grad_diff;
        if (-G_[j] >= Gmax2) {
          Gmax2 = -G_[j];
        }
        if (grad_diff > 0) {
          double obj_diff; 
          double quad_coef = QD_[i] + QD_[j] + 2.0 *y(i) *(Q_i.get()->data)[j];
          LOG(INFO) << QD_[i] << ":" << QD_[j] << ":" << y(i) << ":"
                    << (Q_i.get()->data)[j];
          LOG(INFO) << "quad_coef:" << quad_coef;
          if (quad_coef > 0) {
            obj_diff = -(grad_diff * grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff * grad_diff) / TAU;
          }
          LOG(INFO) << "obj_diff:" << obj_diff;

          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }  //  if (grad_diff
        LOG(INFO) << j << ":" << "Gmin_idx:" << Gmin_idx << "obj_diff_min:" << obj_diff_min;
      }  //  if (!UpperBound(j))
    }  //  else
  } 

  if(Gmax + Gmax2 < para_->eps_) {
    return false;
  }

  *out_i = Gmax_idx;
  *out_j = Gmin_idx;
  return true;
}

double SMO::CalculateB() {
  double r;
  int nr_free = 0;
  double ub = INF;
  double lb = -INF;
  double sum_free = 0;
  for(int i = 0;i < node_count_; i++) {
    double yG = y(i) * G_[i];
    if(UpperBound(i)) {
      if(y(i) == -1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else if(LowerBound(i)) {
      if(y(i) == +1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else {
      nr_free++;
      sum_free += yG;
    }
  }

  if(nr_free > 0) {
    // average value
    r = sum_free / nr_free;
  } else {
    r = (ub + lb) / 2;
  }

  // G[i] + by[i] = 0
  // so r = -b
  return r;
}

void SMO::Do(ModelNode * ptr) {
  for(int i = 0; i < node_count_; i++) {
    if(!LowerBound(i)) {
      base::shared_ptr<CacheNode> Q_i = GetQ(i, node_count_);
      double alpha_i = alpha_[i];
      for(int j = 0; j < node_count_; j++) {
        G_[j] += alpha_i * (Q_i.get()->data)[j];
      }
    }
  }
  LOG(INFO) << JoinVector(G_);
  LOG(INFO) << JoinVector(alpha_);
  for (int i = 0; i < node_count_; i++) {
    LOG(INFO) << y(i);
  }
  int iter = 0;
  int max_iter = max(10000000,
                     node_count_ > INT_MAX / 100 ? INT_MAX : 100 * node_count_);
  
  while(iter < max_iter) {
    LOG(INFO) << "iterator round:" << iter;
    int i = 0;
    int j = 0;
    if(!SelectWorkingSet(&i, &j)) {
      break;
    }
    LOG(INFO) << i << " " << j;
    ++iter;
    // update alpha[i] and alpha[j], handle bounds carefully
    base::shared_ptr<CacheNode> Q_i = GetQ(i, node_count_);
    base::shared_ptr<CacheNode> Q_j = GetQ(j, node_count_);

    double C_i = GetC(i);
    double C_j = GetC(j);

    double old_alpha_i = alpha_[i];
    double old_alpha_j = alpha_[j];

    if(y(i) != y(j)) {
      // k[i][i] + k[j][j] - 2 * k[i][j]
      double quad_coef = QD_[i] + QD_[j] + 2 * (Q_i.get()->data)[j];
      if (quad_coef <= 0) {
        quad_coef = TAU;
      }
      double delta = (-G_[i] - G_[j]) / quad_coef;
      double diff = alpha_[i] - alpha_[j];
      alpha_[i] += delta;
      alpha_[j] += delta;
      
      if(diff > 0) {
        if(alpha_[j] < 0) {
          alpha_[j] = 0;
          alpha_[i] = diff;
        }
      } else {
        if(alpha_[i] < 0) {
          alpha_[i] = 0;
          alpha_[j] = -diff;
        }
      }
      if(diff > C_i - C_j) {
        if(alpha_[i] > C_i) {
          alpha_[i] = C_i;
          alpha_[j] = C_i - diff;
        }
      } else {
        if(alpha_[j] > C_j) {
          alpha_[j] = C_j;
          alpha_[i] = C_j + diff;
        }
      }
    } else {
      double quad_coef = QD_[i] + QD_[j] - 2 * (Q_i.get()->data)[j];
      if (quad_coef <= 0) {
        quad_coef = TAU;
      }
      double delta = (G_[i] - G_[j]) / quad_coef;
      double sum = alpha_[i] + alpha_[j];
      alpha_[i] -= delta;
      alpha_[j] += delta;

      if(sum > C_i) {
        if(alpha_[i] > C_i) {
          alpha_[i] = C_i;
          alpha_[j] = sum - C_i;
        }
      } else {
        if(alpha_[j] < 0) {
          alpha_[j] = 0;
          alpha_[i] = sum;
        }
      }
      if(sum > C_j) {
        if(alpha_[j] > C_j) {
          alpha_[j] = C_j;
          alpha_[i] = sum - C_j;
        }
      } else {
        if(alpha_[i] < 0) {
          alpha_[i] = 0;
          alpha_[j] = sum;
        }
      }
    }

    // update G
    double delta_alpha_i = alpha_[i] - old_alpha_i;
    double delta_alpha_j = alpha_[j] - old_alpha_j;
    
    for(int k=0; k< node_count_; k++) {
      G_[k] += (Q_i.get()->data)[k] * delta_alpha_i +
               (Q_j.get()->data)[k] * delta_alpha_j;
    }
  }  // while

  if(iter >= max_iter) {
    LOG(WARNING) << "reaching max number of iterations";
  }
  // calculate b
  // note this value is -b
  ptr->b = CalculateB();

  // calculate objective value
  double v = 0;
  for(int i = 0; i < node_count_; i++) {
    v += alpha_[i] * (G_[i] + (-1)); 
  }
  // is min value of object function.
  ptr->best_obj = v / 2;

  for (int i = 0; i < node_count_; i++) {
    alpha_[i] *= y(i);
  }
  // when predict
  // sign[sum(alpha_a[i] * y[i] * k[i][j]) + b]

  // put back the solution
  for(int i = 0; i < node_count_; i++) {
    ptr->alpha.insert(i, alpha_[i]);
  }
}

}  // namespace svm
}  // namespace nltk

#endif  //  __SOLVER_H_
