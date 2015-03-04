// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 12:13:55
// File  : predict.h
// Brief :

#ifndef  __PREDICT_H_
#define  __PREDICT_H_

#include "model.h"
#include "kernel.h"
#include "base/public/string_util.h"

namespace nltk {
namespace svm {
class Predict {
 public:
  static Predict & GetInstance() {
    return * Singleton<Predict>::get();
  }

  Predict() {}

  void LoadModel(const std::string path);
  void SvmPredict(const std::string input,
                  const std::string output);
  int32_t SvmPredict(ProblemNode & input);

 private:
  bool Free(const double alpha, int32_t lable);

 private:
  DISALLOW_COPY_AND_ASSIGN(Predict);
};
}  // namespace svm
}  // namespace nltk

#endif  //  __PREDICT_H_
