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
  vec["name"] = "guoliqiang";
  base::shared_ptr<google::protobuf::Message> ptr =
      proto_wrapper::SetNameValuePair(vec,*(Person::descriptor()));
  LOG(INFO) << ptr->DebugString();
  return 0;
}
