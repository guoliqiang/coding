// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-18 15:48:14
// File  : gloabal_unit.cc
// Brief :

#include "../public/global_init.h"

int main(int argc, char** argv) {
  util::GlobalInit::GlobalInit(&argc, &argv); 
  return 0;
}
