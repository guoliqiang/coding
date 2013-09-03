// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-02 22:45:35
// File  : perceptron.h
// Brief :

#ifndef  __PERCEPTRON_H_
#define  __PERCEPTRON_H_

#include <vector>
#include <list>
#include <string>
#include "base/public/logging.h"
#include "third_part/gtl/public/stl_util-inl.h"

namespace nltk {

class Perceptron {
 public:
  bool ReadData(const std::string & path); // format 1/-1:x 
  bool Train();
  int Predict(const std::vector<double> & x);
  
  Perceptron() {}
  ~Perceptron() {
    Clear();
  }
 
 public:
  inline std::vector<double> & get_w() {
    return w_;
  }

  void Clear() {
   gtl::STLDeleteElements(&x_);
   y_.clear();
   w_.clear();
  }
 private:
  std::vector<std::vector<double> *> x_;
  std::vector<int> y_;
  std::vector<double> w_;  // w[0] == b (w.x + b = y)
 
 private:
  DISALLOW_COPY_AND_ASSIGN(Perceptron);
};
}  // namespace nltk
#endif  //__PERCEPTRON_H_
