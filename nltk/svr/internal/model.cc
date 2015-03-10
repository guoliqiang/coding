// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 01:05:04
// File  : model.cc
// Brief :

#include "nltk/svr/public/model.h"

#include<string>

namespace nltk {
namespace svr {

void Model::LoadModel(const std::string & path) {
  std::string content = "";
  file::File::ReadFileToString(path, &content);
  modelout model_out;
  base::FromStringToThrift(content, &model_out);
  para_.reset(new Parameter());
  Transfer(model_out.para, para_.get());
  VLOG(3) << para_->ToString();

  for (std::map<int32_t, featurenode>::iterator i =
       model_out.feature_max_min.begin();
       i != model_out.feature_max_min.end(); i++) {
    base::shared_ptr<std::pair<double, double> >
          foo(new std::pair<double, double>());
    foo->first = i->second.max;
    foo->second = i->second.min;
    feature_max_min_.insert(std::make_pair(i->first, foo));
  }

  model_.reset(new ModelNode());
  Transfer(model_out.model, model_.get());
  for (int i = 0; i < model_out.node.size(); i++) {
    base::shared_ptr<ProblemNode> foo(new ProblemNode());
    Transfer(model_out.node[i], foo.get());
    node_.push_back(foo);
  }
}

}  // namespace svr
}  // namespace nltk
