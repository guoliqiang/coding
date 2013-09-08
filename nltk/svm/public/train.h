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

  void ReadFile(const std::string & path) {
    CHECK(Problem::GetInstance().ReadFile(path));
    Kernel::GetInstance().Set(Problem::GetInstance().para_);
    Model::GetInstance().para_ = Problem::GetInstance().para_;
    Model::GetInstance().node_ = Problem::GetInstance().node_;
    Model::GetInstance().start_ = Problem::GetInstance().start_;
    Model::GetInstance().count_ = Problem::GetInstance().count_;
  }
  void SvmTrain() {
    std::vector<base::shared_ptr<ProblemNode> > a;
    std::vector<base::shared_ptr<ProblemNode> > b;
    for (std::map<int32_t, int32_t>::reverse_iterator i =
      Problem::GetInstance().start_.rbegin();
      i != Problem::GetInstance().start_.rend(); i++) {
      a.clear();
      for (int k = 0; k < Problem::GetInstance().count_[i->first]; k++) {
        a.push_back(Problem::GetInstance().node_[i->second + k]);
      }
      std::map<int32_t, int32_t>::reverse_iterator foo = i;
      foo++;
      for (std::map<int32_t, int32_t>::reverse_iterator j = foo;
           j != Problem::GetInstance().start_.rend(); j++) {
        b.clear();
        for (int k = 0; k < Problem::GetInstance().count_[j->first]; k++) {
          b.push_back(Problem::GetInstance().node_[j->second + k]);
        }
        scoped_ptr<Solver> solver(new SMO(a, b, Problem::GetInstance().para_));
           
        base::shared_ptr<ModelNode> ptr(new ModelNode());
        if (!Model::GetInstance().model_.count(i->first)) {
          base::shared_ptr<std::map<int32_t,
                base::shared_ptr<ModelNode> > > foo(new std::map<int32_t,
                base::shared_ptr<ModelNode> >());
          foo->insert(std::make_pair(j->first, ptr));
          Model::GetInstance().model_.insert(std::make_pair(i->first, foo));
        } else {
          Model::GetInstance().model_[i->first]->insert(std::make_pair(j->first, ptr));
        }
        LOG(INFO) << "solve :" << i->first << "~" << j->first;
        solver->Do(ptr.get());
      }
    }
  }

  void WriteModel(std::string & path) {
    modelout model_out;
    Transfer(*(Problem::GetInstance().para_.get()), &model_out.para);
    model_out.start = Problem::GetInstance().start_;
    model_out.count = Problem::GetInstance().count_;
    for (int i = 0; i < Problem::GetInstance().node_.size(); i++) {
      problemnode foo;
      Transfer(*(Problem::GetInstance().node_[i].get()), &foo);
      model_out.node.push_back(foo);
    }
    for (std::map<int32_t,
         base::shared_ptr<std::map<int32_t, base::shared_ptr<ModelNode> > > >
         ::iterator i = Model::GetInstance().model_.begin();
         i != Model::GetInstance().model_.end(); i++) {
      for (std::map<int32_t, base::shared_ptr<ModelNode> >::iterator
           j = i->second->begin();
           j != i->second->end(); j++) {
        if (!model_out.model.count(i->first)) {
          model_out.model.insert(std::make_pair(i->first, std::map<int32_t, modelnode>()));
        }
        modelnode foo;
        Transfer(*(j->second.get()), &foo);
        model_out.model[i->first].insert(std::make_pair(j->first, foo));
      }
    }
    std::string content = base::FromThriftToString(&model_out);
    std::string debug_content = base::FromThriftToUtf8DebugString(&model_out);
    file::File::WriteStringToFile(content, path);
    file::File::WriteStringToFile(debug_content, path + ".debug");
  }

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
