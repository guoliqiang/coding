// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-08 00:15:29
// File  : problem.cc
// Brief :

#include "nltk/svm/public/problem.h"
#include<string>
#include<algorithm>
#include "nltk/svm/public/scale.h"

namespace nltk {
namespace svm {

DEFINE_bool(scale, false, "");

bool Problem::ReadFile(const std::string & path) {
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
      if (foo[0] == "score") {
        node->score = StringToDouble(foo[1]);
        continue;
      }
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
  // sort by lable
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
  if (FLAGS_scale) {
    VLOG(2) << "scaling begin...";
    MaxMinScale::GetInstance()->Do(&node_);
    VLOG(2) << "scaling end";
  }
  VLOG(5) << ToString();
  return true;
}
}  // namespace svm
}  // namespace nltk
