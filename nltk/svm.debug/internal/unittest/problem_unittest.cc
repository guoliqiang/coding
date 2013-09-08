// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-07 14:10:57
// File  : problem_unittst.cc
// Brief :
#include "../../public/problem.h"

int main(int argc, char** argv) {
  base::AtExitManager exit;
  std::string path = "input.txt";
  CHECK(nltk::svm::Problem::GetInstance().ReadFile(path));
  nltk::svm::Problem::GetInstance().LogContent();
  return 0;
}
