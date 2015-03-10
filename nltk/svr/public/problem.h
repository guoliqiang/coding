// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-07 23:24:32
// File  : problem.h
// Brief :

#ifndef  NLTK_SVR_PUBLIC_PROBLEM_H_
#define  NLTK_SVR_PUBLIC_PROBLEM_H_

#include "nltk/svr/public/kernel.h"
#include <vector>
#include <map>
#include "base/public/normal_sparse_array.h"
#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include "file/public/file.h"
#include "base/public/string_util.h"

#define INF HUGE_VAL
#define TAU 1e-12

namespace nltk {
namespace svr {

DECLARE_double(gamma);

class Problem {
 public:
  static Problem & GetInstance() {
    return * Singleton<Problem>::get();
  }

  Problem() {}
  bool ReadFile(std::string path);
    
  std::string ToString() {
    std::string rs = "\n";
    rs += "start:" + JoinKeysValues(&start_) + "\n";
    rs += "count:" + JoinKeysValues(&count_) + "\n";
    for (int i = 0; i < node_.size(); i++) {
      rs += node_[i]->ToString();
      if (i != node_.size() - 1) rs += "\n";
    }
    return rs;
  }

 public:
  base::shared_ptr<Parameter> para_;
  std::vector<base::shared_ptr<ProblemNode> > node_;
  std::map<int32_t, int32_t> start_;
  std::map<int32_t, int32_t> count_;
 
 private:
  DISALLOW_COPY_AND_ASSIGN(Problem);
};
}  // namespace svr
}  // namespace nltk

#endif  // NLTK_SVR_PUBLIC_PROBLEM_H_
