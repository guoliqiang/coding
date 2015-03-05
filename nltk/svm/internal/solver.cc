// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 02:33:16
// File  : solver.cc
// Brief :

#include "../public/solver.h"
#include "../public/problem.h"

namespace nltk {
namespace svm {

SMO::SMO(const std::vector<base::shared_ptr<ProblemNode> > & a,
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
  VLOG(5) << a.front()->lable << " class alpha range: 0~" << Ca_ << " "
          << b.front()->lable << " class alpha range 0~" << Cb_;

  node_count_ = a_.size() + b_.size();
  cache_.reset(new Cache(node_count_, max(node_count_, para_->mem_size_)));
  alpha_.assign(node_count_, 0);
  G_.assign(node_count_, -1);
  QD_.assign(node_count_, 0);

  for (int i = 0; i < node_count_; i++) {
    QD_[i] = Kernel::GetInstance().Do(GetNode(i), GetNode(i));
  }
}

int32_t SMO::y(int32_t i) {
  if (i < a_.size()) return 1;
  return -1;
}

ProblemNode & SMO::GetNode(int32_t i) {
  CHECK_LT(i, node_count_);
  if (i < a_.size()) {
    return *(a_[i].get());
  } else {
    return *(b_[i - a_.size()].get());
  }
}

double SMO::GetC(int32_t i) {
  CHECK_LT(i, node_count_);
  if (i < a_.size()) return Ca_;
  return Cb_;
}

bool SMO::UpperBound(int32_t i) {
  if (alpha_[i] >= GetC(i)) return true;
  return false;
}

bool SMO::LowerBound(int32_t i) {
  if (alpha_[i] <= 0) return true;
  return false;
}

bool SMO::Free(int32_t i) {
  if (!UpperBound(i) && !LowerBound(i)) return true;
  return false;
}

base::shared_ptr<CacheNode> SMO::GetQ(int32_t i, int32_t len) {
  base::shared_ptr<CacheNode> rs;
  int start = cache_->GetData(i, len, &rs);
  for (int j = start; j < len; j++) {
    rs.get()->data[j] =
      y(i) * y(j) * Kernel::GetInstance().Do(GetNode(i), GetNode(j));
    VLOG(5) << "y[" << i << "]=" << y(i) << " y[" << j << "]=" << y(j)
            << " kernel(x[" << i << "],x[" << j << "])="
            << Kernel::GetInstance().Do(GetNode(i), GetNode(j))
            << "\nx[" << i << "]:\n" << GetNode(i).ToString()
            << "\nx[" << j << "]:\n" << GetNode(j).ToString();
  }
  return rs;
}

bool SMO::SelectWorkingSet(int * out_i, int * out_j) {
  // return i,j such that
  // i: maximizes -y(i) * G_(i), i in I_up(alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y(j) *G_(j) < -y(i) * G_(i), j in I_low(alpha)
  //
  // -y[t] * G[t] <= b t in _I_up
  // I_up = {t | alpha[t] < C; y[t] = 1 or alpah[t] > 0 y[t] = -1}
  // -y[t] * G[t] >= b t in _I_low
  // I_low = {t|alpah[t] < C; y[t] = -1 or alpah[t] > 0 y[t] = 1}

  double Gmax = -INF;
  double Gmax2 = -INF;
  int Gmax_idx = -1;
  int Gmin_idx = -1;
  double obj_diff_min = INF;

  // search maximizes -y(i) * G_(i), i in I_up(alpha)
  for (int t = 0; t< node_count_; t++) {
    if (y(t) == 1) {
      if (!UpperBound(t)) {
        if (-G_[t] >= Gmax) {
          Gmax = -G_[t];
          Gmax_idx = t;
        }
      }
    } else {
      if (!LowerBound(t)) {
        if (G_[t] >= Gmax) {
          Gmax = G_[t];
          Gmax_idx = t;
        }
      }
    }
  }

  int i = Gmax_idx;
  base::shared_ptr<CacheNode> Q_i;
  if (i != -1) {
    Q_i = GetQ(i, node_count_);
  }
  VLOG(5) << "first select i=" << Gmax_idx << " value=" << Gmax;

  for (int j = 0;j < node_count_; j++) {
    if (y(j) == 1) {
      if (!LowerBound(j)) {
        double grad_diff = Gmax + G_[j];
        if (G_[j] >= Gmax2) {
          Gmax2 = G_[j];
        }
        double quad_coef = 0;
        if (grad_diff > 0) {
          double obj_diff;
          // kernal(x[i], x[i]) + kernel(x[j], x[j]) -
          // 2 * y[i] * y[i] * y[j] * kernal(x[i], x[j])
          // ==>
          // kernel(x[i], x[i]) + kernel(x[j], x[j]) - 2 * kernal(x[i], x[j])
          // this is the k value from book
          quad_coef = QD_[i] + QD_[j] - 2.0 * y(i) * (Q_i.get()->data)[j];
          // obj_diff = -(E[i] - E[j])^2 / quad_coef
          // <=> -|E[i] - E[j] | / quad_coef
          // min obj_diff can max change the value  of alpah[i] when updata
          // alpah[i] += y[i] * (E[j] - E[i]) / quad_coef
          if (quad_coef > 0) {
            obj_diff = -(grad_diff * grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff * grad_diff) / TAU;
          }
          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }  // if (grad_diff > 0)
        VLOG(5) << "j=" << j << " Gmax2=" << Gmax2 << " grad_diff=" << grad_diff
                << " obj_diff_min=" << obj_diff_min << " kernel(" << i << ") = "
                << QD_[i] << " kernel(" << j << ")=" << QD_[j] << " quad_coef="
                << quad_coef << " y[" << i << "]=" << y(i) << " "
                << (Q_i.get()->data)[j];
      }  //  if (!LowerBound(j))
    } else {
      if (!UpperBound(j)) {
        double grad_diff= Gmax - G_[j];
        if (-G_[j] >= Gmax2) {
          Gmax2 = -G_[j];  // Note y == -1
        }
        double quad_coef = 0;
        if (grad_diff > 0) {
          double obj_diff;
          quad_coef = QD_[i] + QD_[j] + 2.0 * y(i) *(Q_i.get()->data)[j];
          if (quad_coef > 0) {
            obj_diff = -(grad_diff * grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff * grad_diff) / TAU;
          }
          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }  //  if (grad_diff > 0)
        VLOG(5) << "j=" << j << " Gmax2=" << Gmax2 << " grad_diff=" << grad_diff
                << " obj_diff_min=" << obj_diff_min << " kernel(" << i << ") = "
                << QD_[i] << " kernel(" << j << ")=" << QD_[j] << " quad_coef="
                << quad_coef << " y[" << i << "]=" << y(i) << " "
                << (Q_i.get()->data)[j];
      }  //  if (!UpperBound(j))
    }  //  else
  }
  // I_up 与 I_low 算出的 b 接近时推出
  // Gmax = b  Gmax2 = -b
  if (Gmax + Gmax2 < para_->eps_) {
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
  for (int i = 0;i < node_count_; i++) {
    double yG = y(i) * G_[i];
    if (UpperBound(i)) {
      if (y(i) == -1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else if (LowerBound(i)) {
      if (y(i) == 1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else {
      nr_free++;
      sum_free += yG;
    }
  }

  if (nr_free > 0) {
    // use average value
    r = sum_free / nr_free;
  } else {
    r = (ub + lb) / 2;
  }
  // for all perfect support SVM points x[i]
  // f(x[i]) = y[i]  s.t. y[i] = 1 or -1
  // <=>
  // sum(alpha[m] * y[m] * kernal(x[m], x[i])) + b = y[i]
  // <=>
  // sum(alpha[m] * y[m] * y[i] * kernal(x[m], x[i])) + y[i] * b = 1
  // ==>
  // sum(alpha[m] * y[m] * y[i] * kernal(x[m], x[i])) - 1 + y[i] * b = 0
  // ==>
  // G[i] + b * y[i] = 0
  // ==>
  // y[i] * G[i] = -b
  // so r = -b
  //
  // for all none support SVM points G[i] + b * y[i] > 0
  // for all wrong SVM points G[i] + b * y[i] < 0
  return r;
}

void SMO::Do(ModelNode * ptr) {
  for (int i = 0; i < node_count_; i++) {
    if (!LowerBound(i)) {
      base::shared_ptr<CacheNode> Q_i = GetQ(i, node_count_);
      double alpha_i = alpha_[i];
      for (int j = 0; j < node_count_; j++) {
        G_[j] += alpha_i * (Q_i.get()->data)[j];
      }
    }
  }

  int iter = 0;
  int max_iter = max(10000000,
                     node_count_ > INT_MAX / 100 ? INT_MAX : 100 * node_count_);

  while (iter < max_iter) {
    int i = 0;
    int j = 0;
    if (!SelectWorkingSet(&i, &j)) {
      break;
    }
    VLOG(3) << "round = " << iter << " optimize\nx[" << i << "]:\n"
            << GetNode(i).ToString() << "\nx[" << j << "]:\n"
            << GetNode(j).ToString();
    ++iter;
    // update alpha[i] and alpha[j], handle bounds carefully
    base::shared_ptr<CacheNode> Q_i = GetQ(i, node_count_);
    base::shared_ptr<CacheNode> Q_j = GetQ(j, node_count_);

    double C_i = GetC(i);
    double C_j = GetC(j);

    double old_alpha_i = alpha_[i];
    double old_alpha_j = alpha_[j];

    if (y(i) != y(j)) {
      // kernal(x[i], x[i]) + kernel(x[j], x[j]) +
      // 2 * y[i] * y[j] * kernel(x[i], x[j])
      // because y[i] != y[j], so
      // <=> kernel(x[i], x[i]) + kernel(x[j], x[j]) - 2 * kernal(x[i], x[j])
      // this is the equation of k from book
      double quad_coef = QD_[i] + QD_[j] + 2 * (Q_i.get()->data)[j];
      if (quad_coef <= 0) {
        quad_coef = TAU;
      }
      // because from book we can known
      // delta = y[j] * (E[i] - E[j]) / quad_coef
      // <=>
      // delta = y[j] *(y[i] * G[i] - y[j] * G[j]) / quad_coef
      //       = (-G[i] - G[j]) / quad_coef
      double delta = (-G_[i] - G_[j]) / quad_coef;
      double diff = alpha_[i] - alpha_[j];
      // from bool we known
      // alpha[i] = alpha[i] + y[i] * (E[j] - E[i]) / quad_coef
      //          = alpha[i] + y[i] * (y[j] * G[j] - y[i] * G[i]) / quad_coef
      //          = alpha[i] + (-G[j] - G[i]) / quad_coef
      //          = alpha[i] + delta
      // alpha[j] = alpha[j] + y[j] * (E[i] - E[j]) / quad_coef
      //          = alpha[j] + (-G[i] - G[j]) / quad_coef
      //          = alpha[j] + delta
      alpha_[i] += delta;
      alpha_[j] += delta;

      // max(0, diff) <= alpha[j] <= min(C_j, C_i + diff)
      // max(0, diff) <= alpha[i] <= min(C_i, C_j + diff)
      if (diff > 0) {
        // if alpha[i] < 0, alpha[j] must < 0
        if (alpha_[j] < 0) {
          alpha_[j] = 0;
          alpha_[i] = diff;
        }
      } else {
        // if alpha[j] < 0, alpha[i] must < 0
        if (alpha_[i] < 0) {
          alpha_[i] = 0;
          alpha_[j] = -diff;
        }
      }

      if (diff > C_i - C_j) {
        // if alpah[j] > C_j alpha[i] must > C_i
        if (alpha_[i] > C_i) {
          alpha_[i] = C_i;
          alpha_[j] = C_i - diff;
        }
      } else {
        // if alpha[i] > C_i alpha[j] must > C_j
        if (alpha_[j] > C_j) {
          alpha_[j] = C_j;
          alpha_[i] = C_j + diff;
        }
      }
    } else {
      // the same as before
      // quad_coef =
      // kernel(x[i], x[i]) + kernel(x[j], x[j]) - 2 * kernal(x[i], x[j])
      double quad_coef = QD_[i] + QD_[j] - 2 * (Q_i.get()->data)[j];
      if (quad_coef <= 0) {
        quad_coef = TAU;
      }
      // the same as before
      // delta = y[j] * (E[i] - E[j]) / quad_coef
      double delta = (G_[i] - G_[j]) / quad_coef;
      double sum = alpha_[i] + alpha_[j];
      // from book we known
      // alpha[i] = alpha[i] + y[i] * (E[j] - E[i]) / quad_coef
      //          = alpha[i] + y[i] * (y[j] * G[j] - y[i] * G[i]) / quad_coef
      //          = alpha[i] + (G[j] - G[i]) / quad_coef
      //          = alpha[i] - delta
      // alpha[j] = alpha[j] + y[j] * (E[i] - E[j]) / quad_coef
      //          = alpha[j] + (G[i] - G[j]) / quad_coef
      //          = alpha[j] + delta
      alpha_[i] -= delta;
      alpha_[j] += delta;

      // max(0, sum -C_i) <= alpha[j] <= min(C_j, sum)
      // max(0, sum -C_j) <= alpha[i] <= min(C_i, sum)
      if (sum > C_i) {
        if (alpha_[i] > C_i) {
          alpha_[i] = C_i;
          alpha_[j] = sum - C_i;
        }
      } else {
        if (alpha_[j] < 0) {
          alpha_[j] = 0;
          alpha_[i] = sum;
        }
      }

      if (sum > C_j) {
        if (alpha_[j] > C_j) {
          alpha_[j] = C_j;
          alpha_[i] = sum - C_j;
        }
      } else {
        if (alpha_[i] < 0) {
          alpha_[i] = 0;
          alpha_[j] = sum;
        }
      }
    }

    // update G
    double delta_alpha_i = alpha_[i] - old_alpha_i;
    double delta_alpha_j = alpha_[j] - old_alpha_j;

    for (int k = 0; k< node_count_; k++) {
      G_[k] += (Q_i.get()->data)[k] * delta_alpha_i +
               (Q_j.get()->data)[k] * delta_alpha_j;
    }
  }  // while
  LOG(INFO) << "optimize finished iter=" << iter;
  if (iter >= max_iter) {
    LOG(WARNING) << "reaching max number of iterations";
  }
  // calculate b
  // note this value is -b
  ptr->b = CalculateB();

  // calculate objective value
  double v = 0;
  for (int i = 0; i < node_count_; i++) {
    v += alpha_[i] * (G_[i] + (-1));
  }
  // is min value of object function.
  // best_obj =
  //   0.5 * sum(alpha[i] * alpha[j] * y[i] * y[j] * kernel(x[i], x[j]))
  //   - sum(alpha[i])
  // this is the object function of CSVM
  ptr->best_obj = v / 2;

  for (int i = 0; i < node_count_; i++) {
    alpha_[i] *= y(i);
  }
  // when predict x(m)
  // sign[sum(alpha_a[i] * y[i] * kernel(x[i], x[m])) + b]

  // put back the solution
  for (int i = 0; i < node_count_; i++) {
    ptr->alpha.insert(i, alpha_[i]);
  }
}
}   // namepace svm
}  // namespace nltk
