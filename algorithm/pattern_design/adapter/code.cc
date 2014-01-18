// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 16:06:27
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// 某个现成的类接口不符合现在的使用方式
namespace algorithm {

template <typename T>
class GenerateAdaper {
 public:
  void Do() {
    ptr->wordk();  // adapt work to do
  }
 private:
  T * ptr;
};

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
