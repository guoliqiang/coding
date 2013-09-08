// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-04 18:52:37
// File  : svm_problem.h
// Brief :

#ifndef  __SVM_PROBLEM_H_
#define  __SVM_PROBLEM_H_
namespace nltk {
class SvmNode;

class SvmProblem {
 public:
  int l_;
  double * y_;
  SvmNode **x_;
};

}  // namespace nltk
#endif  // __SVM_PROBLEM_H_
