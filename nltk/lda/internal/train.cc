// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-15 06:40:57
// File  : train.cc
// Brief :

#include "nltk/lda/public/train.h"

#include <algorithm>
#include <vector>
#include <utility>
#include "nltk/lda/public/gibbs_sample.h"
#include "base/public/string_util.h"
#include "file/public/file.h"

namespace nltk {
namespace lda {

bool CompPair(const std::pair<double, int> & a,
              const std::pair<double, int> & b) {
  return a.first > b.first;
}

Train::Train(const std::string & tdata_path, const std::string & dir) {
  dir_ = dir;
  problem_.reset(new Problem(tdata_path));
  model_.reset(new TrainModel());
  model_->Init(*problem_);
}

void Train::Save(const std::string & suffix) {
  modelout model_out;
  model_->Transfer(&model_out);
  std::string content = base::FromThriftToString(&model_out);
  std::string content_debug = base::FromThriftToUtf8DebugString(&model_out);
  file::File::WriteStringToFile(content, dir_ + "/model." + suffix);
  file::File::WriteStringToFile(content_debug, dir_ + "/model-debug." + suffix);

  topic_word topic_word_out;
  for (int i = 0; i < model_->phi_.size(); i++) {
    std::vector<std::pair<double, int> > v;
    for (int j = 0; j < model_->phi_[i].size(); j++) {
      v.push_back(std::make_pair(model_->phi_[i][j], j));
    }
    int offset = v.size() < model_->out_words_ ? v.size() : model_->out_words_;
    std::partial_sort(v.begin(), v.begin() + offset, v.end(), CompPair);

    std::vector<std::string> words_str;
    for (int j = 0; j < offset; j++) {
      std::string value = model_->id2word_[v[j].second] + "/" +
                          DoubleToString(v[j].first);
      words_str.push_back(value);
    }
    topic_word_out.top_words.push_back(words_str);
  }
  content_debug = base::FromThriftToUtf8DebugString(&topic_word_out);
  file::File::WriteStringToFile(content_debug, dir_ + "/topic-words." + suffix);

  doc_topic doc_topic_out;
  for (int i = 0; i < model_->theta_.size(); i++) {
    std::vector<std::pair<double, int> > v;
    for (int j = 0; j < model_->theta_[i].size(); j++) {
      v.push_back(std::make_pair(model_->theta_[i][j], j));
    }
    int offset = v.size() < model_->out_topics_ ?
                 v.size() : model_->out_topics_;
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
  file::File::WriteStringToFile(content_debug, dir_ + "/doc-topics." + suffix);
}

void Train::DoTrain() {
  GibbsSample::GetInstance().InitTrain(model_.get());
  for (int i = 0; i < model_->niter_; i += model_->save_niter_) {
    LOG(INFO) << "train :" << i << "/" << model_->niter_;
    int steps = std::min(model_->save_niter_, model_->niter_ - i);
    GibbsSample::GetInstance().IterTrain(model_.get(), steps);
    std::string suffix = IntToString(i + steps);
    LOG(INFO) << "save :" << i << "/" << model_->niter_;
    Save(suffix);
  }
}

}  // namespace lda
}  // namespace nltk
