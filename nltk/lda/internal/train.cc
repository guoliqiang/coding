// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-15 06:40:57
// File  : train.cc
// Brief :

#include "nltk/lda/public/train.h"

namespace nltk {
namespace lda {

Train::Train(const std::string & tdata_path, const std::string & dir) {
  dir_ = dir;
}

void Train::Save(const std::string & suffix) {
}

void Train::DoTrain() {
}

}  // namespace lad
}  // namespace nltk
