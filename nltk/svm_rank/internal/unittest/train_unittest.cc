// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-21 04:17:38
// File  : train_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "nltk/svm_rank/public/train.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  std::string path = "input.txt";
  nltk::svm_rank::Train::GetInstance().ReadFile(path);
  nltk::svm_rank::Train::GetInstance().SvmTrain();
  std::string out = "output";
  nltk::svm_rank::Train::GetInstance().WriteModel(out);
  return 0;
}
