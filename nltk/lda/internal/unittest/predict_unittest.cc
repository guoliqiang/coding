// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-21 08:15:24
// File  : predict_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "base/public/at_exit.h"
#include "nltk/lda/public/predict.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);

  std::string data_path = "./data/newdocs.dat.p";
  std::string model_path = "./data/model.1";
  std::string data_dir = "./data/";
  nltk::lda::Predict p(data_path, model_path, data_dir);
  p.DoPredict();
  return 0;
}
