// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 09:01:13
// File  : train.h
// Brief :

#ifndef  NLTK_SVR_PUBLIC_TRAIN_H_
#define  NLTK_SVR_PUBLIC_TRAIN_H_

#include "nltk/svr/public/kernel.h"
#include "nltk/svr/public/problem.h"
#include "nltk/svr/public/model.h"
#include "nltk/svr/public/solver.h"

#include "base/public/scoped_ptr.h"
#include "base/public/shared_ptr.h"
#include "thrift-out/nltk/svm/proto/gen-cpp/svm_types.h"
#include "third_part/thrift/wrapper/public/thrift.h"
#include "file/public/file.h"

namespace nltk {
namespace svr {

class Train {
 public:
  static Train & GetInstance() {
    return *Singleton<Train>::get();
  }

  Train() {}
  void ReadFile(const std::string path);
  void SvrTrain();
  void WriteModel(std::string path);

 private:
  void Transfer(ProblemNode & a, problemnode *b) {
    b->value = a.value;
    b->sparse_array = a.element.Get();
  }

  void Transfer(ModelNode & a, modelnode * b) {
    b->b = a.b;
    b->best_obj = a.best_obj;
    b->sparse_alpha = a.alpha.Get();
  }

  void Transfer(const Parameter & a, parameter * b) {
    b->c = a.c_;
    b->degree = a.degree_;
    b->gamma = a.gamma_;
    b->coef0 = a.coef0_;
    b->mem_size = a.mem_size_;
    b->eps = a.eps_;
    b->p = a.p_;
    b->kernel_type = a.kernel_type_;
  }

  private:
   DISALLOW_COPY_AND_ASSIGN(Train);
};

}  // namespace svr
}  // namespace nltk

#endif  // NLTK_SVR_PUBLIC_TRAIN_H_
