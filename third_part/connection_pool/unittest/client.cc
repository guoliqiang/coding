// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-08 22:28:41
// File  : client.cc
// Brief :

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "base/public/net.h"
#include "base/public/logging.h"
#include "base/public/thread.h"
#include "third_part/connection_pool/public/connection_pool.h"

DECLARE_int32(v);

int main(int argc, char** argv) {
  signal(SIGPIPE, SIG_IGN);
  signal(SIGUSR2, SIG_IGN);
  signal(SIGUSR1, SIG_IGN);
  connection_pool::ConnectionPool pool("127.0.0.1", 30008, 10);
  pool.StartMonitor();
  char buf[104] = { 0 };
  while (true) {
    scoped_ptr<base::InstanceWrapper<connection_pool::ConnectionHandler> > one;
    pool.GetOneHandler(&one);
    *reinterpret_cast<int *>(buf) = 100;
    const char * msg = "Hello Word!";
    memcpy(buf + 4, msg, strlen(msg));
    if (-1 == one->Get().Write(buf, sizeof(buf))) {
      LOG(INFO) << "write Error";
    } else {
      char res[104] = { 0 };
      if (-1 == one->Get().Read(res, sizeof(res))) {
        LOG(INFO) << "Read Error";
      }
      LOG(INFO) << res + 4;
    }
  }
  pool.Join();
}
