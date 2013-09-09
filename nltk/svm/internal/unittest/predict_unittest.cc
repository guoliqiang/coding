// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 13:53:48
// File  : predict_unittest.cc
// Brief :
#include "../../public/predict.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  nltk::svm::Predict::GetInstance().LoadModel("output");
  nltk::svm::Predict::GetInstance().SvmPredict("pinput.txt", "predict");
  return 0;
}
