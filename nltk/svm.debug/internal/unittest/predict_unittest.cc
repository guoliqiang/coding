// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 13:53:48
// File  : predict_unittest.cc
// Brief :
#include "../../public/predict.h"

int main(int argc, char** argv) {
  nltk::svm::Predict::GetInstance().SvmPredict("input.txt", "output.txt");
  return 0;
}
