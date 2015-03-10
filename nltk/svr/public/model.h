// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 00:55:43
// File  : model.h
// Brief :

#ifndef  NLTK_SVR_PUBLIC_MODEL_H_
#define  NLTK_SVR_PUBLIC_MODEL_H_

#include "nltk/svr/public/kernel.h"
#include <vector>
#include <map>
#include "file/public/file.h"
#include "third_part/thrift/wrapper/public/thrift.h"
#include "thrift-out/nltk/svr/proto/gen-cpp/svr_types.h"

namespace nltk {
namespace svr {

struct ModelNode {
 double b;
 base::NormalSarray<double> alpha;
 base::NormalSarray<double> zeta;
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
  base::shared_ptr<ModelNode> model_;
  std::vector<base::shared_ptr<ProblemNode> > node_;
  std::map<int32_t,
           base::shared_ptr<std::pair<double, double> > > feature_max_min_;


 private:
  void Transfer(const parameter & a, Parameter * b) {
     b->c_ = a.c;
     b->degree_ = a.degree;
     b->gamma_ = a.gamma;
     b->coef0_ = a.coef0;
     b->mem_size_ = a.mem_size;
     b->eps_ = a.eps;
     b->p_ = a.p;
     b->kernel_type_ = static_cast<KernelType>(a.kernel_type);
  }

  void Transfer(const problemnode & a, ProblemNode * b) {
    b->value = a.value;
    b->element.set(a.sparse_array);
  }

  void Transfer(const modelnode & a, ModelNode * b) {
    b->b = a.b;
    b->best_obj = a.best_obj;
    b->alpha.set(a.sparse_alpha);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Model);
};
}  // namespace svr
}  // namespace nltk

#endif  //  NLTK_SVR_PUBLIC_MODEL_H_
