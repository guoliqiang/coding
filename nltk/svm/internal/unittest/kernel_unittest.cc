// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-08 18:40:35
// File  : kernel_unittest.cc
// Brief :

#include "nltk/svm/public/kernel.h"
#include "nltk/svm/public/problem.h"
#include "base/public/shared_ptr.h"

using namespace nltk::svm;

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::shared_ptr<nltk::svm::Parameter> para(new Parameter());
  Kernel::GetInstance().Set(para);
  std::string path = "input.txt";
  CHECK(nltk::svm::Problem::GetInstance().ReadFile(path));
  for (int i = 0; i < nltk::svm::Problem::GetInstance().node_.size(); i++) {
    for (int j = i; j < nltk::svm::Problem::GetInstance().node_.size(); j++) {
      LOG(INFO) << i << "-" << j << ":";
      LOG(INFO) << Kernel::GetInstance().Do(
                  *nltk::svm::Problem::GetInstance().node_[i].get(),
                  *nltk::svm::Problem::GetInstance().node_[j].get());
    }
  }
  return 0;
}
