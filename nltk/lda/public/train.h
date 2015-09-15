// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 17:07:53
// File  : train.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_TRAIN_H_
#define  NLTK_LDA_PUBLIC_TRAIN_H_

#include <string>
#include "base/public/logging.h"
#include "nltk/lda/public/model.h"
#include "nltk/lda/public/problem.h"
#include "base/public/shared_ptr.h"

namespace nltk {

namespace lda {

class Train {
 public:
  Train(const std::string & tdata_path, const std::string & dir);
  void Save(const std::string & suffix);
  void DoTrain();

 private:
  std::string dir_;
  base::shared_ptr<Problem> problem_;
  base::shared_ptr<TrainModel> model_;
};

}  // namespace lda
}  // namespace nltk

#endif  // NLTK_LDA_PUBLIC_TRAIN_TRAIN_H_
