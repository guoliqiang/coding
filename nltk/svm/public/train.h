// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 09:01:13
// File  : train.h
// Brief :

#ifndef  __TRAIN_H_
#define  __TRAIN_H_

#include "kernel.h"
#include "problem.h"
#include "model.h"
#include "solver.h"
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
    b->sparse_array = a.alpha.Get();
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

#endif  // __TRAIN_H_
