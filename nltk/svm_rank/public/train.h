// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 01:51:02
// File  : train.h
// Brief :

#ifndef  NLTK_SVM_RANK_PUBLIC_TRAIN_H_
#define  NLTK_SVM_RANK_PUBLIC_TRAIN_H_

#include "nltk/svm/public/train.h"
#include <string>

namespace nltk {
namespace svm_rank {
class Train : public svm::Train {
 public:
  static Train & GetInstance() {
    return *Singleton<Train>::get();
  }
  Train() {}
  void ReadFile(const std::string & path);

 private:
  DISALLOW_COPY_AND_ASSIGN(Train);
};

}  // namespace svm_rank
}  // namespace nltk

#endif  // NLTK_SVM_RANK_PUBLIC_TRAIN_H_
