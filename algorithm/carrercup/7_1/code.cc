// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-05 13:49:26
// File  : code.cc
// Brief :

#include "base/public/common_head.h"

namespace algorithm {

enum Shape {CLUBS = 0, SPADES = 1, HEARTS = 2, DIAMONDS = 3};

class Card {
 public:
  Card(int v, Shape s) : val_(v), shape_(s) {}
  int GetVal() {
    return val_;
  }
  Shape GetShape() {
    return shape_;
  }
 private:
  int val_;
  Shape shape_;
};

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
