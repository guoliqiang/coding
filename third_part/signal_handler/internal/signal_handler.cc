// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#include "../public/signal_handler.h"

#include <string>

#include "base/public/logging.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT

namespace signal_handler {

void DefaultHandler(int i) {
  SignalHandler::GetSignalHandlerInstance()->Run(i);
}

SignalHandler SignalHandler::signal_handler_;

SignalHandler::SignalHandler() {
  memset(handler_array_, 0, sizeof(handler_array_));
}

SignalHandler::~SignalHandler() {
}

void SignalHandler::SetHandlerForDefaultSignals(Callback1<int>* callback) {
  if (!SetHandlerForSignal(2, callback)) {
    CHECK(false);
  }

  if (!SetHandlerForSignal(3, callback)) {
    CHECK(false);
  }

  if (!SetHandlerForSignal(15, callback)) {
    CHECK(false);
  }
}

bool SignalHandler::SetHandlerForSignal(int sig, Callback1<int>* callback) {
  if (!signal_handler_.CheckSignal(sig)) {
    return false;
  }

  signal_handler_.handler_array_[sig] = callback;
  signal(sig, DefaultHandler);

  return true;
}

void SignalHandler::RestoreSystemHandler() {
  for (int i = 0; i < 31; ++i) {
    if (RestoreSystemHandlerForSignal(i)) {
      CHECK(signal_handler_.handler_array_[i] == NULL);
    }
  }
}

bool SignalHandler::RestoreSystemHandlerForSignal(int sig) {
  if (!signal_handler_.CheckSignal(sig)) {
    return false;
  }

  signal_handler_.handler_array_[sig] = NULL;
  signal(sig, SIG_DFL);
  return true;
}

void SignalHandler::Run(int sig) {
  if (handler_array_[sig] == NULL) {
    CHECK(false);
  }

  handler_array_[sig]->Run(sig);
}

bool SignalHandler::CheckSignal(int sig) {
  if (sig > 31 || sig < 1) {
    return false;
  }

  if (sig == SIGSTOP || sig == SIGKILL) {
    return false;
  }
  return true;
}
}  // namespace signal_handler
