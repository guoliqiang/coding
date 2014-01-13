// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-11 22:31:46
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {
// list是链接存储的
void ListErase(std::list<int> & vec) {
  for (std::list<int>::iterator i = vec.begin(); i != vec.end();) {
    if (*i == 100) i = vec.erase(i);
    else i++;
  }
}

void ListErase2(std::list<int> & vec) {
  for (std::list<int>::iterator i = vec.begin(); i != vec.end();) {
    if (*i == 10) vec.erase(i++);
    else i++;
  }
}

// vector是连续存储的
void VectorErase(std::vector<int> & vec) {
  for (std::vector<int>::iterator i = vec.begin(); i != vec.end();) {
    if (*i == 100) i = vec.erase(i);
    else i++;
  }
}

void VectorErase2(std::vector<int> & vec) {
  for (std::vector<int>::iterator i = vec.begin(); i != vec.end();) {
    if (*i == 100) vec.erase(i);
    else i++;
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> vec;
  vec.push_back(2);
  vec.push_back(100);
  vec.push_back(40);
  VectorErase2(vec);
  LOG(INFO) << JoinVector(vec);
  return 0;
}
