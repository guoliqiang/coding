// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-03 12:52:34
// File  : code.cc
// Brief :

/*
 * idea from stackoverflow
 * */

#include "base/public/common_head.h"

namespace algorithm {
// bug fix: must compare the size first.
bool IsRotate(std::string &s1, std::string & s2) {
  return s1.size() == s2.size() ? (s1 + s1).find(s2) != std::string::npos : false;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string s1 = "waterbottle";
  std::string s2 = "erbottlewat";
  LOG(INFO) << IsRotate(s1, s2);
  return 0;
}
