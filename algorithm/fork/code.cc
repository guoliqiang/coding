// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-04-14 22:32:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

void Foo() {
  int count = 0;
  pid_t pid = fork();
  if (pid == 0) {
    count++;
    LOG(INFO) << getppid() << " " << getpid() << " " << count; 
  } else {
    count++;
    LOG(INFO) << getppid() << " " << getpid() << " " << count;
  }
}

void Bar() {
  int * count = new int(0);
  pid_t pid = fork();
  if (pid == 0) {
    (*count)++;
    LOG(INFO) << getppid() << " " << getpid() << " " << count << " " << *count; 
  } else {
    sleep(2);
    LOG(INFO) << getppid() << " " << getpid() << " " << count << " " << *count;
    (*count)+= 2;
    LOG(INFO) << getppid() << " " << getpid() << " " << count << " " << *count;
  }
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  //Foo();
  Bar();
  return 0;
}
