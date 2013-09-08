// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 12:13:55
// File  : predict.h
// Brief :

#ifndef  __PREDICT_H_
#define  __PREDICT_H_

#include "model.h"
#include "kernel.h"


namespace nltk {
namespace svm {
class Predict {
 public:
  static Predict & GetInstance() {
    return * Singleton<Predict>::get();
  }

  Predict() {}

  void LoadModel(const std::string & path) {
    Model::GetInstance().LoadModel(path);   
    Kernel::GetInstance().Set(Model::GetInstance().para_);
  }

  void SvmPredict(const std::string & input, const std::string &output) {
    std::string rs = "";    
    std::string content;
    std::vector<std::string> lines;
    file::File::ReadFileToStringOrDie(input, &content);
    Tokenize(content, "\r\n", &lines);
    std::vector<std::string> parts;
    for (int i = 0; i < lines.size(); i++) {
      ProblemNode node;
      parts.clear();
      Tokenize(lines[i], "\t ", &parts);
      CHECK_GT(parts.size(), 0);
      for (int j = 0; j < parts.size() ; j++) {
        std::vector<std::string> foo;
        SplitString(parts[j], ':', &foo);
        CHECK_EQ(foo.size(), 2);
        int32_t index = StringToInt(foo[0]);
        double value = StringToDouble(foo[1]);
        CHECK_GE(index, 0);
        node.element.insert(index, value);
      }
      rs += IntToString(SvmPredict(node)) + "\n";
    }
    file::File::WriteStringToFile(output, rs);
  }

  int32_t SvmPredict(const ProblemNode & input) {
    std::map<int32_t, int32_t> votes;
    int32_t max_vote = 0;
    int32_t max_lable = 0;
    for (std::map<int32_t, int32_t>::iterator i =  Model::GetInstance().start_.begin();
         i != Model::GetInstance().start_.end(); i++) {
      std::map<int32_t, int32_t>::iterator tmp = i;
      tmp++;
      for (std::map<int32_t, int32_t>::iterator j = tmp;
           j != Model::GetInstance().start_.end(); j++) {
        base::shared_ptr<ModelNode> foo = (*Model::GetInstance().model_[i->first].get())[j->first];
        CHECK(foo.get() != NULL);
        bool bar = 0;
        CHECK(Model::GetInstance().count_.count(i->first));
        for (int k = 0; k < Model::GetInstance().count_[i->first]; k++) {
          double alpha;
          CHECK(foo->alpha.get(k, &alpha));
          if (!Free(alpha, i->first)) continue;
          bar += alpha * Kernel::GetInstance().
                 Do(*Model::GetInstance().node_[i->first + k].get(), input);
        }
        CHECK(Model::GetInstance().count_.count(j->first));
        for (int k = 0; k < Model::GetInstance().count_[j->first]; k++) {
          double alpha = 0;
          CHECK(foo->alpha.get(Model::GetInstance().count_[i->first] + k, &alpha));
          if (!Free(alpha, j->first)) continue;
          bar += alpha * Kernel::GetInstance().
                 Do(*Model::GetInstance().node_[j->first + k].get(), input);
        }
        bar -= foo->b;
        if (bar > 0)  {
          if (votes.count(i->first)) {
            votes[i->first]++;
          } else {
            votes[i->first] = 1;
          }
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
    return max_lable;
  }

 private:
  bool Free(const double & alpha, int32_t lable) {
    if (alpha <= 0) return false;
    double foo = Model::GetInstance().para_->c_;
    if (Model::GetInstance().para_->weights_.count(lable)) {
      foo *= Model::GetInstance().para_->weights_[lable];
    }
    if (alpha >= foo) return false;
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Predict);
};
}  // namespace svm
}  // namespace nltk

#endif  //  __PREDICT_H_
