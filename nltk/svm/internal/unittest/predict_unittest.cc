// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 13:53:48
// File  : predict_unittest.cc
// Brief :

#include "nltk/svm/public/predict.h"
#include "base/public/logging.h"
#include "third_part/gperftools/wrapper/public/gperftools.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  profiler::Start("svm_predict.prof");
  nltk::svm::Predict::GetInstance().LoadModel("output");
  nltk::svm::Predict::GetInstance().SvmPredict("pinput.txt", "pinput.txt.out");
  profiler::Stop();
  return 0;
}
