// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 03:58:31
// File  : predict.cc
// Brief :

#include "nltk/svm_rank/public/predict.h"
#include "nltk/svm/public/scale.h"

DECLARE_bool(scale);

namespace nltk {
namespace svm_rank {

using svm::ProblemNode;

void Predict::SvmPredict(const std::string & input,
                         const std::string & output) {
  std::string rs = "";
  std::string content;

  std::vector<std::string> lines;
  file::File::ReadFileToStringOrDie(input, &content);
  Tokenize(content, "\r\n", &lines);
  std::vector<std::string> parts;
  for (int i = 0; i < lines.size(); i++) {
    ProblemNode node;
    node.line_no = i;
    parts.clear();
    Tokenize(lines[i], "\t ", &parts);
    CHECK_GT(parts.size(), 0);

    for (int j = 0; j < parts.size() ; j++) {
      std::vector<std::string> foo;
      SplitString(parts[j], ':', &foo);
      CHECK_EQ(foo.size(), 2) << parts[j];
      int32_t index = StringToInt(foo[0]);
      double value = StringToDouble(foo[1]);
      CHECK_GE(index, 0);
      // scale
      if (FLAGS_scale) {
        value = svm::MaxMinScale::GetInstance()->Do(index, value,
                &(svm::Model::GetInstance().feature_max_min_));
      }
      node.element.insert(index, value);
    }
    LOG(INFO) << i << "/" << lines.size();
    int predict_lable = 0;
    std::map<std::pair<int32_t, int32_t>, double> class_score;
    if (svm::Model::GetInstance().para_->kernel_type_ == svm::LINEAR) {
      predict_lable = SvmPredictFast(node, &class_score);
    } else {
      predict_lable = svm::Predict::SvmPredict(node, &class_score);
    }
    CHECK_EQ(class_score.size(), 1)  << class_score.size();
    rs += DoubleToString(class_score.begin()->second) + "\n";
  }
  file::File::WriteStringToFile(rs, output);
}

}  // namespace svm_rank
}  // namespace nltk
