// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#ifndef UTIL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_
#define UTIL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_

#include <signal.h>
#include "base/public/callback.h"
#include "base/public/hash_tables.h"
#include "base/public/basictypes.h"

namespace signal_handler {
void DefaultHandler(int i);

class SignalHandler {
  friend void DefaultHandler(int i);
 public:

  // for default, set handler for SIGINT[2], SIGQUIT[3], SIGTERM[15]
  static void SetHandlerForDefaultSignals(base::Callback1<int>* callback);

  // set handler for appointed signal
  static bool SetHandlerForSignal(int sig, base::Callback1<int>* callback);

  // restore the system action for signals
  static void RestoreSystemHandler();

  // restore the system action for appointed signal
  static bool RestoreSystemHandlerForSignal(int sig);

 private:
  static SignalHandler* GetSignalHandlerInstance() {
    return &signal_handler_;
  }
  SignalHandler();
  ~SignalHandler();
  void Run(int sig);
  bool CheckSignal(int sig);

  static SignalHandler signal_handler_;
  base::Callback1<int>* handler_array_[31];
  DISALLOW_COPY_AND_ASSIGN(SignalHandler);
};
}

#endif  // UTIL_SIGNAL_HANDLER_SIGNAL_HANDLER_H_
