// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 13:16:12
// File  : predict.cc
// Brief :

#include "nltk/svr/public/predict.h"
#include "nltk/svr/public/scale.h"

namespace nltk {
namespace svr {

DECLARE_bool(scale);

void Predict::LoadModel(const std::string path) {
  Model::GetInstance().LoadModel(path);   
  Kernel::GetInstance().Set(Model::GetInstance().para_);
}

bool Predict::Free(const double alpha) {
  if (fabs(alpha) > 0) return false;
  return true;
}

void Predict::SvrPredict(const std::string input, const std::string output) {
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

    double input_value = StringToDouble(parts[0]);
    node.value = input_value;
    for (int j = 1; j < parts.size() ; j++) {
      std::vector<std::string> foo;
      SplitString(parts[j], ':', &foo);
      CHECK_EQ(foo.size(), 2) << parts[j];
      int32_t index = StringToInt(foo[0]);
      double value = StringToDouble(foo[1]);
      CHECK_GE(index, 0);
      // scale
      if (FLAGS_scale) {
        value = MaxMinScale::GetInstance()->Do(index, value,
                &(Model::GetInstance().feature_max_min_));
      }
      node.element.insert(index, value);
    }
    double predict_value = SvrPredict(node);
    rs += DoubleToString(predict_value) + "\n";
    LOG(INFO) << i << "/" << lines.size() << " input value=" << input_value
              << " real_value=" << predict_value;
  }
  file::File::WriteStringToFile(rs, output);
}

void Predict::KernelValue(const ProblemNode & input, std::vector<double> * rs) {
  rs->clear();
  for (int i = 0; i < Model::GetInstance().node_.size(); i++) {
    rs->push_back(Kernel::GetInstance().Do(*(Model::GetInstance().node_[i]),
                                           input));
  }
}

double Predict::SvrPredict(ProblemNode & input) {
  std::vector<double> kernel_value;
  KernelValue(input, &kernel_value);
  double predict_value = 0;
  for (int i = 0; i < Model::GetInstance().node_.size(); i++) {
    double alpha = 0;
    CHECK(Model::GetInstance().model_->alpha.get(i, &alpha));
    predict_value += alpha * kernel_value[i];
  }
  predict_value -= Model::GetInstance().model_->b;
  return predict_value;
}
}  // namespace svr
}  // namespace nltk
