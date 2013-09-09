// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 00:24:45
// File  : scale.cc
// Brief :

#include "../public/scale.h"

#define INF HUGE_VAL
#define TAU 1e-12


namespace nltk {
namespace svm {

DEFINE_double(upper, 1.0, "");
DEFINE_double(lower, -1.0, "");

void Scale::GetFeatureMaxMin(
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

void Scale::Do(
     std::vector<base::shared_ptr<ProblemNode> >* v) {
  feature_max_min_.clear();
  GetFeatureMaxMin(*v);
  for (int i = 0; i< v->size(); i++) {
    for (base::NormalSarray<double>::iterator j = (*v)[i]->element.begin();
         j != (*v)[i]->element.end(); j++) {
      CHECK(feature_max_min_.count(j->first));
      double max = feature_max_min_[j->first]->first;
      double min = feature_max_min_[j->first]->second;
      if (max == min) continue;  // ingnore single value feature
      if (j->second == min) {
        j->second = FLAGS_lower;
        continue;
      }
      if (j->second == max) {
        j->second = FLAGS_upper;
        continue;
      }
      j->second = (FLAGS_upper - FLAGS_lower) *
                  ((j->second - min) / (max - min));
    }
  }
}

}  // namespace svm
}  // namepace nltk
