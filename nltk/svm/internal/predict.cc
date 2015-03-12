// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 13:16:12
// File  : predict.cc
// Brief :

#include "nltk/svm/public/predict.h"
#include "nltk/svm/public/scale.h"

namespace nltk {
namespace svm {

DECLARE_bool(scale);

void Predict::LoadModel(const std::string path) {
  Model::GetInstance().LoadModel(path);   
  Kernel::GetInstance().Set(Model::GetInstance().para_);
}

bool Predict::Free(const double alpha) {
  if (fabs(alpha) > 0) return false;
  return true;
}

void Predict::SvmPredict(const std::string input, const std::string output) {
  std::string rs = "";    
  std::string content;
  int32_t right_num = 0;

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

    int32_t input_lable = StringToInt(parts[0]);
    node.lable = input_lable;
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
    LOG(INFO) << i << "/" << lines.size();
    int predict_lable = 0;
    if (Model::GetInstance().para_->kernel_type_ == LINEAR) {
      predict_lable = SvmPredictFast(node);
    } else {
      predict_lable = SvmPredict(node);
    }
    rs += IntToString(predict_lable) + "\n";
    right_num += input_lable == predict_lable ? 1 : 0;
  }
  file::File::WriteStringToFile(rs, output);
  LOG(INFO) << "rignt num=" << right_num << " all num=" << lines.size()
            << " percent=" << double(right_num) / lines.size();
}

void Predict::KernelValue(const ProblemNode & input, std::vector<double> * rs) {
  rs->clear();
  for (int i = 0; i < Model::GetInstance().node_.size(); i++) {
    rs->push_back(Kernel::GetInstance().Do(*(Model::GetInstance().node_[i]),
                                           input));
  }
}

int32_t Predict::SvmPredictFast(ProblemNode & input) {
  std::map<int32_t, int32_t> votes;
  int32_t max_vote = 0;
  int32_t max_lable = 0;

  for (std::map<int32_t, int32_t>::reverse_iterator i =
       Model::GetInstance().start_.rbegin();
       i != Model::GetInstance().start_.rend(); i++) {
    std::map<int32_t, int32_t>::reverse_iterator tmp = i;
    tmp++;
    for (std::map<int32_t, int32_t>::reverse_iterator j = tmp;
         j != Model::GetInstance().start_.rend(); j++) {
      base::shared_ptr<ModelNode> foo =
            (*Model::GetInstance().model_[i->first].get())[j->first];
      CHECK(foo.get() != NULL);

      ProblemNode fake;
      fake.element = foo->w;
      double bar = Kernel::GetInstance().Do(fake, input);
      bar -= foo->b;
      VLOG(3) << "fast predict class=" << i->first << " V.S. class=" << j->first
              << " value=" << bar;

      if (bar > 0)  {
        if (votes.count(i->first)) votes[i->first]++;
        else votes[i->first] = 1;

        if (max_vote < votes[i->first]) {
          max_vote = votes[i->first];
          max_lable = i->first;
        }
      } else {
        if (votes.count(j->first)) votes[j->first]++;
        else votes[j->first] = 1;

        if (max_vote < votes[j->first]) {
          max_vote = votes[j->first];
          max_lable = j->first;
        }
      }
    }
  }
  VLOG(3)<< JoinKeysValues(&votes);
  return max_lable;
}

int32_t Predict::SvmPredict(ProblemNode & input) {
  std::map<int32_t, int32_t> votes;
  int32_t max_vote = 0;
  int32_t max_lable = 0;

  std::vector<double> kernel_value;
  KernelValue(input, &kernel_value);

  for (std::map<int32_t, int32_t>::reverse_iterator i =
       Model::GetInstance().start_.rbegin();
       i != Model::GetInstance().start_.rend(); i++) {
    std::map<int32_t, int32_t>::reverse_iterator tmp = i;
    tmp++;
    for (std::map<int32_t, int32_t>::reverse_iterator j = tmp;
         j != Model::GetInstance().start_.rend(); j++) {
      base::shared_ptr<ModelNode> foo =
            (*Model::GetInstance().model_[i->first].get())[j->first];
      CHECK(foo.get() != NULL);
      double bar = 0;
      CHECK(Model::GetInstance().count_.count(i->first));
      for (int k = 0; k < Model::GetInstance().count_[i->first]; k++) {
        double alpha;
        CHECK(foo->alpha.get(k, &alpha));
        if (Free(alpha)) {
          VLOG(5) << "not support SVM point " << " at:" << k;
          continue;
        }
        CHECK_LT(i->second + k, kernel_value.size());
        bar += alpha * kernel_value[i->second + k];
        // Kernel::GetInstance().
        //        Do(*(Model::GetInstance().node_[i->second + k].get()), input);
        VLOG(5) << "alpha=" << alpha << " kernel="
                << kernel_value[i->second + k] << "value=" << bar;
      }
      VLOG(5) << "after compute class=" << i->first << " value=" << bar;

      CHECK(Model::GetInstance().count_.count(j->first));
      for (int k = 0; k < Model::GetInstance().count_[j->first]; k++) {
        double alpha = 0;
        CHECK(foo->alpha.get(Model::GetInstance().count_[i->first] + k,
                             &alpha));
        if (Free(alpha)) {
          VLOG(5) << "not support SVM point " << " at:" << k;
          continue;
        }
        CHECK_LT(j->second + k, kernel_value.size());
        bar += alpha * kernel_value[j->second + k];
        // Kernel::GetInstance().
        //        Do(*(Model::GetInstance().node_[j->second + k].get()), input);
        VLOG(5) << "alpha=" << alpha << " kernel="
                << kernel_value[j->second + k] << " value=" << bar;
      }
      bar -= foo->b;
      VLOG(3) << "class=" << i->first << " V.S. class=" << j->first
              << " value=" << bar;

      if (bar > 0)  {
        if (votes.count(i->first)) votes[i->first]++;
        else votes[i->first] = 1;

        if (max_vote < votes[i->first]) {
          max_vote = votes[i->first];
          max_lable = i->first;
        }
      } else {
        if (votes.count(j->first)) votes[j->first]++;
        else votes[j->first] = 1;

        if (max_vote < votes[j->first]) {
          max_vote = votes[j->first];
          max_lable = j->first;
        }
      }
    }
  }
  VLOG(3)<< JoinKeysValues(&votes);
  return max_lable;
}
}  // namespace svm
}  // namespace nltk
