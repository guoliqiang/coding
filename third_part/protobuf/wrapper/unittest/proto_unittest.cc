// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-08-20 20:04:25
// File  : proto_unittest.cc
// Brief :

#include "third_part/protobuf/wrapper/public/proto_wrapper.h"
#include "third_part/protobuf/wrapper/proto/test.pb.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  Person person;
  person.set_id(1);
  person.set_name("seven");
  person.set_email("guoliqiang2006@gmail.com");
  std::map<std::string, std::string> vec;
  proto_wrapper::GetNameValuePair(person, &vec);
  LOG(INFO) << person.DebugString();
  Person person2;
  vec["name"] = "guoliqiang";
  proto_wrapper::SetNameValuePair(vec, &person2);
  LOG(INFO) << person2.DebugString();
  return 0;
}
