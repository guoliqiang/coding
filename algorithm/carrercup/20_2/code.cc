// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 16:41:57
// File  : code.cc
// Brief :
// 随机洗牌算法

/*
 *  Write a method to shuffle a deck of cards. It must be a perfect shuffle – in other words, each 52! 
 *  permutations of the deck has to be equally likely.
 *  Assume that you are given a random number generator which is perfect.
 *
 *  see more info in 20_3
 * */

#include <time.h>
#include "base/public/common_head.h"

namespace algorithm {

void Shuffle(std::vector<int> & v) {
  if (v.size() != 52) return;
  for (int i = 0; i < 52; i++) {
    int t = i + rand() % (52 - i);
    std::swap(v[i], v[t]);
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
