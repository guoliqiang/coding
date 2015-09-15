// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 18:40:57
// File  : model.cc
// Brief :

#include "nltk/lda/public/model.h"

#include "file/public/file.h"

namespace nltk {

namespace lda {

DEFINE_double(beta, 0.1, "");
DEFINE_int32(k, 100, "");
DEFINE_double(alpha, 50, "");
DEFINE_int32(niter, 2000, "");
DEFINE_int32(save_niter, 200, "");
DEFINE_int32(twords, 10, "");

void TrainModel::LoadModel(const std::string & path) {
  std::string content = "";
  file::File::ReadFileToString(path, &content);
  modelout model;
  base::FromStringToThrift(content, &model);
  M_ = model.M;
  V_ = model.V;
  K_ = model.K;
  alpha_ = model.alpha;
  beta_ = model.beta;
  niter_ = model.niter;
  save_niter_ = model.save_niter;
  twords_ = model.twords;
  word2id_ = model.word2id;
  phi_ = model.phi;
  theta_ = model.theta;
  nword_ = model.nword;
  ntopic_ = model.ntopic;
  z_ = model.z;
  nword_sum_ = model.nword_sum;
  doc_words_ = model.doc_words;
  for (int i = 0; i < doc_words_.size(); i++) {
    doc_len_.push_back(doc_words_[i].size());
  }
  for (std::map<std::string, int>::iterator i = word2id_.begin();
       i != word2id_.end(); i++) {
    id2word_.insert(std::make_pair(i->second, i->first));
  }
}

void TrainModel::Transfer(modelout * model) {
  model->M = M_;
  model->V = V_;
  model->K = K_;
  model->alpha = alpha_;
  model->beta = beta_;
  model->niter = niter_;
  model->save_niter = save_niter_;
  model->twords = twords_;
  model->word2id = word2id_;
  model->phi = phi_;
  model->theta = theta_;
  model->nword = nword_;
  model->ntopic = ntopic_;
  model->z = z_;
  model->nword_sum = nword_sum_;
  model->doc_words = doc_words_;
}

TrainModel::TrainModel(const Problem & problem) {
  M_ = problem.doc_words_.size();
  K_ = FLAGS_k;
  alpha_ = FLAGS_alpha == 50 ? FLAGS_alpha / K_ : FLAGS_alpha;
  beta_ = FLAGS_beta;
  niter_ = FLAGS_niter;
  save_niter_ = FLAGS_save_niter;
  twords_ = FLAGS_twords;
  doc_words_.resize(M_);

  for (int i = 0; i < problem.doc_words_.size(); i++) {
    doc_len_.push_back(problem.doc_words_[i].size());
    for (int j = 0; j < problem.doc_words_[i].size(); j++) {
      const std::string & word = problem.doc_words_[i][j];
      if (!word2id_.count(word)) {
        word2id_.insert(std::make_pair(word, word2id_.size()));
        id2word_.insert(std::make_pair(word2id_[word], word));
      }
      doc_words_[i].push_back(word2id_[word]);
    }
  }
  V_ = word2id_.size();

  for (int i = 0; i < K_; i++) {
    phi_.push_back(std::vector<double>(V_, 0.0));
    nword_sum_.push_back(0);
  }
  for (int i = 0; i < M_; i++) {
    theta_.push_back(std::vector<double>(K_, 0.0));
    ntopic_.push_back(std::vector<int>(K_, 0));
    z_.push_back(std::vector<int>(doc_len_[i], 0));
  }
  for (int i = 0; i < V_; i++) {
    nword_.push_back(std::vector<int>(K_, 0));
  }
}

PredictModel::PredictModel(const Problem & problem, const TrainModel & model) {
  M_ = problem.doc_words_.size();
  doc_words_.resize(M_);

  for (int i = 0; i < problem.doc_words_.size(); i++) {
    doc_len_.push_back(problem.doc_words_[i].size());
    for (int j = 0; j < problem.doc_words_[i].size(); j++) {
      const std::string & word = problem.doc_words_[i][j];
      if (!model.word2id_.count(word)) {
        LOG(INFO) << "ignore word " << word;
        continue;
      }
      if (!word2id_.count(word)) {
        word2id_.insert(std::make_pair(word, word2id_.size()));
        id2word_.insert(std::make_pair(word2id_[word], word));
        id2id_.insert(std::make_pair(word2id_[word],
            model.word2id_.find(word)->second));
      }
      doc_words_[i].push_back(word2id_[word]);
    }
  }
  V_ = word2id_.size();

  for (int i = 0; i < model.K_; i++) {
    phi_.push_back(std::vector<double>(V_, 0.0));
    nword_sum_.push_back(0);
  }
  for (int i = 0; i < M_; i++) {
    theta_.push_back(std::vector<double>(model.K_, 0.0));
    ntopic_.push_back(std::vector<int>(model.K_, 0));
    z_.push_back(std::vector<int>(doc_len_[i], 0));
  }
  for (int i = 0; i < V_; i++) {
    nword_.push_back(std::vector<int>(model.K_, 0));
  }
}

void PredictModel::Transfer(modelout * model) {
  model->M = M_;
  model->V = V_;
  model->word2id = word2id_;
  model->id2id = id2id_;
  model->phi = phi_;
  model->theta = theta_;
  model->nword = nword_;
  model->ntopic = ntopic_;
  model->z = z_;
  model->nword_sum = nword_sum_;
  model->doc_words = doc_words_;
}

void PredictModel::LoadModel(const std::string & path) {
  std::string content = "";
  file::File::ReadFileToString(path, &content);
  modelout model;
  base::FromStringToThrift(content, &model);
  M_ = model.M;
  V_ = model.V;
  word2id_ = model.word2id;
  id2id_ = model.id2id;
  phi_ = model.phi;
  theta_ = model.theta;
  nword_ = model.nword;
  ntopic_ = model.ntopic;
  z_ = model.z;
  nword_sum_ = model.nword_sum;
  doc_words_ = model.doc_words;
  for (int i = 0; i < doc_words_.size(); i++) {
    doc_len_.push_back(doc_words_[i].size());
  }
  for (std::map<std::string, int>::iterator i = word2id_.begin();
       i != word2id_.end(); i++) {
    id2word_.insert(std::make_pair(i->second, i->first));
  }
}

}  // namespace lad
}  // namespace nltk
