// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-18 18:12:33
// File  : code.cc
// Brief :

// 当一个对象引起内部状态改变后，需要改变其行为
// 通过改变对象的成员变量实现

#include "base/public/common_ojhead.h"

namespace algorithm {
class TCPConnect;

class TCPState {
 public:
  virtual ~TCPState() {}
  virtual void Close(TCPConnect * ptr) {};
  virtual void Open(TCPConnect * ptr) {};
  virtual void Work (TCPConnect * ptr) {};
};


class TCPClose : public TCPState {
  public:
   void Open(TCPConnect * ptr);
};

class TCPListen : public TCPState {
 public:
  void Work(TCPConnect * ptr);
  void Close(TCPConnect * ptr);
};

class TCPEstablish : public TCPState {
 public:
  void Work(TCPConnect * ptr);
  void Close(TCPConnect * ptr);
};

class TCPConnect {
 public:
  TCPConnect(TCPState * s) : state(s) {}
  virtual ~TCPConnect() {}

  void Open() {
    state->Open(this);
  }

  void Close() {
    state->Close(this);
  }

  void Work() {
    state->Work(this);
  }

  void ChangeState(TCPState * s) {
    delete state;
    state = s;
  }
 private:
  TCPState * state;
};

void TCPClose::Open(TCPConnect * ptr) {
  ptr->ChangeState(new TCPEstablish());
}

void TCPEstablish::Work(TCPConnect * ptr) {
  ptr->ChangeState(new TCPListen());
}

void TCPEstablish::Close(TCPConnect * ptr) {
  ptr->ChangeState(new TCPClose());
}

void TCPListen::Work(TCPConnect * ptr) {
  LOG(INFO) << "listen";
}
  
void TCPListen::Close(TCPConnect * ptr) {
  ptr->ChangeState(new TCPClose());
}

}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  FROMFILE;
  return 0;
}
