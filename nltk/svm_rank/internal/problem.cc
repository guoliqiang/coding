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
  // from group 1 -> group 2 -> group 3
  int num = 0;
  for (std::map<int32_t, int32_t>::iterator i = start_.begin();
       i != start_.end(); i++) {
    int cnt = count_[i->first];
    for (int j = 0; j < cnt; j++) {
      for (int k = j + 1; k < cnt; k++) {
        if (node_[i->second + j]->score > node_[i->second + k]->score) {
          base::shared_ptr<ProblemNode> positive(new ProblemNode());
          positive->lable = 1;
          positive->line_no = num++;
          positive->element = node_[i->second + j]->element -
                              node_[i->second + k]->element;
          transfer_node_.push_back(positive);
          // In fact, model can ignore negative instance, but I find
          // if insert negative instance the result will be better.
          base::shared_ptr<ProblemNode> negative(new ProblemNode());
          negative->lable = -1;
          negative->line_no = num++;
          negative->element = node_[i->second + k]->element -
                              node_[i->second + j]->element;
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
  VLOG(5) << ToString();
  return true;
}

}  // namespace svm_rank
}  // namespace nltk
