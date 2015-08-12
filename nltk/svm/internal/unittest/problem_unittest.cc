// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2013-09-07 14:10:57
// File  : problem_unittst.cc
// Brief :

#include "nltk/svm/public/problem.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  std::string path = "input.txt";
  CHECK(nltk::svm::Problem::GetInstance().ReadFile(path));
  LOG(INFO) << nltk::svm::Problem::GetInstance().ToString();
  return 0;
}
