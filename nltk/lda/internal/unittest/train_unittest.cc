// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-21 06:04:56
// File  : train_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "base/public/at_exit.h"
#include "nltk/lda/public/train.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);

  std::string data_path = "./data/newdocs.dat";
  std::string data_dir = "./data/";
  nltk::lda::Train p(data_path, data_dir);
  p.DoTrain();
  return 0;
}
