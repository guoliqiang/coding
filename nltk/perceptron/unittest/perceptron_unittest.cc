// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-03 07:51:43
// File  : perceptron_unittest.cc
// Brief :

#include "../public/perceptron.h"
#include "base/public/string_util.h"

using namespace nltk;   // NOLINT

int main(int argc, char** argv) {
  Perceptron p;
  p.ReadData("input.txt");
  CHECK(p.Train());
  std::vector<double> & w = p.get_w();
  LOG(INFO) << JoinVector(w);
  return 0;
}
