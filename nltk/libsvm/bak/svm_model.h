// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 18:59:02
// File  : SvmModel.h
// Brief :

#ifndef  __SVM_MODEL_H_
#define  __SVM_MODEL_H_
#include "svm_parameter.h"

namespace nltk {

class SvmModel {
 public:
  SvmParameter param_;  // parameter
  int nr_class_;   // number of classes, = 2 in regression/one class svm
  int l_;      // total #SV
  svm_node **SV_;    // SVs (SV[l])
  
  // coefficients for SVs in decision functions (sv_coef[k-1][l])
  double ** sv_coef_;
  double * rho_;    // constants in decision functions (rho[k*(k-1)/2])
  double * probA_;  // pariwise probability information
  double * probB_;
  // sv_indices[0,...,nSV-1] are values in [1,...,num_traning_data]
  // to indicate SVs in the training set
  int * sv_indices_;

  // for classification only
  int * label_;   // label of each class (label[k])
  // number of SVs for each class (nSV[k])
  // nSV[0] + nSV[1] + ... + nSV[k-1] = l
  int * nSV_;
  // 1 if SvmModel is created by svm_load_model
  // 0 if SvmModel is created by svm_train 
  int free_sv_;
};

}  // namespace nltk

#endif  // __SVM_MODEL_H_
