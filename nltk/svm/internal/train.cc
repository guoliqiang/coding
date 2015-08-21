// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-08 09:35:10
// File  : train.cc
// Brief :

#include "nltk/svm/public/train.h"
#include "nltk/svm/public/scale.h"

namespace nltk {
namespace svm {

void Train::ReadFile(const std::string & path) {
  CHECK(Problem::GetInstance().ReadFile(path));
  Kernel::GetInstance().Set(Problem::GetInstance().para_);
  Model::GetInstance().para_ = Problem::GetInstance().para_;
  Model::GetInstance().node_ = Problem::GetInstance().node_;
  Model::GetInstance().start_ = Problem::GetInstance().start_;
  Model::GetInstance().count_ = Problem::GetInstance().count_;
  Model::GetInstance().feature_max_min_ =
      MaxMinScale::GetInstance()->feature_max_min_;
}

void Train::SvmTrain() {
  std::vector<base::shared_ptr<ProblemNode> > a;
  std::vector<base::shared_ptr<ProblemNode> > b;
  // label from +1 to -1 of start
  for (std::map<int32_t, int32_t>::reverse_iterator i =
       Problem::GetInstance().start_.rbegin();
       i != Problem::GetInstance().start_.rend(); i++) {
    a.clear();
    for (int k = 0; k < Problem::GetInstance().count_[i->first]; k++) {
      a.push_back(Problem::GetInstance().node_[i->second + k]);
    }

    std::map<int32_t, int32_t>::reverse_iterator next = i;
    next++;
    for (std::map<int32_t, int32_t>::reverse_iterator j = next;
         j != Problem::GetInstance().start_.rend(); j++) {
      b.clear();
      for (int k = 0; k < Problem::GetInstance().count_[j->first]; k++) {
        b.push_back(Problem::GetInstance().node_[j->second + k]);
      }

      base::shared_ptr<ModelNode> ptr(new ModelNode());
      if (!Model::GetInstance().model_.count(i->first)) {
        base::shared_ptr<std::map<int32_t, base::shared_ptr<ModelNode> > >
            tmp(new std::map<int32_t, base::shared_ptr<ModelNode> >());
        Model::GetInstance().model_.insert(std::make_pair(i->first, tmp));
      }
      Model::GetInstance().model_[i->first]->insert(
          std::make_pair(j->first, ptr));

      LOG(INFO) << "solve class " << i->first << " V.S. class " << j->first;
      scoped_ptr<Solver> solver(new SMO(a, b, Problem::GetInstance().para_));
      solver->Do(ptr.get());
    }
  }
}

void Train::WriteModel(const std::string & path) {
  modelout model_out;
  Transfer(*(Problem::GetInstance().para_.get()), &model_out.para);
  model_out.start = Problem::GetInstance().start_;
  model_out.count = Problem::GetInstance().count_;
  for (std::map<int32_t, base::shared_ptr<std::pair<double, double> > >
       ::iterator i = MaxMinScale::GetInstance()->feature_max_min_.begin();
       i != MaxMinScale::GetInstance()->feature_max_min_.end(); i++) {
    featurenode foo;
    foo.max = i->second->first;
    foo.min = i->second->second;
    model_out.feature_max_min.insert(std::make_pair(i->first, foo));
  }

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
         j = i->second->begin(); j != i->second->end(); j++) {
      if (!model_out.model.count(i->first)) {
        model_out.model.insert(std::make_pair(i->first,
                                              std::map<int32_t, modelnode>()));
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

}  // svm
}  // namespace
