// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 00:55:43
// File  : model.h
// Brief :

#ifndef  __MODEL_H_
#define  __MODEL_H_

#include <vector>
#include <map>
#include "kernel.h"
#include "file/public/file.h"
#include "third_part/thrift/wrapper/public/thrift.h"
#include "thrift-out/nltk/svm/proto/gen-cpp/svm_types.h"

namespace nltk {
namespace svm {

struct ModelNode {
 double b;
 base::NormalSarray<double> alpha;
 double best_obj;
};

class Model {
 public:
  static Model & GetInstance() {
    return *Singleton<Model>::get();
  }

  Model() {}
  
  void LoadModel(const std::string & path);

 public:
  base::shared_ptr<Parameter> para_;
  // class C --
  //           class B  -- model
  //           class A  -- model
  // class B --
  //           class A  -- model
  std::map<int32_t,
           base::shared_ptr<std::map<int32_t, base::shared_ptr<ModelNode> > >
          > model_;
  std::vector<base::shared_ptr<ProblemNode> > node_;
  std::map<int32_t, int32_t> start_;
  std::map<int32_t, int32_t> count_;
  std::map<int32_t,
           base::shared_ptr<std::pair<double, double> > > feature_max_min_;


 private:
  void Transfer(const parameter & a, Parameter * b) {
     b->c_ = a.c;
     b->weights_ = a.weights;
     b->degree_ = a.degree;
     b->gamma_ = a.gamma;
     b->coef0_ = a.coef0;
     b->mem_size_ = a.mem_size;
     b->eps_ = a.eps;
     b->kernel_type_ = static_cast<KernelType>(a.kernel_type);
  }

  void Transfer(const problemnode & a, ProblemNode * b) {
    b->lable = a.lable;
    b->element.set(a.sparse_array);
  }

  void Transfer(const modelnode & a, ModelNode * b) {
    b->b = a.b;
    b->best_obj = a.best_obj;
    b->alpha.set(a.sparse_array);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Model);
};
}  // namespace svm
}  // namespace nltk

#endif  //  __MODEL_H_
