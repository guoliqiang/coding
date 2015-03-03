// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 02:33:16
// File  : solver.cc
// Brief :

#include "../public/solver.h"
#include "../public/problem.h"

namespace nltk {
namespace svm {

bool SMO::SelectWorkingSet(int * out_i, int * out_j) {
  // return i,j such that
  // i: maximizes -y(i) * G_(i), i in I_up(alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y(j) *G_(j) < -y(i) * G_(i), j in I_low(alpha)

  double Gmax = -INF;
  double Gmax2 = -INF;
  int Gmax_idx = -1;
  int Gmin_idx = -1;
  double obj_diff_min = INF;

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

  for (int j = 0;j < node_count_; j++) {
    if (y(j) == 1) {
      if (!LowerBound(j)) {
        double grad_diff = Gmax + G_[j];
        if (G_[j] >= Gmax2) {
          Gmax2 = G_[j];
        }
        if (grad_diff > 0) {
          double obj_diff;
          double quad_coef = QD_[i] + QD_[j] -2.0 * y(i) * (Q_i.get()->data)[j];
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
      }  //  if (!LowerBound(j))
    } else {
      if (!UpperBound(j)) {
        double grad_diff= Gmax - G_[j];
        if (-G_[j] >= Gmax2) {
          Gmax2 = -G_[j];  // Note y == -1
        }
        if (grad_diff > 0) {
          double obj_diff;
          double quad_coef = QD_[i] + QD_[j] + 2.0 *y(i) *(Q_i.get()->data)[j];
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
      }  //  if (!UpperBound(j))
    }  //  else
  }
  // I_up 与 I_low 算出的 b 接近时推出
  // Gmax = b  Gmax = -b
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

  // G[i] + by[i] = 0
  // so r = -b
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
    LOG(INFO) << "iterator round:" << iter;
    int i = 0;
    int j = 0;
    if (!SelectWorkingSet(&i, &j)) {
      break;
    }
    ++iter;
    // update alpha[i] and alpha[j], handle bounds carefully
    base::shared_ptr<CacheNode> Q_i = GetQ(i, node_count_);
    base::shared_ptr<CacheNode> Q_j = GetQ(j, node_count_);

    double C_i = GetC(i);
    double C_j = GetC(j);

    double old_alpha_i = alpha_[i];
    double old_alpha_j = alpha_[j];

    if (y(i) != y(j)) {
      // k[i][i] + k[j][j] - 2 * k[i][j]
      double quad_coef = QD_[i] + QD_[j] + 2 * (Q_i.get()->data)[j];
      if (quad_coef <= 0) {
        quad_coef = TAU;
      }
      double delta = (-G_[i] - G_[j]) / quad_coef;
      double diff = alpha_[i] - alpha_[j];
      alpha_[i] += delta;
      alpha_[j] += delta;

      if (diff > 0) {
        if (alpha_[j] < 0) {
          alpha_[j] = 0;
          alpha_[i] = diff;
        }
      } else {
        if (alpha_[i] < 0) {
          alpha_[i] = 0;
          alpha_[j] = -diff;
        }
      }
      if (diff > C_i - C_j) {
        if (alpha_[i] > C_i) {
          alpha_[i] = C_i;
          alpha_[j] = C_i - diff;
        }
      } else {
        if (alpha_[j] > C_j) {
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
  ptr->best_obj = v / 2;

  for (int i = 0; i < node_count_; i++) {
    alpha_[i] *= y(i);
  }
  // when predict
  // sign[sum(alpha_a[i] * y[i] * k[i][j]) + b]

  // put back the solution
  for (int i = 0; i < node_count_; i++) {
    ptr->alpha.insert(i, alpha_[i]);
  }
}
}   // namepace svm
}  // namespace nltk
