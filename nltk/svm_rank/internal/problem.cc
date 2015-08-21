// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 02:02:48
// File  : problem.cc
// Brief :

#include "nltk/svm_rank/public/problem.h"
#include <string>
#include <algorithm>

namespace nltk {
namespace svm_rank {

using svm::ProblemNode;

bool Problem::ReadFile(std::string path) {
  CHECK(svm::Problem::ReadFile(path));
  for (std::map<int32_t, int32_t>::iterator i = start_.begin();
       i != start_.end(); i++) {
    for (int i_index = 0; i_index < count_[i->first]; i_index++) {
      std::map<int32_t, int32_t>::iterator next = i;
      for (std::map<int32_t, int32_t>::iterator j = ++next; j != start_.end();
           j++) {
        for (int j_index = 0; j_index < count_[j->first]; j_index++) {
          base::shared_ptr<ProblemNode> positive(new ProblemNode());
          base::shared_ptr<ProblemNode> negative(new ProblemNode());
          positive->lable = 1;
          negative->lable = -1;
          positive->element = node_[i->second + i_index]->element -
                              node_[j->second + j_index]->element;
          negative->element = node_[j->second + j_index]->element -
                              node_[i->second + i_index]->element;
          transfer_node_.push_back(positive);
          transfer_node_.push_back(negative);
        }
      }
    }
  }
  sort(transfer_node_.begin(), transfer_node_.end(), Cmp);
  int32_t before = 0;
  for (int i = 0; i < transfer_node_.size(); i++) {
    if (i == 0 || before != transfer_node_[i]->lable) {
      transfer_start_.insert(std::make_pair(transfer_node_[i]->lable, i));
      before = transfer_node_[i]->lable;
      transfer_count_.insert(std::make_pair(transfer_node_[i]->lable, 1));
    } else {
      transfer_count_[before]++;
      before = transfer_node_[i]->lable;
    }
  }
  VLOG(5) << LocalToString();
  return true;
}

}  // namespace svm_rank
}  // namespace nltk
