// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-10 10:00:50
// File  : p13.cc
// Brief :

#include <string>
#include "base/public/logging.h"

namespace algorithm {
char FindFirstSingle(std::string & str) {
  LOG(INFO) << -3%10;
  int * count = new int[0xFF];
  memset(count, 0, sizeof(int) * 0xFF);
  for (int i = 0; i < str.size(); i++) {
    count[static_cast<unsigned char>(str[i])]++;
  }
  char rs = ' ';
  for (int i = 0; i< str.size(); i++) {
    if (count[static_cast<unsigned char>(str[i])] == 1) {
      rs = str[i];
      break;
    }
  }
  delete [] count;
  return rs;
}
}  // namespace algorithm

int main(int argc, char** argv) {
  std::string str = "abaccdeff";
  LOG(INFO) << algorithm::FindFirstSingle(str);
  return 0;
}
