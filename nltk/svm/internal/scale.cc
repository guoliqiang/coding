// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-10 00:24:45
// File  : scale.cc
// Brief :

#include "nltk/svm/public/scale.h"

#include <map>
#include <vector>
#include "nltk/svm/public/solver.h"

namespace nltk {
namespace svm {

DEFINE_double(upper, 1.0, "");
DEFINE_double(lower, -1.0, "");

void MaxMinScale::GetFeatureMaxMin(
     std::vector<base::shared_ptr<ProblemNode> >& v) {
  for (int i = 0; i< v.size(); i++) {
    for (base::NormalSarray<double>::iterator j = v[i]->element.begin();
         j != v[i]->element.end(); j++) {
      if (!feature_max_min_.count(j->first)) {
        base::shared_ptr<std::pair<double, double> > foo(
              new std::pair<double, double>(TAU, INF));
        feature_max_min_.insert(std::make_pair(j->first, foo));
      }
      if (feature_max_min_[j->first]->first < j->second) {  // max
        feature_max_min_[j->first]->first = j->second;
      }
      if (feature_max_min_[j->first]->second > j->second) {  // min
        feature_max_min_[j->first]->second = j->second;
      }
    }
  }
}

void MaxMinScale::Do(
     std::vector<base::shared_ptr<ProblemNode> >* v) {
  feature_max_min_.clear();
  GetFeatureMaxMin(*v);
  for (int i = 0; i< v->size(); i++) {
    for (base::NormalSarray<double>::iterator j = (*v)[i]->element.begin();
         j != (*v)[i]->element.end(); j++) {
      j->second = Do(j->first, j->second);
    }
  }
}

double MaxMinScale::Do(int32_t index, double value, std::map<int32_t,
    base::shared_ptr<std::pair<double, double> > > * f /*=NULL*/) {
  if (f != NULL && !f->count(index)) return value;

  std::map<int32_t,
      base::shared_ptr<std::pair<double, double> > > * foo = f;
  if (foo == NULL) {
    foo = &feature_max_min_;
  }
  CHECK(foo->count(index));
  double max = (*foo)[index]->first;
  double min = (*foo)[index]->second;
  if (max == min) return value;  // ingnore single value feature
  if (value <= min) {  // may be < when f!= NULL
    return FLAGS_lower;
  }
  if (value >= max) {  // may be > when f!= NULL
    return FLAGS_upper;
  }
  return  (FLAGS_upper - FLAGS_lower) * ((value - min) / (max - min));
}

}  // namespace svm
}  // namepace nltk
