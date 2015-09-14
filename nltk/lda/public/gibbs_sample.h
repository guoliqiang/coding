// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 17:01:49
// File  : gibbs_sample.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_GIBBS_SAMPLE_H_
#define  NLTK_LDA_PUBLIC_GIBBS_SAMPLE_H_

#include <vector>
#include "base/public/logging.h"
#include "base/public/singleton.h"
#include "nltk/lda/public/model.h"

namespace lda {

class GibbsSample {
 public:
  GibbsSample & GetInstance() {
    return *Singleton<GibbsSample>::get();
  }
  GibbsSample() {}

  void InitTrain(TrainModel * model);
  void IterTrain(TrainModel * model, int steps);

  void InitPredict(const TrainModel & tmodel, PredictModel * pmodel);
  void IterPredict(const TrainModel & tmodel, PredictModel * pmodel, int steps);

 private:
  int Sampling(TrainModel * model, int m, int n);
  int Sampling(const TrainModel &tmodel, PredictModel * pmodel, int m, int n);

  void ComputePhi(TrainModel * model);
  void ComputeTheta(TrainModel * model);

  void ComputePhi(const TrainModel & tmodel, PredictModel * pmodel);
  void ComputeTheta(const TrainModel & tmodel, PredictModel * pmodel);

 private:
  DISALLOW_COPY_AND_ASSIGN(GibbsSample);
};

}  // namespace lad


#endif  // NLTK_LDA_PUBLIC_GIBBS_SAMPLE_H_
