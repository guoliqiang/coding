// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 11:47:49
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

std::pair<int, int> Hint(std::string & guss, std::string & solution) {
  std::pair<int, int> rs(0, 0);
  for (int i = 0; i < 4; i++) {
    if(guss[i] == solution[i]) {
      rs.first++;
    } else {
      for (int j = 1; j < 4; j++) {
        if (guss[(i + j) % 4] != solution[(i + j) %4] && guss[i] == solution[(i + j) % 4]) {
          rs.second++;
          break;
        }
      }
    }
  }
  return rs;
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::string solution = "RGGB";
  std::string guss = "YRGB";
  std::pair<int, int> rs = Hint(guss, solution);
  LOG(INFO) << rs.first << " " << rs.second;
  return 0;
}
