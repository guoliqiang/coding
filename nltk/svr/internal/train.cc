// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 09:35:10
// File  : train.cc
// Brief :

#include "nltk/svr/public/train.h"
#include "nltk/svr/public/scale.h"

namespace nltk {
namespace svr {

void Train::ReadFile(const std::string path) {
  CHECK(Problem::GetInstance().ReadFile(path));
  Kernel::GetInstance().Set(Problem::GetInstance().para_);
  Model::GetInstance().para_ = Problem::GetInstance().para_;
  Model::GetInstance().node_ = Problem::GetInstance().node_;
  Model::GetInstance().model_.reset(new ModelNode());
  Model::GetInstance().feature_max_min_ =
      MaxMinScale::GetInstance()->feature_max_min_;
}

void Train::SvrTrain() {
  scoped_ptr<Solver> solver(new SMO(Problem::GetInstance().node_,
                                    Problem::GetInstance().para_));
  solver->Do(Model::GetInstance().model_.get());
}

void Train::WriteModel(std::string path) {
  modelout model_out;
  Transfer(*(Problem::GetInstance().para_.get()), &model_out.para);
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
  Transfer(*(Model::GetInstance().model_), &(model_out.model));

  std::string content = base::FromThriftToString(&model_out);
  std::string debug_content = base::FromThriftToUtf8DebugString(&model_out);
  file::File::WriteStringToFile(content, path);
  file::File::WriteStringToFile(debug_content, path + ".debug");
}

}  // svr
}  // namespace
