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
      // char res[104] = { 0 };
      // base::TcpRecvLen(fd, res, sizeof(res), 100);
      // LOG(INFO) << res + 4;
      // sleep(1);
    }
    close(fd);
  }
};

int main(int argc, char** argv) {
  Client c1;
  Client c2;
  Client c3;
  Client c4;
  Client c5;
  Client c6;
  Client c7;
  Client c8;
  Client c9;
  Client c10;
  Client c11;


  c1.Start();
  c2.Start();
  c3.Start();
  c4.Start();
  c5.Start();
  c6.Start();
  c7.Start();
  c8.Start();
  c9.Start();
  c10.Start();
  c11.Start();

  c1.Join();
  c2.Join();
  c3.Join();
  c4.Join();
  c5.Join();
  c6.Join();
  c7.Join();
  c8.Join();
  c9.Join();
  c10.Join();
  c11.Join();
  return 0;
}
