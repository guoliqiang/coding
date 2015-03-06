// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 11:18:01
// File  : train_unittest.cc
// Brief :

#include "nltk/svm/public/train.h"
#include "base/public/logging.h"
#include "third_part/gperftools/wrapper/public/gperftools.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  profiler::Start("svm_train.prof");
  std::string path = "input.txt";
  nltk::svm::Train::GetInstance().ReadFile(path);
  nltk::svm::Train::GetInstance().SvmTrain();
  std::string out = "output";
  nltk::svm::Train::GetInstance().WriteModel(out);
  profiler::Stop();
  return 0;
}
