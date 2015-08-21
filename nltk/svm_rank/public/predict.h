// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 03:54:52
// File  : predict.h
// Brief :

#ifndef  NLTK_SVM_RANK_PUBLIC_PREDICT_H_
#define  NLTK_SVM_RANK_PUBLIC_PREDICT_H_

#include <map>
#include <vector>
#include <string>
#include "nltk/svm/public/predict.h"

namespace nltk {
namespace svm_rank {

class Predict : public svm::Predict {
 public:
  void SvmPredict(const std::string & input, const std::string & output);

 private:
  DISALLOW_COPY_AND_ASSIGN(Predict);
};

}  // namespace svm_rank
}  // namespace nltk

#endif  // NLTK_SVM_RANK_PUBLIC_PREDICT_H_
