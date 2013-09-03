// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-02 23:00:07
// File  : perceptron.cc
// Brief :

// TODO(guoliqiang) use sparse_array.

#include "../public/perceptron.h"
#include "base/public/string_util.h"
#include "file/public/file.h"

// step of learning
DEFINE_double(c, 0.01, "");


namespace nltk {

bool Perceptron::ReadData(const std::string & path) {
  Clear();
  std::string content;
  std::vector<std::string> lines;
  file::File::ReadFileToStringOrDie(path, &content);
  Tokenize(content, "\r\n", &lines);
  std::vector<std::string> parts;
  std::vector<std::string> foo;
  for (int i = 0; i < lines.size(); i++) {
    parts.clear();
    SplitString(lines[i], ':', &parts);
    CHECK_EQ(parts.size(), 2) << lines[i];
    y_.push_back(StringToInt(parts[0]));
    foo.clear();
    Tokenize(parts[1], "\t ", &foo);

    if (x_.size() > 0) {
      CHECK_EQ(x_[0]->size(), foo.size() + 1);
    }
    std::vector<double> * bar = new std::vector<double>();
    bar->push_back(1);  // x0 = 1 for b
    for (int j = 0; j < foo.size() ; j++) {
      bar->push_back(StringToDouble(foo[j]));
    }
    x_.push_back(bar);
  }
  CHECK_GT(x_.size(), 0);
  w_.assign(x_[0]->size(), 0);
  return true;
}

bool Perceptron::Train() {
  bool flag = true;
  int count = 0;
  while (flag) {
    flag = false;
    if (++count > x_.size() * 1000) {
      LOG(ERROR) << "maybe can not to convergence";
      break;
    }
    for (int i = 0; i < x_.size(); i++) {
      double foo = 0;
      for (int j = 0 ; j < x_[0]->size(); j++) {
        foo += w_[j] * (*x_[i])[j];
      }
      if (foo * y_[i] <= 0) {
        // find a wrong point.
        flag = true;
        for (int j = 0; j < w_.size(); j++) {
          double bar = w_[j];
          w_[j] += FLAGS_c * y_[i] * (*x_[i])[j];
          VLOG(5) << "change w[" << j <<"] from "
                  << bar << " to " << w_[j];
        }
      }  // if
    }  // for
  }  // while
  return !flag;
}


int Perceptron::Predict(const std::vector<double> & x) {
  CHECK_EQ(x.size() + 1, w_.size());
  double foo = w_[0];  // w_[0] is b. (w.x + b = y)

  for (int i = 0; i < x.size(); i++) {
    foo += x[i] * w_[i + 1];
  }
  if (foo > 0) return 1;
  return -1;
}

}  // namespace nltk
