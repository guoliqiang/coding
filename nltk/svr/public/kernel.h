// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-07 23:21:29
// File  : kernel.h
// Brief :

#ifndef  __KERNEL_H_
#define  __KERNEL_H_

#include "base/public/logging.h"
#include "base/public/normal_sparse_array.h"
#include "base/public/singleton.h"
#include "third_part/string/public/string_enum.h"
#include "base/public/shared_ptr.h"
#include "base/public/string_util.h"
#include <math.h>

namespace nltk {
namespace svr {

enum KernelType {
  UNKNOWN = 0,
  LINEAR = 1,
  POLY = 2,
  RBF = 3,
  SIGMOID = 4,
};
}  // namesace svr
}  // namespace nltk

BEGIN_STRINGMAP_PARSER(nltk::svr::KernelType)
  STRINGMAP_ENUM(nltk::svr::UNKNOWN)
  STRINGMAP_ENUM(nltk::svr::LINEAR)
  STRINGMAP_ENUM(nltk::svr::POLY)
  STRINGMAP_ENUM(nltk::svr::RBF)
  STRINGMAP_ENUM(nltk::svr::SIGMOID)
END_STRINGMAP_PARSER()

namespace nltk {
namespace svr {

class Parameter {
 public:
  Parameter();
  std::string ToString();
  
 public:
  double c_;
  double degree_;
  double gamma_;
  double coef0_;
  int32_t mem_size_;
  double eps_;
  double p_;
  KernelType kernel_type_;
};

struct ProblemNode {
  int32_t line_no;
  int32_t value;
  base::NormalSarray<double> element;
  
  ProblemNode() : line_no(-1), value(0) {}
  std::string ToString();
};

class Kernel {
 public:
  static Kernel & GetInstance() {
    return * Singleton<Kernel>::get();
  }

  Kernel() {}
  void Set(base::shared_ptr<Parameter> para);

  inline double Do(const ProblemNode & a, const ProblemNode & b) {
    return ((this->*f_)(a, b));
  }

 private:
  inline double dot(const ProblemNode & a, const ProblemNode & b) {
    base::NormalSarray<double>::const_iterator i = a.element.begin();
    base::NormalSarray<double>::const_iterator ei = a.element.end();
    
    base::NormalSarray<double>::const_iterator j = b.element.begin();
    base::NormalSarray<double>::const_iterator ej = b.element.end();
    if (i == ei || j == ej) return 0;
    double rs = 0;
    while (i != ei && j!= ej) {
      if (i->first < j->first ) {
        i++;
      } else if (i->first > j->first) {
        ++j;
      } else {
        rs += i->second * j->second;
        i++;
        j++;
      }
    }
    return rs;
  }

  inline double powi(double base, int times) {
    double tmp = base;
    double ret = 1.0;
    for(int t = times; t > 0; t /= 2) {
      if(t % 2 == 1) ret *= tmp;
      tmp = tmp * tmp;
    }
    return ret;
  }
  // a * b
  inline double linear(const ProblemNode & a, const ProblemNode & b) {
    return dot(a, b);   
  }
  // (gamma * a * b + coef0) ^ degree
  inline double poly(const ProblemNode & a, const ProblemNode & b) {
    return powi(para_->gamma_ * dot(a, b) + para_->coef0_, para_->degree_);
  }
  // exp(-gamma * |a - b|^2)
  inline double rbf(const ProblemNode & a, const ProblemNode & b) {
    return exp(-(para_->gamma_) * (dot(a, a) + dot(b, b) - 2 * dot(a, b)));
  }
  // tanh(gamma * a * b + coef0)
  inline double sigmoid(const ProblemNode & a, const ProblemNode & b) {
    return tanh(para_->gamma_ * dot(a, b) + para_->coef0_);
  }

 private:
  double (Kernel::*f_)(const ProblemNode & a, const ProblemNode & b);
  base::shared_ptr<Parameter> para_;
 
 private:
  DISALLOW_COPY_AND_ASSIGN(Kernel);
};

}  // namesace svr
}  // namespace nltk

#endif  //__KERNEL_H_
