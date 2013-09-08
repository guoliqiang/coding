// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 18:56:34
// File  : svm_parameter.h
// Brief :

#ifndef  __SVM_PARAMETER_H_
#define  __SVM_PARAMETER_H_

namespace nltk {

class SvmParameter {
 public:
  int svm_type_;
  int kernel_type_;
  int degree_;  // for poly
  double gamma_;  // for poly/rbf/sigmoid
  double coef0_;  // for poly/sigmoid

  // these are for training only
  double cache_size_;  // in MB
  double eps_;  // stopping criteria
  double C_;  // for C_SVC, EPSILON_SVR and NU_SVR
  int nr_weight_;    // for C_SVC
  int *weight_label_;  // for C_SVC
  double* weight_;   // for C_SVC
  double nu_;   // for NU_SVC, ONE_CLASS, and NU_SVR
  double p_;    // for EPSILON_SVR
  int shrinking_;  // use the shrinking heuristics
  int probability_; // do probability estimates
};

}  // namespace nltk

#endif  //__SVM_PARAMETER_H_
