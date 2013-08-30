// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-20 20:04:25
// File  : proto_unittest.cc
// Brief :

#include "third_part/protobuf/wrapper/public/proto_wrapper.h"
#include "third_part/protobuf/wrapper/proto/test.pb.h"

using namespace proto_wrapper;

int main(int argc, char** argv) {
  std::string str;
  // google::protobuf::Message message;
  ParseFromASCII(str, NULL);
  return 0;
}
