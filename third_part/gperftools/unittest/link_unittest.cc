// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-06-15 19:50:40
// File  : link_unittest.cc
// Brief :

void Foo();
void Bar();

// use the first one in the link sequence
int main(int argc, char** argv) {
  Foo();
  Bar();
  return 0;
}
