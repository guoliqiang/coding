// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-07 23:24:32
// File  : problem.h
// Brief :

#ifndef  __PROBLEM_H_
#define  __PROBLEM_H_

#include "base/public/normal_sparse_array.h"
#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include "kernel.h"
#include "file/public/file.h"
#include <vector>
#include <map>
#include "base/public/string_util.h"

#define INF HUGE_VAL
#define TAU 1e-12

namespace nltk {
namespace svm {

DECLARE_double(gamma);

class Problem {
 public:
  static Problem & GetInstance() {
    return * Singleton<Problem>::get();
  }

  Problem() {}
  bool ReadFile(std::string path);
    
  void LogContent(int l = 0) {
    para_->LogContent();
    VLOG(l) << "start:" << JoinKeysValues(&start_);
    VLOG(l) << "count:" << JoinKeysValues(&count_);
    for (int i = 0; i< node_.size(); i++) {
      node_[i]->LogContent();
    }
  }
  
 private:
  static bool Cmp(const base::shared_ptr<ProblemNode> & a,
                  const base::shared_ptr<ProblemNode> & b) {
    return a->lable > b->lable;
  }

 public:
  base::shared_ptr<Parameter> para_;
  std::vector<base::shared_ptr<ProblemNode> > node_;
  std::map<int32_t, int32_t> start_;
  std::map<int32_t, int32_t> count_;
 
 private:
  DISALLOW_COPY_AND_ASSIGN(Problem);
};
}  // namespace svm
}  // namespace nltk

#endif  //__PROBLEM_H_
