// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 17:00:54
// File  : code.cc
// Brief :
/*
 * Write code to reverse a C-Style String. (C-String means that “abcd” is represented as
 * five characters, including the null character.)
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

// this function is nice.
// if need swap, please use this version

template <typename T>
void swap(T & a, T & b) {
  a = a^b;
  b = a^b;
  a = a^b;
}


void Reverse(char * ptr) {
  int len = strlen(ptr);
  /*
   * int len = 0;
   * char * p = ptr;
   * while (*p != '\0') len++;
   *
   * */
  for (int i = 0; i < len / 2; i++) {
    // std::swap(ptr[i], ptr[len - 1 - i]);
    std::swap(ptr[i], ptr[len - 1 - i]);
  }
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {
  char ptr[] = "abcd";
  Reverse(ptr);
  LOG(INFO) << ptr;
  return 0;
}
