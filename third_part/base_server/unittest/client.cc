// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-08 22:28:41
// File  : client.cc
// Brief :

#include <string.h>
#include <stdlib.h>
#include "base/public/net.h"
#include "base/public/logging.h"
#include "base/public/thread.h"

class Client : public base::Thread {
 public:
  Client() : base::Thread(true) {}

 protected:
  virtual void Run() {
    int fd = base::TcpConnect("127.0.0.1", 30008, 100);
    char buf[104] = { 0 };
    while (true) {
      *reinterpret_cast<int *>(buf) = 100;
      const char * msg = "Hello Word!";
      memcpy(buf + 4, msg, strlen(msg));
      base::TcpSend(fd, buf, sizeof(buf));
      char res[104] = { 0 };
      base::TcpRecvLen(fd, res, sizeof(res), 100);
      LOG(INFO) << res + 4;
      // sleep(1);
    }
    close(fd);
  }
};

int main(int argc, char** argv) {
  Client c1;
  Client c2;
  c1.Start();
  c2.Start();
  c1.Join();
  c2.Join();
  return 0;
}
