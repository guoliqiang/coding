// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 20:05:20
// File  : kernel.cc
// Brief :

#include "../public/kernel.h"

namespace nltk {

double Kernel::Linear(int i, int j) const {
  return Dot(x_[i], x_[j]);
}

double Kernel::Poly(int i, int j) const {
  return powi(gamma_ * Dot(x_[i], x_[j]) + coef0_, degree_);
}

double Kernel::Rbf(int i, int j) const {
    return exp(-gamma_ * (x_square_[i] + x_square_[j] - 2 * Dot(x_[i], x_[j])));
}

double Kernel::Sigmoid(int i, int j) const {
    return tanh(gamma_ * Dot(x_[i], x_[j]) + coef0_);
}

double Kernel::Precomputed(int i, int j) const {
  return x_[i][(int)(x_[j][0].value_)].value_;
}

Kernel::Kernel(int l, SvmNode ** x,
               const SvmParameter& param)
               :kernel_type_(param.kernel_type_),
                degree_(param.degree_),
                gamma_(param.gamma_),
                coef0_(param.coef0_) {
  switch(kernel_type_) {
    case LINEAR:
      kernel_function_ = &Kernel::Linear;
      break;
    case POLY:
      kernel_function_ = &Kernel::Poly;
      break;
    case RBF:
      kernel_function_ = &Kernel::Rbf;
      break;
    case SIGMOID:
      kernel_function_ = &Kernel::Sigmoid;
      break;
    case PRECOMPUTED:
      kernel_function_ = &Kernel::Precomputed;
      break;
  }

  clone(x_, x, l);

  if(kernel_type_ == RBF) {
    x_square_ = new double[l];
    for(int i = 0; i < l; i++) {
      x_square_[i] = Dot(x[i], x[i]);
    }
  } else {
    x_square_ = NULL;
  }
}


Kernel::~Kernel() {
  delete[] x_;
  delete[] x_square_;
}

double Kernel::Dot(const SvmNode * px,
                   const SvmNode * py) {
  double sum = 0;
  while(px->index_ != -1 && py->index_ != -1) {
    if(px->index_ == py->index_) {
      sum += px->value_ * py->value_;
      ++px;
      ++py;
    } else {
      if(px->index_ > py->index_) {
        ++py;
      } else {
        ++px;
      }
    }
  }  // while
  return sum;
}


double Kernel::Kfunction(const SvmNode * x,
                         const SvmNode * y,
                         const SvmParameter& param) {
  switch(param.kernel_type_) {
    case LINEAR:
      return Dot(x,y);
    case POLY:
      return powi(param.gamma_* Dot(x, y) + param.coef0_, param.degree_);
    case RBF: {
      double sum = 0;
      while(x->index_ != -1 && y->index_ !=-1) {
        if(x->index_ == y->index_) {
          double d = x->value_ - y->value_;
          sum += d * d;
          ++x;
          ++y;
        } else {
          if(x->index_ > y->index_) {
            sum += y->value_ * y->value_;
            ++y;
          } else {
            sum += x->value_ * x->value_;
            ++x;
          }
        }  // if
      }  // while

      while(x->index_ != -1) {
        sum += x->value_ * x->value_;
        ++x;
      }
      while(y->index_ != -1) {
        sum += y->value_ * y->value_;
        ++y;
      }
      return exp(-param.gamma_ * sum);
    }

    case SIGMOID:
      return tanh(param.gamma_ * Dot(x,y) + param.coef0_);
    
    case PRECOMPUTED:  //x: test (validation), y: SV
      return x[(int)(y->value_)].value_;
    
    default:
      return 0;  // Unreachable 
  }
}


SVC_Q::SVC_Q(const SvmProblem & prob,
             const SvmParameter & param,
             const schar *y) :Kernel(prob.l_, prob.x_, param) {
  clone(y_, y, prob.l_);
  cache_ = new Cache(prob.l_, static_cast<int64_t> (param.cache_size_ * (1 << 20)));
  QD_ = new double[prob.l_];
  for(int i = 0;i < prob.l_; i++) {
    QD_[i] = (this->*kernel_function_)(i, i);
  }
}
  
Qfloat * SVC_Q::GetQ(int i, int len) const {
  Qfloat *data;
  int start, j;
  if((start = cache_->GetData(i, &data, len)) < len) {
    for(j = start; j < len; j++) {
      data[j] = (Qfloat)(y_[i] * y_[j] * (this->*kernel_function_)(i, j));
    }
  }
  return data;
}

double * SVC_Q::GetQD() const {
  return QD_;
}

void SVC_Q::SwapIndex(int i, int j) const {
  cache_->SwapIndex(i, j);
  Kernel::SwapIndex(i, j);
  swap(y_[i], y_[j]);
  swap(QD_[i], QD_[j]);
}

SVC_Q::~SVC_Q() {
  delete[] y_;
  delete cache_;
  delete[] QD_;
}

}  // namepace nltk
