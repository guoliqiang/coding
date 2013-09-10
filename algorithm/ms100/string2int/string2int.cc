// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 18:28:39
// File  : sting2int.cc
// Brief :
#include <string>
#include "./string2int.h"

using namespace algorithm;  // NOLINT

int main(int argc, char** argv) {
  std::string foo = "134";
  int num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }

  foo = "+00134";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
  foo = "-00134";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
  foo = "a+134";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
  foo = "+q134";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
  foo = "13ss4";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
  foo = "9999999999999999999999999999";
  num = 0;
  if (String2Int(foo, &num)) {
    LOG(INFO) << num;
  }
 
  return 0;
}
