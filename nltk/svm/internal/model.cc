// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 01:05:04
// File  : model.cc
// Brief :

#include "../public/model.h"

namespace nltk {
namespace svm {

void Model::LoadModel(const std::string & path) {
  std::string content = "";
  file::File::ReadFileToString(path, &content);
  modelout model_out;
  base::FromStringToThrift(content, &model_out);
  para_.reset(new Parameter());
  Transfer(model_out.para, para_.get());
  para_->LogContent();
  start_ = model_out.start;
  count_ = model_out.count;
  for (std::map<int32_t, featurenode>::iterator i =
       model_out.feature_max_min.begin();
       i != model_out.feature_max_min.end(); i++) {
    base::shared_ptr<std::pair<double, double> >
          foo(new std::pair<double, double>());
    foo->first = i->second.max;
    foo->second = i->second.min;
    feature_max_min_.insert(std::make_pair(i->first, foo));
  }

  for (std::map<int32_t, std::map<int32_t, modelnode> >::iterator
       i = model_out.model.begin(); i != model_out.model.end();
       i++) {
    if (!model_.count(i->first)) {
      base::shared_ptr<std::map<int32_t,
            base::shared_ptr<ModelNode> > > bar;
      bar.reset(new std::map<int32_t, base::shared_ptr<ModelNode> >());
      model_.insert(std::make_pair(i->first, bar));
    }
    for (std::map<int32_t, modelnode>::iterator j = i->second.begin();
         j != i->second.end(); j++) {
      if (!model_[i->first]->count(j->first)) {
        base::shared_ptr<ModelNode> foo(new ModelNode());
        model_[i->first]->insert(std::make_pair(j->first, foo));
      }
      Transfer(j->second, (*model_[i->first].get())[j->first].get());
    }
  }
  for (int i = 0; i < model_out.node.size(); i++) {
    base::shared_ptr<ProblemNode> foo(new ProblemNode());
    Transfer(model_out.node[i], foo.get());
    node_.push_back(foo);
  }
}

}  // namespace svm
}  // namespace nltk
