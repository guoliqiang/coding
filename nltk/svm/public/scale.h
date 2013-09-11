// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 00:17:48
// File  : scale.h
// Brief :

#ifndef  __SCALE_H_
#define  __SCALE_H_

#include "problem.h"
#include "kernel.h"
#include "base/public/shared_ptr.h"

namespace nltk {
namespace svm {

class Scale {
 public:
  static Scale & GetInstance() {
    return * Singleton<Scale>::get();
  }

  Scale() {} 
  void Do(std::vector<base::shared_ptr<ProblemNode> > * v);
 private:
  void GetFeatureMaxMin(std::vector<base::shared_ptr<ProblemNode> >& v);
 private:
 std::map<int32_t,
          base::shared_ptr<std::pair<double, double> > > feature_max_min_;
 private:
  DISALLOW_COPY_AND_ASSIGN(Scale);
};

}  // svm
}  // namespace nltk

#endif  // __SCALE_H_
