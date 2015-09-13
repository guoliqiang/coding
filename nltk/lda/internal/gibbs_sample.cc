// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 19:30:56
// File  : gibbs_sample.cc
// Brief :

#include "nltk/lda/public/gibbs_sample.h"

#include <time.h>

namespace lda {

void GibbsSample::InitTrain(TrainModel * model) {
  srandom(time(NULL));
  for (int i = 0; i < model->doc_words_.size(); i++) {
    for (int j = 0; j < model->doc_words_[i].size(); j++) {
      int word = model->doc_words_[i][j];
      int topic = static_cast<int>(static_cast<double>(
          random()) / RAND_MAX * model->K_);
      model->z_[i][j] = topic;
      model->nword_[word][topic]++;
      model->ntopic_[i][topic]++;
      model->nword_sum_[topic]++;
    }
  }
}

int GibbsSample::Sampling(TrainModel * model, int m, int n) {
  int topic = model->z_[m][n];
  int word = model->doc_words_[m][n];
  model->nword_[word][topic]--;
  model->ntopic_[m][topic]--;
  model->nword_sum_[topic]--;
  model->doc_len_[m]--;

  std::vector<double> p(model->K_, 0.0);
  double Vbeta = model->V_ * model->beta_;
  double Kalpha = model->K_ * model->alpha_;

  for (int i = 0; i < model->K_; i++) {
    p[i] = ((model->nword_[word][i] + model->beta_) /
            (Vbeta + model->nword_sum_[i])) *
           ((model->ntopic_[m][topic] + model->alpha_) /
            (model->doc_len_[m] + Kalpha));
  }
  for (int i = 1; i < model->K_; i++) {
    p[i] += p[i - 1];
  }
  double random_v = static_cast<double>(random()) / RAND_MAX
      * p[model->K_ - 1];

  int rs = 0;
  for (rs = 0; rs < model->K_; rs++) {
    if (p[rs] > random_v) break;
  }

  model->doc_len_[m]++;
  model->nword_sum_[rs]++;
  model->ntopic_[m][rs]++;
  model->nword_[word][rs]++;

  return rs;
}

void GibbsSample::ComputeTheta(TrainModel * model) {
  for (int i = 0; i < model->M_; i++) {
    for (int j = 0; j < model->K_; j++) {
      model->theta_[i][j] =
          (model->ntopic_[i][j] + model->alpha_) /
          (model->doc_len_[i] + model->K_ * model->alpha_);
    }
  }
}

void GibbsSample::ComputePhi(TrainModel * model) {
  for (int i = 0; i < model->K_; i++) {
    for (int j = 0; j < model->V_; j++) {
      model->phi_[i][j] =
         (model->nword_[j][i] + model->beta_) /
         (model->nword_sum_[i] + model->V_ * model->beta_);
    }
  }
}

void GibbsSample::IterTrain(TrainModel * model, int steps) {
  for (int step = 0; step < steps; step++) {
    for (int i = 0; i < model->M_; i++) {
      for (int j = 0; j < model->doc_len_[i]; j++) {
        model->z_[i][j] = Sampling(model, i, j);
      }
    }
  }
  ComputePhi(model);
  ComputeTheta(model);
}

}  // namespace lad
