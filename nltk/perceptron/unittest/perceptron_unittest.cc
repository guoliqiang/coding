// Copyright 2013. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gamil.com)
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
