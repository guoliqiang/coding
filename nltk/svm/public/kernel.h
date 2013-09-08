// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
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
#include <math.h>
#include "base/public/string_util.h"

namespace nltk {
namespace svm {

enum KernelType {
  UNKNOWN = 0,
  LINEAR = 1,
  POLY = 2,
  RBF = 3,
  SIGMOID = 4,
};
}  // namesace svm
}  // namespace nltk

BEGIN_STRINGMAP_PARSER(nltk::svm::KernelType)
  STRINGMAP_ENUM(nltk::svm::UNKNOWN)
  STRINGMAP_ENUM(nltk::svm::LINEAR)
  STRINGMAP_ENUM(nltk::svm::POLY)
  STRINGMAP_ENUM(nltk::svm::RBF)
  STRINGMAP_ENUM(nltk::svm::SIGMOID)
END_STRINGMAP_PARSER()


namespace nltk {
namespace svm {

DECLARE_double(c);
DECLARE_double(degree);
DECLARE_double(gamma);
DECLARE_double(coef0);
DECLARE_string(kernel_type);
DECLARE_int32(mem_size);
DECLARE_double(eps);
DECLARE_string(weights);
static util::StringEnumParser<KernelType> g_kernel_type;

class Parameter {
 public:
  Parameter() {
    c_ = FLAGS_c;
    degree_ = FLAGS_degree;
    gamma_ = FLAGS_gamma;
    coef0_ = FLAGS_coef0;
    mem_size_ = FLAGS_mem_size;
    eps_ = FLAGS_eps;
    std::vector<std::string> foo;
    Tokenize(FLAGS_weights, "\t ", &foo);
    for (int i = 0; i < foo.size(); i++) {
      std::vector<std::string> bar;
      SplitString(foo[i], ':', &bar);
      CHECK_EQ(bar.size(), 2);
      weights_[StringToInt(bar[0])] = StringToDouble(bar[1]);
    }
    CHECK(g_kernel_type.String2Enum(FLAGS_kernel_type, &kernel_type_));
  }

  void LogContent(int l = 0) {
    VLOG(l) << "[paremeter]";
    VLOG(l) << "c:" << c_;
    VLOG(l) << "degree:" << degree_;
    VLOG(l) << "gamma:" << gamma_;
    VLOG(l) << "coef0:" << coef0_;
    VLOG(l) << "mem_size:" << mem_size_;
    VLOG(l) << "eps:" << eps_;
    VLOG(l) << "weights:";
    for (std::map<int32_t, double>::iterator i = weights_.begin();
         i != weights_.end(); i++) {
      VLOG(l) << "\t" << i->first << ":" << i->second;
    }
    std::string foo;
    CHECK(g_kernel_type.Enum2String(kernel_type_, &foo));
    VLOG(l) << "kernel type:" << foo;
    VLOG(l) << "[parameter]";
  }

 public:
  double c_;
  std::map<int32_t, double> weights_;
  double degree_;
  double gamma_;
  double coef0_;
  int32_t mem_size_;
  double eps_;
  KernelType kernel_type_;
};

struct ProblemNode {
  int32_t line_no;
  int32_t lable;
  base::NormalSarray<double> element;
  
  void LogContent(int l = 0) {
    VLOG(l) << "line no:" << line_no;
    VLOG(l) << "lable:" << lable;
    VLOG(l) << "sparse array:";
    VLOG(l) << JoinKeysValues(&element.Get());
  }
};

class Kernel {
 public:
  static Kernel & GetInstance() {
    return * Singleton<Kernel>::get();
  }

  Kernel() {}
  
  void Set(base::shared_ptr<Parameter> para) {
    para_ = para;
    f_ = NULL;
    if (para->kernel_type_ == LINEAR) {
      f_ = &Kernel::linear;
      return;
    } else if (para->kernel_type_ == POLY) {
      f_ = &Kernel::poly;
      return;
    } else if (para->kernel_type_ == RBF) {
      f_ = &Kernel::rbf;
      return;
    } else if (para->kernel_type_ == SIGMOID) {
      f_ = &Kernel::sigmoid;
      return;
    }
    CHECK(false);
  }

  double Do(const ProblemNode & a, const ProblemNode & b) {
    return ((this->*f_)(a, b));
  }

 private:
  double dot(const ProblemNode & a, const ProblemNode & b) {
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

  double powi(double base, int times) {
    double tmp = base;
    double ret = 1.0;
    for(int t = times; t > 0; t /= 2) {
      if(t % 2 == 1) ret *= tmp;
      tmp = tmp * tmp;
    }
    return ret;
  }

  double linear(const ProblemNode & a, const ProblemNode & b) {
    return dot(a, b);   
  }

  double poly(const ProblemNode & a, const ProblemNode & b) {
    return powi(para_->gamma_ * dot(a, b) + para_->coef0_, para_->degree_);
  }

  double rbf(const ProblemNode & a, const ProblemNode & b) {
    return exp(-(para_->gamma_) * (dot(a, a) + dot(b, b) - 2 * dot(a, b)));
  }

  double sigmoid(const ProblemNode & a, const ProblemNode & b) {
    return tanh(para_->gamma_ * dot(a, b) + para_->coef0_);
  }
 private:
  double (Kernel::*f_)(const ProblemNode & a, const ProblemNode & b);
  base::shared_ptr<Parameter> para_;
  DISALLOW_COPY_AND_ASSIGN(Kernel);
};

}  // namesace svm
}  // namespace nltk

#endif  //__KERNEL_H_
