// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-08 12:13:55
// File  : predict.h
// Brief :

#ifndef  NLTK_SVM_PUBLIC_PREDICT_H_
#define  NLTK_SVM_PUBLIC_PREDICT_H_

#include "nltk/svm/public/model.h"
#include "nltk/svm/public/kernel.h"
#include "base/public/string_util.h"

namespace nltk {
namespace svm {
class Predict {
 public:
  static Predict & GetInstance() {
    return * Singleton<Predict>::get();
  }
  Predict() {}

  void LoadModel(const std::string & path);
  void SvmPredict(const std::string & input, const std::string & output);
  int32_t SvmPredict(ProblemNode & input,
      std::map<std::pair<int32_t, int32_t>, double> * class_socre = NULL);
  int32_t SvmPredictFast(ProblemNode & input,
      std::map<std::pair<int32_t, int32_t>, double> * class_socre = NULL);
  void KernelValue(const ProblemNode & input, std::vector<double> * rs);

 private:
  bool Free(const double alpha);

 private:
  DISALLOW_COPY_AND_ASSIGN(Predict);
};
}  // namespace svm
}  // namespace nltk

#endif  // NLTK_SVM_PUBLIC_PREDICT_H_
