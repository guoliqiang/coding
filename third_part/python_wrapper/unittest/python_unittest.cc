// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-12 22:31:57
// File  : python_unittest.cc
// Brief :

#include "third_part/python_wrapper/public/python_wrapper.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

TEST(PythonWrapper, Func) {
  python_wrapper::PythonWrapper pw("./hello_world.py");
  std::vector<std::string> vec;
  vec.push_back("MM");
  vec.push_back("LL");
  vec.push_back("QQ");
  std::string rs;
  pw.CallFunc("Foo", vec, &rs);
  LOG(INFO) << rs;
}

TEST(PythonWrapper, Class) {
  python_wrapper::PythonWrapper pw("./hello_world.py");
  std::vector<std::string> cvec;
  cvec.push_back("NAME");
  cvec.push_back("VALUE");
  std::string rs;
  pw.CallClassFunc("Bar", "Dump", cvec, "INPUT1", "INPUT2", &rs);
  LOG(INFO) << rs;
}

TEST(PythonWrapper, SendEmail) {
  python_wrapper::PythonWrapper pw("./send_email.py");
  std::vector<std::string> vec;
  vec.push_back("guol@garena.com");
  vec.push_back("From PythonWrapper");
  pw.CallFunc("SendEmail", vec);
}
