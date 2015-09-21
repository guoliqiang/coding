// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-15 06:44:01
// File  : predict.cc
// Brief :

#include "nltk/lda/public/predict.h"

#include <algorithm>
#include <vector>
#include <utility>
#include "nltk/lda/public/gibbs_sample.h"
#include "file/public/file.h"
#include "base/public/string_util.h"

namespace nltk {
namespace lda {

bool CompPair(const std::pair<double, int> & a,
              const std::pair<double, int> & b);

Predict::Predict(const std::string & pdata_path,
                 const std::string & tmodel_path,
                 const std::string & dir) {
  dir_ = dir;
  problem_.reset(new Problem(pdata_path));
  tmodel_.reset(new TrainModel());
  pmodel_.reset(new PredictModel());

  tmodel_->LoadModel(tmodel_path);
  pmodel_->Init(*problem_, *tmodel_);
}

void Predict::Save(const std::string & suffix) {
  modelout model_out;
  pmodel_->Transfer(&model_out);
  std::string content = base::FromThriftToString(&model_out);
  std::string content_debug = base::FromThriftToUtf8DebugString(&model_out);
  file::File::WriteStringToFile(content, dir_ + "/pmodel." + suffix);
  file::File::WriteStringToFile(content_debug, dir_ + "/pmodel-debug." +
                                suffix);

  topic_word topic_word_out;
  for (int i = 0; i < pmodel_->phi_.size(); i++) {
    std::vector<std::pair<double, int> > v;
    for (int j = 0; j < pmodel_->phi_[i].size(); j++) {
      v.push_back(std::make_pair(pmodel_->phi_[i][j], j));
    }
    int offset = v.size() < tmodel_->out_words_ ?
                 v.size() : tmodel_->out_words_;
    std::partial_sort(v.begin(), v.begin() + offset, v.end(), CompPair);

    std::vector<std::string> words_str;
    for (int j = 0; j < offset; j++) {
      std::string value = pmodel_->id2word_[v[j].second] + "/" +
                          DoubleToString(v[j].first);
      words_str.push_back(value);
    }
    topic_word_out.top_words.push_back(words_str);
  }
  content_debug = base::FromThriftToUtf8DebugString(&topic_word_out);
  file::File::WriteStringToFile(content_debug, dir_ + "/ptopic-words." +
                                suffix);

  doc_topic doc_topic_out;
  for (int i = 0; i < pmodel_->theta_.size(); i++) {
    std::vector<std::pair<double, int> > v;
    for (int j = 0; j < pmodel_->theta_[i].size(); j++) {
      v.push_back(std::make_pair(pmodel_->theta_[i][j], j));
    }
    int offset = v.size() < tmodel_->out_topics_ ?
                 v.size() : tmodel_->out_topics_;
    std::partial_sort(v.begin(), v.begin() + offset, v.end(), CompPair);

    std::vector<std::string> words_str;
    for (int j = 0; j < offset; j++) {
      std::string value = IntToString(v[j].second)+ "/" +
                          DoubleToString(v[j].first);
      words_str.push_back(value);
    }
    doc_topic_out.top_topics.push_back(words_str);
  }
  content_debug = base::FromThriftToUtf8DebugString(&doc_topic_out);
  file::File::WriteStringToFile(content_debug, dir_ + "/pdoc-topics." + suffix);
}

void Predict::DoPredict() {
  GibbsSample::GetInstance().InitPredict(*tmodel_, pmodel_.get());
  for (int i = 0; i < tmodel_->niter_; i += tmodel_->save_niter_) {
    int steps = std::min(tmodel_->save_niter_, tmodel_->niter_ - i);
    GibbsSample::GetInstance().IterPredict(*tmodel_, steps, pmodel_.get());
    std::string suffix = IntToString(i + steps);
    Save(suffix);
  }
}

}  // namespace lda
}  // namespace nltk
