// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-10 00:17:48
// File  : scale.h
// Brief :

#ifndef  __SCALE_H_
#define  __SCALE_H_

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

}  // svm
}  // namespace nltk

#endif  // __SCALE_H_
