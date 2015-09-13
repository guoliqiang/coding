// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 17:07:53
// File  : train.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_TRAIN_H_
#define  NLTK_LDA_PUBLIC_TRAIN_H_

#include "base/public/logging.h"
#include "nltk/lad/public/model.h"
#include "base/public/shared_ptr.h"

namespace lda {

class Train {
 public:
 private:
  base::shared_ptr<Problem> problem_;
  base::shared_ptr<TrainModel> model_;
};

}  // namespace lda

#endif  // NLTK_LDA_PUBLIC_TRAIN_TRAIN_H_
