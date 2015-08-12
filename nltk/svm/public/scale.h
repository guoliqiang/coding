// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-10 00:17:48
// File  : scale.h
// Brief :

#ifndef  NLTK_SVM_PUBLIC_SCALE_H_
#define  NLTK_SVM_PUBLIC_SCALE_H_

#include "nltk/svm/public/problem.h"
#include "nltk/svm/public/kernel.h"
#include "base/public/shared_ptr.h"

namespace nltk {
namespace svm {

DECLARE_double(lower);
DECLARE_double(upper);

class Scale {
 public:
  virtual void Do(std::vector<base::shared_ptr<ProblemNode> > * v) = 0;
  virtual ~Scale(){}
};

class MaxMinScale : public Scale {
 public:
  static MaxMinScale * GetInstance() {
    return Singleton<MaxMinScale>::get();
  }

  MaxMinScale() {} 
  void Do(std::vector<base::shared_ptr<ProblemNode> > * v);
  double Do(int32_t index, double value,
            std::map<int32_t,
            base::shared_ptr<std::pair<double, double> > > * f = NULL);

 private:
  void GetFeatureMaxMin(std::vector<base::shared_ptr<ProblemNode> >& v);
 
 public:
  std::map<int32_t,
           base::shared_ptr<std::pair<double, double> > > feature_max_min_;
 
 private:
  DISALLOW_COPY_AND_ASSIGN(MaxMinScale);
};

}  // namespace svm
}  // namespace nltk

#endif  // NLTK_SVM_PUBLIC_SCALE_H_
