// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 03:29:38
// File  : train.cc
// Brief :

#include "nltk/svm_rank/public/train.h"
#include "nltk/svm_rank/public/problem.h"

namespace nltk {
namespace svm_rank {

void Train::ReadFile(const std::string & path) {
  svm::Train::ReadFile(path);
  svm::Model::GetInstance().node_ = Problem::GetInstance().transfer_node_;
  svm::Model::GetInstance().start_ = Problem::GetInstance().transfer_start_;
  svm::Model::GetInstance().count_ = Problem::GetInstance().transfer_count_;
}

}  // namespace svm_rank
}  // namespace nltk
