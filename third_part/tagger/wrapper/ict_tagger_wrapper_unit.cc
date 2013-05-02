// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-03-27 11:20:25
// File  : IctTaggerWrapper_unittest.cc
// Brief :
#include "ict_tagger_wrapper.h"
#include <string>
int main(int argc, char** argv) {
  base::ParseCommandLineFlags(&argc, &argv, true);
  std::string pos_str;
  ict_wrapper::IctTaggerWrapper::Tagger("北京大学位于北京市。", &pos_str);
  LOG(INFO) << pos_str;
  return 0;
}
