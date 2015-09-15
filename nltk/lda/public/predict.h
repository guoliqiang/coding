// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 17:10:41
// File  : predict.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_PREDICT_H_
#define  NLTK_LDA_PUBLIC_PREDICT_H_

#include <string>
#include "base/public/logging.h"
#include "nltk/lda/public/problem.h"
#include "nltk/lda/public/model.h"
#include "base/public/shared_ptr.h"

namespace nltk {
namespace lda {

class Predict {
 public:
  Predict(const std::string & pdata_path,
          const std::string & tmodel_path,
          const std::string & dir);
  void Save(const std::string & suffix);
  void DoPredict();

 private:
  std::string dir_;
  base::shared_ptr<Problem> problem_;
  base::shared_ptr<PredictModel> pmodel_;
  base::shared_ptr<TrainModel> tmodel_;
};

}  // namespace lad
}  // namespace nltk

#endif  // NLTK_LDA_PUBLIC_PREDICT_H_
