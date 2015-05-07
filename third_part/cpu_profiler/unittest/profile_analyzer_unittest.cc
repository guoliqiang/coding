// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-03-26 03:36:04
// File  : profile_analyzer_unittest.cc
// Brief :

#include "third_part/cpu_profiler/public/profile_analyzer.h"

void Foo() {
  ProfileAnalyzer pa;
  pa.Build();
  std::map<std::string, int> tmp;
  pa.SimpleList(&tmp);
  for (std::map<std::string, int>::iterator i = tmp.begin(); i != tmp.end();
       i++) {
    LOG(INFO) << i->first << " " << i->second;
  }
}

int main(int argc, char** argv) {
  Foo();
  return 0;
}
