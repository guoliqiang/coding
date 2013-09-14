// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-12 10:18:57
// File  : p36.h
// Brief :

#ifndef  __P36_H_
#define  __P36_H_

#include "../swap_string/swap_string.h"

namespace algorithm {

void StringEliminate(std::string * input, std::string & eliminator) {
  int * ch = new int [0xff];
  memset(ch, 0, 0xff * sizeof(char));
  for (int i = 0; i < eliminator.size(); i++) {
    ch[static_cast<int>(eliminator[i])] = 1;
  }
  char * b1 = &(*input)[0];
  char * b2 = &(*input)[0];
  char * e = &(*input)[input->size() - 1];
  while (b2 <= e) {
    if (b1 != b2); *b1 = *b2;
    b1++;
    b2++;
    while(ch[static_cast<int>(*b2)] && b2 <= e) b2++;
  }
  input->resize(b1 - &(*input)[0]);
}

}  // namespace algorithm

#endif  // __P36_H_
