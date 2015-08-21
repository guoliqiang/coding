// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 04:29:38
// File  : predict_unittest.cc
// Brief :

#include "nltk/svm_rank/public/predict.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  nltk::svm_rank::Predict::GetInstance().LoadModel("output");
  nltk::svm_rank::Predict::GetInstance().SvmPredict("pinput.txt",
      "pinput.txt.out");
  return 0;
}
