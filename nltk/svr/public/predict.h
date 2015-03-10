// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 12:13:55
// File  : predict.h
// Brief :

#ifndef  __PREDICT_H_
#define  __PREDICT_H_

#include "nltk/svr/public/model.h"
#include "nltk/svr/public/kernel.h"
#include "base/public/string_util.h"

namespace nltk {
namespace svr {
class Predict {
 public:
  static Predict & GetInstance() {
    return * Singleton<Predict>::get();
  }
  Predict() {}

  void LoadModel(const std::string path);
  void SvrPredict(const std::string input, const std::string output);
  double SvrPredict(ProblemNode & input);
  void KernelValue(const ProblemNode & input, std::vector<double> * rs);

 private:
  bool Free(const double alpha);

 private:
  DISALLOW_COPY_AND_ASSIGN(Predict);
};
}  // namespace svm
}  // namespace nltk

#endif  //  __PREDICT_H_
