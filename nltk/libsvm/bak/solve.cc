// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-05 10:25:52
// File  : solve.cc
// Brief :

#include "../public/solve.h"

namespace nltk {

double Solver::GetC(int i) {
  return (y_[i] > 0)? Cp_ : Cn_;
}

void Solver::UpdateAlphaStatus(int i) {
  if(alpha_[i] >= GetC(i)) {
    alpha_status_[i] = UPPER_BOUND;
  } else if(alpha_[i] <= 0) {
    alpha_status_[i] = LOWER_BOUND;
  } else {
    alpha_status_[i] = FREE;
  }
}

bool Solver::IsUpperBound(int i) {
  return alpha_status_[i] == UPPER_BOUND;
}
  
bool Solver::IsLowerBound(int i) {
  return alpha_status_[i] == LOWER_BOUND;
}

void Solver::SwapIndex(int i, int j) {
  Q_->SwapIndex(i, j);
  swap(y_[i], y_[j]);
  swap(G_[i], G_[j]);
  swap(alpha_status_[i], alpha_status_[j]);
  swap(alpha_[i], alpha_[j]);
  swap(p_[i], p_[j]);
  swap(active_set_[i], active_set_[j]);
  swap(G_bar_[i], G_bar_[j]);
}

void Solver::ReconstructGradient() {
  // reconstruct inactive elements of G from G_bar and free variables
  if(active_size_ == l_) return;

  int i;
  int j;
  int nr_free = 0;

  for(j = active_size_; j < l_; j++) {
    G_[j] = G_bar_[j] + p_[j];
  }

  for(j = 0; j < active_size_; j++) {
    if(IsFree(j)) {
      nr_free++;
    }
  }

  if(2 * nr_free < active_size_) {
    info("\nWARNING: using -h 0 may be faster\n");
  }

  if (nr_free * l_ > 2 * active_size_ * (l_ - active_size_)) {
    for(i = active_size_; i < l_; i++) {
      const Qfloat *Q_i = Q_->GetQ(i, active_size_);
      for(j=0; j < active_size_; j++) {
        if(IsFree(j)) {
          G_[i] += alpha_[j] * Q_i[j];
        }
      }
    }
  } else {
    for(i = 0; i < active_size_; i++) {
      if(IsFree(i)) {
        const Qfloat * Q_i = Q_->GetQ(i, l_);
        double alpha_i = alpha_[i];
        for(j = active_size_; j < l_; j++) {
          G_[j] += alpha_i * Q_i[j];
        }
      }  // if
    }  // for
  } // if
}

void Solver::Solve(int l, const QMatrix& Q,
                   const double * p, const schar * y,
                   double *alpha, double Cp,
                   double Cn, double eps,
                   SolutionInfo* si, int shrinking) {
  l_ = l;
  Q_ = &Q;
  QD_ = Q.GetQD();
  clone(p_, p ,l);
  clone(y_, y ,l);
  clone(alpha_, alpha, l);
  Cp_ = Cp;
  Cn_ = Cn;
  eps_ = eps;
  unshrink_ = false;

  // initialize alpha_status_
  {
    alpha_status_ = new char[l_];
    for(int i = 0; i < l_; i++) {
      UpdateAlphaStatus(i);
    }
  }

  // initialize active set (for shrinking)
  {
    active_set_ = new int[l_];
    for(int i = 0; i < l_; i++) {
      active_set_[i] = i;
    }
    active_size_ = l_;
  }

 // initialize gradient
  {
    G_ = new double[l_];
    G_bar_ = new double[l_];
    int i;
    for(i = 0; i < l_; i++) {
      G_[i] = p_[i];  // -1
      G_bar_[i] = 0;
    }
    for(i = 0; i < l_; i++) {
      if(!IsLowerBound(i)) {
        const Qfloat * Q_i = Q_->GetQ(i, l_);
        double alpha_i = alpha_[i];
        int j;
        for(j = 0; j < l_; j++) {
          G_[j] += alpha_i * Q_i[j];
        }
        if(IsUpperBound(i)) {
          for(j = 0;j < l_; j++) {
            G_bar_[j] += GetC(i) * Q_i[j];
          }
        }  // if
      }  // if
    }  // for
  }

  // optimization step
  int iter = 0;
  int max_iter = max(10000000, l_ > INT_MAX/100 ? INT_MAX : 100 * l_);
  int counter = min(l_, 1000) + 1;
  
  while(iter < max_iter) {
    // show progress and do shrinking
    if(--counter == 0) {
      counter = min(l_, 1000);
      if(shrinking) DoShrinking();
      info(".");
    }

    int i;
    int j;
    if(SelectWorkingSet(i, j) != 0) {
      // reconstruct the whole gradient
      ReconstructGradient();
      // reset active set size and check
      active_size_ = l_;
      info("*");
      if(SelectWorkingSet(i,j) != 0) {
        break;
      } else {
        counter = 1;  // do shrinking next iteration
      }
    }
    ++iter;
    // update alpha_[i] and alpha_[j], handle bounds carefully
    const Qfloat *Q_i = Q.GetQ(i, active_size_);
    const Qfloat *Q_j = Q.GetQ(j, active_size_);

    double C_i = GetC(i);
    double C_j = GetC(j);

    double old_alpha_i = alpha_[i];
    double old_alpha_j = alpha_[j];

    if(y_[i] != y_[j]) {
      double quad_coef = QD_[i] + QD_[j] + 2 * Q_i[j];
      if (quad_coef <= 0) quad_coef = TAU;
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

      if(diff > C_i - C_j){
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
      double quad_coef = QD_[i] + QD_[j] - 2 * Q_i[j];
      if (quad_coef <= 0) quad_coef = TAU;
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
    
    for(int k = 0; k < active_size_; k++) {
      G_[k] += Q_i[k] * delta_alpha_i + Q_j[k] * delta_alpha_j;
    }

    // update alpha_status_ and G_bar
    {
      bool ui = IsUpperBound(i);
      bool uj = IsUpperBound(j);
      UpdateAlphaStatus(i);
      UpdateAlphaStatus(j);
      int k;
      if(ui != IsUpperBound(i)) {
        Q_i = Q_->GetQ(i, l_);
        if(ui) {
          for(k = 0; k < l_; k++) G_bar_[k] -= C_i * Q_i[k];
        } else {
          for(k = 0; k < l_; k++) G_bar_[k] += C_i * Q_i[k];
        }
      }

      if(uj != IsUpperBound(j)) {
        Q_j = Q.GetQ(j, l_);
        if(uj) {
          for(k = 0; k < l_; k++) G_bar_[k] -= C_j * Q_j[k];
        } else {
          for(k = 0; k < l_; k++) G_bar_[k] += C_j * Q_j[k];
        }
      }
    }
  }  // while

  if(iter >= max_iter) {
    if(active_size_ < l_) {
      // reconstruct the whole gradient to calculate objective value
      ReconstructGradient();
      active_size_ = l_;
      info("*");
    }
    LOG(INFO) << "WARNING: reaching max number of iterations";
  }

  // calculate rho
  si->rho_ = CalculateRho();

  // calculate objective value
  {
    double v = 0;
    int i;
    for(i = 0; i < l_; i++) v += alpha_[i] * (G_[i] + p_[i]);
    si->obj_ = v/2;
  }

  // put back the solution
  {
    for(int i = 0;i < l_; i++) alpha_[active_set_[i]] = alpha_[i];
  }


  si->upper_bound_p_ = Cp_;
  si->upper_bound_n_ = Cn_;

  info("\noptimization finished, #iter = %d\n",iter);

  delete[] p_;
  delete[] y_;
  delete[] alpha_;
  delete[] alpha_status_;
  delete[] active_set_;
  delete[] G_;
  delete[] G_bar_;
}


// return 1 if already optimal, return 0 otherwise
int Solver::SelectWorkingSet(int & out_i, int & out_j) {
  // return i,j such that
  // i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)
  
  double Gmax = -INF;
  double Gmax2 = -INF;
  int Gmax_idx = -1;
  int Gmin_idx = -1;
  double obj_diff_min = INF;

  for(int t = 0; t < active_size_; t++) {
    if(y_[t] == +1) {
      if(!IsUpperBound(t)) {
        if(-G_[t] >= Gmax) {
          Gmax = -G_[t];
          Gmax_idx = t;
        }
      }
    } else {
      if(!IsLowerBound(t)) {
        if(G_[t] >= Gmax) {
          Gmax = G_[t];
          Gmax_idx = t;
        }
      }
    }  // if
  }  // for

  int i = Gmax_idx;
  const Qfloat *Q_i = NULL;
  if(i != -1) {  // NULL Q_i not accessed: Gmax=-INF if i=-1
    Q_i = Q_->GetQ(i, active_size_);
  }

  for(int j = 0; j < active_size_; j++) {
    if(y_[j] == +1) {
      if (!IsLowerBound(j)) {
        double grad_diff = Gmax+G_[j];
        if (G_[j] >= Gmax2) Gmax2 = G_[j];
        if (grad_diff > 0) {
          double obj_diff; 
          double quad_coef = QD_[i] + QD_[j]- 2.0 *y_[i] * Q_i[j];
          if (quad_coef > 0) {
            obj_diff = -(grad_diff * grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff * grad_diff) / TAU;
          }

          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }
      }  // if
    } else {
      if (!IsUpperBound(j)) {
        double grad_diff = Gmax-G_[j];
        if (-G_[j] >= Gmax2) Gmax2 = -G_[j];
        if (grad_diff > 0) {
         double obj_diff; 
          double quad_coef = QD_[i] + QD_[j] + 2.0* y_[i] * Q_i[j];
          if (quad_coef > 0) {
            obj_diff = -(grad_diff * grad_diff) / quad_coef;
          } else {
            obj_diff = -(grad_diff * grad_diff) / TAU;
          }

          if (obj_diff <= obj_diff_min) {
            Gmin_idx = j;
            obj_diff_min = obj_diff;
          }
        }
      }  // if
    }
  }  // for

  if(Gmax + Gmax2 < eps_) return 1;

  out_i = Gmax_idx;
  out_j = Gmin_idx;
  return 0;
}

bool Solver::BeShrunk(int i, double Gmax1, double Gmax2) {
  if(IsUpperBound(i)) {
    if(y_[i] == +1) {
      return(-G_[i] > Gmax1);
    } else {
      return(-G_[i] > Gmax2);
    }
  } else if(IsLowerBound(i)) {
    if(y_[i] == +1) {
      return(G_[i] > Gmax2);
    } else {
      return(G_[i] > Gmax1);
    }
  } else {
    return(false);
  }
}

void Solver::DoShrinking() {
  int i;
  double Gmax1 = -INF;    // max { -y_i * grad(f)_i | i in I_up(\alpha) }
  double Gmax2 = -INF;    // max { y_i * grad(f)_i | i in I_low(\alpha) }

  // find maximal violating pair first
  for(i = 0;i < active_size_; i++) {
    if(y_[i] == +1)   {
      if(!IsUpperBound(i)) {
        if(-G_[i] >= Gmax1) Gmax1 = -G_[i];
      }
      if(!IsLowerBound(i)) {
        if(G_[i] >= Gmax2) Gmax2 = G_[i];
      }
    } else {
      if(!IsUpperBound(i)) {
        if(-G_[i] >= Gmax2) Gmax2 = -G_[i];
      }
      if(!IsLowerBound(i)) {
        if(G_[i] >= Gmax1) Gmax1 = G_[i];
      }
    }
  }  // for

  if(unshrink_ == false && Gmax1 + Gmax2 <= eps_* 10) {
    unshrink_ = true;
    ReconstructGradient();
    active_size_ = l_;
    info("*");
  }

  for(i = 0; i < active_size_; i++) {
    if (BeShrunk(i, Gmax1, Gmax2)) {
      active_size_--;
      while (active_size_ > i) {
        if (!BeShrunk(active_size_, Gmax1, Gmax2)) {
          SwapIndex(i,active_size_);
          break;
        }
        active_size_--;
      }
    }  // if
  }
}

double Solver::CalculateRho() {
  double r;
  int nr_free = 0;
  double ub = INF, lb = -INF, sum_free = 0;
  for(int i = 0; i < active_size_; i++) {
    double yG = y_[i] * G_[i];
    if(IsUpperBound(i)) {
      if(y_[i] == -1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else if(IsLowerBound(i)) {
      if(y_[i] == +1) {
        ub = min(ub, yG);
      } else {
        lb = max(lb, yG);
      }
    } else {
      ++nr_free;
      sum_free += yG;
    }
  }  // for

  if(nr_free > 0) {
    r = sum_free / nr_free;
  } else {
    r = (ub + lb) / 2;
  }
  return r;
}


}  // namespace nltk
