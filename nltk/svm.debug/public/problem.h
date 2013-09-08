// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
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

namespace nltk {
namespace svm {

DECLARE_double(gamma);

class Problem {
 public:
  static Problem & GetInstance() {
    return * Singleton<Problem>::get();
  }

  Problem() {}
  
  bool ReadFile(std::string path) {
    std::string content;
    std::vector<std::string> lines;
    file::File::ReadFileToStringOrDie(path, &content);
    Tokenize(content, "\r\n", &lines);
    std::vector<std::string> parts;
    double max_feature_num = 0;
    for (int i = 0; i < lines.size(); i++) {
      ProblemNode * node = new ProblemNode();
      node->line_no = i;
      parts.clear();
      Tokenize(lines[i], "\t ", &parts);
      CHECK_GT(parts.size(), 1);
      CHECK(parts[0].find(':') == std::string::npos);
      node->lable = StringToInt(parts[0]);

      for (int j = 1; j < parts.size() ; j++) {
        std::vector<std::string> foo;
        SplitString(parts[j], ':', &foo);
        CHECK_EQ(foo.size(), 2);
        int32_t index = StringToInt(foo[0]);
        double value = StringToDouble(foo[1]);
        CHECK_GE(index, 0);
        node->element.insert(index, value);
        if (index > max_feature_num) max_feature_num = index;
      }
      base::shared_ptr<ProblemNode> foo;
      foo.reset(node);
      node_.push_back(foo);
    }
    sort(node_.begin(), node_.end(), Cmp);
    int32_t before = 0;
    for (int i = 0; i < node_.size(); i++) {
      if (i == 0 || node_[i]->lable != before) {
        start_.insert(std::make_pair(node_[i]->lable, i));
        before = node_[i]->lable;
        count_.insert(std::make_pair(node_[i]->lable, 1));
      } else {
        count_[before]++;
        before = node_[i]->lable;
      }
    }
    FLAGS_gamma = 1.0 / static_cast<double>(max_feature_num);
    para_.reset(new Parameter());
    LogContent();
    return true;
  }

 public:
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
  DISALLOW_COPY_AND_ASSIGN(Problem);
};
}  // namespace svm
}  // namespace nltk

#endif  //__PROBLEM_H_
