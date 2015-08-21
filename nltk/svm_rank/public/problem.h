// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 01:54:52
// File  : problem.h
// Brief :

#ifndef  NLTK_SVM_RANK_PUBLIC_PROBLEM_H_
#define  NLTK_SVM_RANK_PUBLIC_PROBLEM_H_

#include <map>
#include <vector>
#include "nltk/svm/public/problem.h"

namespace nltk {
namespace svm_rank {

class Problem : public svm::Problem {
 public:
  static Problem & GetInstance() {
    return *Singleton<Problem>::get();
  }

  Problem() {}
  virtual bool ReadFile(std::string path);
  std::string LocalToString() const {
    std::string rs = "\n";
    rs += "transfer_start:" + JoinKeysValues(&transfer_start_) + "\n";
    rs += "transfer_count:" + JoinKeysValues(&transfer_count_) + "\n";
    for (int i = 0; i < transfer_node_.size(); i++) {
      rs += transfer_node_[i]->ToString();
      if (i != transfer_node_.size() - 1) rs += "\n";
    }
    return rs;
  }

 public:
  std::vector<base::shared_ptr<svm::ProblemNode> > transfer_node_;
  std::map<int32_t, int32_t> transfer_start_;
  std::map<int32_t, int32_t> transfer_count_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Problem);
};

}  // namespace svm_rank
}  // namespace nltk

#endif  // NLTK_SVM_RANK_PUBLIC_PROBLEM_H_
