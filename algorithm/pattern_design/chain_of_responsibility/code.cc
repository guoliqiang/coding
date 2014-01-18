// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 21:28:37
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

class HandlerBase {
 public:
  HandlerBase(HandlerBase * next = NULL, int flag = 0) : next_(next), flag_(flag) {}
  virtual bool HasHandle(int t) {
    return t == flag_;
  }
  virtual bool Handle(std::string & str, int t) {
    if (next_ != NULL) return next_->Handle(str, t);
    return true;
  }

 protected:
  HandlerBase * next_;
  int flag_;
};

class MysqlHandler : public HandlerBase {
 public:
  MysqlHandler(HandlerBase * next = NULL, int flag = 0) : HandlerBase(next, flag) {}
  
  bool Handle(std::string & str, int t) {
    if (HasHandle(t)) LOG(INFO) << str << " Mysql";
    return HandlerBase::Handle(str, t);
  }
};

class HDFSHandler : public HandlerBase {
 public:
  HDFSHandler(HandlerBase * next = NULL, int flag = 0) : HandlerBase(next, flag) {};
  
  bool Handle(std::string & str, int t) {
    if (HasHandle(t)) LOG(INFO) << str << " HDFS";
    return HandlerBase::Handle(str, t);
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  HandlerBase * mysql = new MysqlHandler(NULL, 1);
  HandlerBase * hdfs = new HDFSHandler(mysql, 2);
  std::string str = "hello world!";
  HandlerBase * begin = new HandlerBase(hdfs, -1);
  begin->Handle(str, 1);
  return 0;
}
