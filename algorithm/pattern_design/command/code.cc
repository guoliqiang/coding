// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-17 23:21:56
// File  : code.cc
// Brief :

// http://www.cnblogs.com/zhenyulu/articles/69858.html
// 回调模式
#include "base/public/common_ojhead.h"

namespace algorithm {

class Command {
 public:
  virtual void Execute() = 0;
  virtual ~Command() {}
};

template<typename T>
class SimpleCommand : public Command {
 public:
  typedef void (T::*Action)();
  
  SimpleCommand(T * receiver, Action action) : receiver_(receiver), action_(action) {}
  void Execute() {
    (receiver_->*action_)();
  }
 
 private:
  T * receiver_;
  Action action_;
};

class CompositeCommand : public Command {
 public:
  void Add(Command * c) {
    vec_.push_back(c);
  }

  bool Remove(Command * c) {
    for (std::vector<Command*>::iterator i = vec_.begin(); i != vec_.end(); i++) {
      if (*i == c) {
        vec_.erase(i);
        return true;
      }
    }
    return false;
  }

  void Execute() {
    for (int i = 0; i < vec_.size(); i++) {
      vec_[i]->Execute();
    }
  }
 private:
  std::vector<Command * > vec_;
};

class Receiver {
 public:
  void Action() {
    LOG(INFO) << "Hello Word!";
  }
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  Command * cmd = new SimpleCommand<Receiver>(new Receiver(), &Receiver::Action);
  cmd->Execute();
  return 0;
}
