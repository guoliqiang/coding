// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-08 11:47:49
// File  : code.cc
// Brief :

/*
 * The Game of Master Mind is played as follows:
 *
 * The computer has four slots containing balls that are red (R ), yellow (Y), 
 * green (G) or blue (B). For example, the computer might have RGGB (e.g., 
 * Slot #1 is red, Slots #2 and #3 are green, Slot #4 is blue).
 *
 * You, the user, are trying to guess the solution. You might, for example, guess YRGB.
 * When you guess the correct color for the correct slot, you get a “hit”.
 * If you guess returns the number of hits and pseudo hits.
 * For example, the guess YRGB has 2 hits and one pseudo hit.
 *
 * For each guess, you are told the number of hits and pseudo-hits. 
 * Write a method that, given a guess and a solution, returns the number of hits and pseudo hits.
 *
 * */

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

/*
 * 思想很好，利用bitset快速判断
 * */

std::pair<int, int> HintFast(std::string & guss, std::string & solution) {
  int mask = 0;
  for (int i = 0; i < solution.size(); i++) {
    mask |= 1 << (solution[i] - 'A');
  }
  std::pair<int, int> rs = std::make_pair(0, 0);
  for (int i = 0; i < guss.size(); i++) {
    if (guss[i] == solution[i]) rs.first++;
    else if (mask & (1 << (guss[i] - 'A'))) rs.second++;
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
