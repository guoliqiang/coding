// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 19:30:56
// File  : gibbs_sample.cc
// Brief :

#include "nltk/lda/public/gibbs_sample.h"

#include <time.h>

namespace nltk {

namespace lda {

void GibbsSample::InitTrain(TrainModel * model) {
  srandom(time(NULL));
  for (int i = 0; i < model->M_; i++) {
    for (int j = 0; j < model->doc_len_[i]; j++) {
      int word = model->doc_words_[i][j];
      int topic = static_cast<double>(random()) / RAND_MAX * model->K_;
      model->z_[i][j] = topic;
      model->nword_[word][topic]++;
      model->ntopic_[i][topic]++;
      model->nword_sum_[topic]++;
    }
  }
}

void GibbsSample::InitPredict(const TrainModel & tmodel,
                              PredictModel * pmodel) {
  srandom(time(NULL));
  for (int i = 0; i < pmodel->M_; i++) {
    for (int j = 0; j < pmodel->doc_len_[i]; j++) {
      int word = pmodel->doc_words_[i][j];
      int topic = static_cast<double>(random()) / RAND_MAX * tmodel.K_;
      pmodel->z_[i][j] = topic;
      pmodel->nword_[word][topic]++;
      pmodel->ntopic_[i][topic]++;
      pmodel->nword_sum_[topic]++;
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

  std::vector<double> p;
  double Vbeta = model->V_ * model->beta_;
  double Kalpha = model->K_ * model->alpha_;

  for (int i = 0; i < model->K_; i++) {
    double phi = (model->nword_[word][i] + model->beta_) /
                 (model->nword_sum_[i] + Vbeta);
    double theta = (model->ntopic_[m][i] + model->alpha_) /
                   (model->doc_len_[m] + Kalpha);
    p.push_back(phi * theta);
  }
  for (int i = 1; i < model->K_; i++) p[i] += p[i - 1];

  double random_v = static_cast<double>(random()) / RAND_MAX
      * p[model->K_ - 1];

  int ntopic = 0;
  for (ntopic = 0; ntopic < model->K_; ntopic++) {
    if (p[ntopic] > random_v) break;
  }

  model->doc_len_[m]++;
  model->nword_sum_[ntopic]++;
  model->ntopic_[m][ntopic]++;
  model->nword_[word][ntopic]++;
  return ntopic;
}

int GibbsSample::Sampling(const TrainModel & tmodel,
                          PredictModel * pmodel, int m, int n) {
  int topic = pmodel->z_[m][n];
  int word = pmodel->doc_words_[m][n];
  int old_word = pmodel->id2id_[word];

  pmodel->nword_[word][topic]--;
  pmodel->ntopic_[m][topic]--;
  pmodel->nword_sum_[topic]--;
  pmodel->doc_len_[m]--;

  std::vector<double> p;
  double Vbeta = tmodel.V_ * tmodel.beta_;
  double Kalpha = tmodel.K_ * tmodel.alpha_;

  for (int i = 0; i < tmodel.K_; i++) {
    double phi = (tmodel.nword_[old_word][i] + pmodel->nword_[word][i] +
                  tmodel.beta_) /
                 (tmodel.nword_sum_[i] + pmodel->nword_sum_[i] + Vbeta);
    double theta = (pmodel->ntopic_[m][i] + tmodel.alpha_) /
                   (pmodel->doc_len_[m] + Kalpha);
    p.push_back(phi * theta);
  }
  for (int i = 1; i < tmodel.K_; i++) p[i] += p[i - 1];

  double random_v = static_cast<double>(random()) / RAND_MAX
      * p[tmodel.K_ - 1];

  int ntopic = 0;
  for (ntopic = 0; ntopic < tmodel.K_; ntopic++) {
    if (p[ntopic] > random_v) break;
  }
  pmodel->doc_len_[m]++;
  pmodel->nword_sum_[ntopic]++;
  pmodel->ntopic_[m][ntopic]++;
  pmodel->nword_[word][ntopic]++;
  return ntopic;
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

void GibbsSample::ComputeTheta(const TrainModel & tmodel,
                               PredictModel * pmodel) {
  for (int i = 0; i < pmodel->M_; i++) {
    for (int j = 0; j < tmodel.K_; j++) {
      pmodel->theta_[i][j] =
          (pmodel->ntopic_[i][j] + tmodel.alpha_) /
          (pmodel->doc_len_[i] + tmodel.K_ * tmodel.alpha_);
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

void GibbsSample::ComputePhi(const TrainModel & tmodel,
                             PredictModel * pmodel) {
  for (int i = 0; i < tmodel.K_; i++) {
    for (int j = 0; j < pmodel->V_; j++) {
      int old_word = pmodel->id2id_[j];
      pmodel->phi_[i][j] =
         (tmodel.nword_[old_word][i] + pmodel->nword_[j][i] + tmodel.beta_) /
         (tmodel.nword_sum_[i] + pmodel->nword_sum_[i] +
          tmodel.V_ * tmodel.beta_);
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

void GibbsSample::IterPredict(const TrainModel & tmodel,
                              PredictModel * pmodel, int steps) {
  for (int step = 0; step < steps; step++) {
    for (int i = 0; i < pmodel->M_; i++) {
      for (int j = 0; j < pmodel->doc_len_[i]; j++) {
        pmodel->z_[i][j] = Sampling(tmodel, pmodel, i, j);
      }
    }
  }
  ComputePhi(tmodel, pmodel);
  ComputeTheta(tmodel, pmodel);
}

}  // namespace lad
}  // namespace nltk
