// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-08-20 20:04:25
// File  : proto_unittest.cc
// Brief :

#include "third_part/protobuf/wrapper/public/proto_wrapper.h"
#include "third_part/protobuf/wrapper/proto/test.pb.h"
#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

TEST(ProtoWrapper, GetNameValue) {
  Person person;
  person.set_id(1);
  person.set_name("seven");
  person.set_email("guoliqiang2006@gmail.com");
  std::map<std::string, std::string> vec;
  proto_wrapper::GetNameValuePair(person, &vec);
  EXPECT_TRUE(vec.count("id"));
  EXPECT_TRUE(vec.count("name"));
  EXPECT_TRUE(vec.count("email"));

  EXPECT_EQ(vec["id"], "1");
  EXPECT_EQ(vec["name"], "seven");
  EXPECT_EQ(vec["email"], "guoliqiang2006@gmail.com");
}

TEST(ProtoWrapper, SetNameValue) {
  std::map<std::string, std::string> vec;
  Person person;
  vec["id"] = "1";
  vec["name"] = "seven";
  vec["email"] = "guoliqiang2006@gmail.com";
  proto_wrapper::SetNameValuePair(vec, &person);
  EXPECT_EQ(person.id(), 1);
  EXPECT_EQ(person.name(), "seven");
  EXPECT_EQ(person.email(), "guoliqiang2006@gmail.com");
}

TEST(ProtoWrapper, SetNameValueCreate) {
  std::map<std::string, std::string> vec;
  vec["id"] = "1";
  vec["name"] = "seven";
  vec["email"] = "guoliqiang2006@gmail.com";
  base::shared_ptr<google::protobuf::Message> ptr =
      proto_wrapper::SetNameValuePair(vec,*(Person::descriptor()));
  proto_wrapper::SetNameValuePair(vec, ptr.get());
  Person * person = (Person *)(ptr.get());
  EXPECT_EQ(person->id(), 1);
  EXPECT_EQ(person->name(), "seven");
  EXPECT_EQ(person->email(), "guoliqiang2006@gmail.com");
}

TEST(ProtoWrapper, Creater) {
  std::map<std::string, std::string> vec;
  proto_wrapper::PbRuntimeCreater creater;
  creater.AddProtoFile("../proto/test.proto");
  base::shared_ptr<google::protobuf::Message> foo =
      creater.CreateMessageByName("Person");
  vec["id"] = "1";
  vec["name"] = "guoliqiang";
  proto_wrapper::SetNameValuePair(vec, foo.get());
  vec.clear();
  proto_wrapper::GetNameValuePair(*(foo.get()), &vec);
  EXPECT_EQ(vec["id"], "1");
  EXPECT_EQ(vec["name"], "guoliqiang");
}
