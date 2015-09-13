// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 16:00:38
// File  : model.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_MODEL_H_
#define  NLTK_LDA_PUBLIC_MODEL_H_

#include <map>
#include <string>
#include <vector>
#include "base/public/logging.h"
#include "nltk/lda/public/problem.h"

namespace lda {

class TrainModel {
 public:
  TrainModel(const Problem & problem);

 public:
  int M_;  // number of doc
  int V_;  // number of word
  int K_;  // number of topic
  // Dir(theta_|alpha_)
  // In fact, alpha_ is a vector<alpha_(1), ..., alpha_(k)>
  // of size K_, and alpha_(1) = ... = alpha_(k) = alpha_.
  double alpha_;
  // Dir(phi_|beta_)
  // In fact, alpha_ is a vector<beta_(1), ..., beta_(v)>
  // of size V_, and alpha_(1) = ... = alpha_(v) = beta_.
  double beta_;
  // number of iterations
  int niter_;
  // number of save iterations
  int save_niter_;
  // number of print word for each topic
  int twords_;
  std::map<std::string, int> word2id_;
  std::map<int, std::string> id2word_;
  // K_ x V_
  // phi_[i][j] probility of word[j] in topic[i]
  std::vector<std::vector<double> > phi_;
  // M_ x K_
  // theta_[i][j] probility of topic[j] in doc[i]
  std::vector<std::vector<double> > theta_;
  // V_ x K_
  // nword_[i][j] number of word[i] in topic[j]
  std::vector<std::vector<int> > nword_;
  // M_ x K_
  // ntopic_[i][j] number of topic[j] in doc[i]
  std::vector<std::vector<int> > ntopic_;
  // M_ x doc.size()
  // z_[i][j] topic index of word[j] in doc[i],
  std::vector<std::vector<int> > z_;
  // 1 x K_
  // nword_sum_[i] number of words in topic[i]
  std::vector<int> nword_sum_;
  // 1 x M_
  // doc_len_[i] the length of doc[i]
  std::vector<int> doc_len_;
  // M_ x doc.size()
  // doc_words_[i][j] word at position j in doc[i]
  std::vector<std::vector<int> > doc_words_;
};

class PredictModel {
 public:
  PredictModel(const Problem & problem,
               const TrainModel & model);

 public:
  int M_;  // number of doc
  int V_;  // number of word
  std::map<std::string, int> word2id_;
  std::map<int, std::string> id2word_;
  std::map<int, int> id2id_;
  // K_ x V_
  // phi_[i][j] probility of word[j] in topic[i]
  std::vector<std::vector<double> > phi_;
  // M_ x K_
  // theta_[i][j] probility of topic[j] in doc[i]
  std::vector<std::vector<double> > theta_;
  // V_ x K_
  // nword_[i][j] number of word[i] in topic[j]
  std::vector<std::vector<int> > nword_;
  // M_ x K_
  // ntopic_[i][j] number of topic[j] in doc[i]
  std::vector<std::vector<int> > ntopic_;
  // M_ x doc.size()
  // z_[i][j] topic index of word[j] in doc[i],
  std::vector<std::vector<int> > z_;
  // 1 x K_
  // nword_sum_[i] number of words in topic[i]
  std::vector<int> nword_sum_;
  // 1 x M_
  // doc_len_[i] the length of doc[i]
  std::vector<int> doc_len_;
  // M_ x doc.size()
  // doc_words_[i][j] word at position j in doc[i]
  std::vector<std::vector<int> > doc_words_;
};

}  // namespace lda

#endif  // NLTK_LDA_PUBLIC_MODEL_H_
