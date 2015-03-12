// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 09:01:13
// File  : train.h
// Brief :

#ifndef  NLTK_SVM_PUBLIC_TRAIN_H_
#define  NLTK_SVM_PUBLIC_TRAIN_H_

#include "nltk/svm/public/kernel.h"
#include "nltk/svm/public/problem.h"
#include "nltk/svm/public/model.h"
#include "nltk/svm/public/solver.h"

#include "base/public/scoped_ptr.h"
#include "base/public/shared_ptr.h"
#include "thrift-out/nltk/svm/proto/gen-cpp/svm_types.h"
#include "third_part/thrift/wrapper/public/thrift.h"
#include "file/public/file.h"

namespace nltk {
namespace svm {

class Train {
 public:
  static Train & GetInstance() {
    return *Singleton<Train>::get();
  }

  Train() {}
  void ReadFile(const std::string path);
  void SvmTrain();
  void WriteModel(std::string path);

 private:
  void Transfer(ProblemNode & a, problemnode *b) {
    b->lable = a.lable;
    b->sparse_array = a.element.Get();
  }

  void Transfer(ModelNode & a, modelnode * b) {
    b->b = a.b;
    b->best_obj = a.best_obj;
    b->sparse_alpha = a.alpha.Get();
    b->sparse_zeta = a.zeta.Get();
    b->sparse_w = a.w.Get();
  }

  void Transfer(const Parameter & a, parameter * b) {
    b->c = a.c_;
    b->weights = a.weights_;
    b->degree = a.degree_;
    b->gamma = a.gamma_;
    b->coef0 = a.coef0_;
    b->mem_size = a.mem_size_;
    b->eps = a.eps_;
    b->kernel_type = a.kernel_type_;
  }

  private:
   DISALLOW_COPY_AND_ASSIGN(Train);
};

}  // namespace svm
}  // namespace nltk

#endif  // NLTK_SVM_PUBLIC_TRAIN_H_
