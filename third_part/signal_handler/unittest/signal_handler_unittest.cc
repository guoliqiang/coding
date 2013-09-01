// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#include <string>
#include "../public/signal_handler.h"
#include "base/public/logging.h"

class Test {
 public:
  void Add(int i) { num += i; }
  int  Get() { return num; }
 private:
  int num;
};

static int stop = 0;
void TestStop(int i) {
  LOG(INFO) << "Signal[" << i << "]";
  ++stop;
}

static int stop2 = 0;
void TestStop2(int i) {
  LOG(INFO) << "Signal[" << i << "]";
  ++stop2;
}

using signal_handler::SignalHandler;
int main(int argc, char* argv[]) {
  pid_t temp = getpid();
  Test test;
  base::Callback1<int>* func_handler1 =
      base::NewPermanentCallback(TestStop);
  base::Callback1<int>* func_handler2 =
      base::NewPermanentCallback(TestStop2);
  base::Callback1<int>* class_handler =
      base::NewPermanentCallback(&test, &Test::Add);

  SignalHandler::SetHandlerForDefaultSignals(func_handler1);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForDefaultSignals(class_handler);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(2, func_handler2);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(3, func_handler2);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(15, func_handler2);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(2, class_handler);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(3, class_handler);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::SetHandlerForSignal(15, class_handler);
  kill(temp, 2);
  LOG(INFO) << "Send signal 2";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 3);
  LOG(INFO) << "Send signal 3";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  kill(temp, 15);
  LOG(INFO) << "Send signal 15";
  LOG(INFO) << "Test:num:" << test.Get();
  LOG(INFO) << "stop: " << stop;
  LOG(INFO) << "stop2: " << stop2;

  LOG(INFO) << "======================";

  SignalHandler::RestoreSystemHandler();
  delete func_handler1;
  delete func_handler2;
  delete class_handler;

  kill(temp, 15);
  LOG(INFO) << "something wrong";
  return 0;
}
