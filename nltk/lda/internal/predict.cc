// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-15 06:44:01
// File  : predict.cc
// Brief :

#include "nltk/lda/public/predict.h"

namespace nltk {
namespace lda {

Predict::Predict(const std::string & pdata_path,
                 const std::string & tmodel_path,
                 const std::string & dir) {
  dir_ = dir;
}

void Predict::Save(const std::string & suffix) {
}

void Predict::DoPredict() {
}

}  // namespace lda
}  // namespace nltk
