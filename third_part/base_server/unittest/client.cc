// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-08 22:28:41
// File  : client.cc
// Brief :

#include <string.h>
#include <stdlib.h>
#include "base/public/net.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  int fd = base::TcpConnect("127.0.0.1", 30008, 100);
  char buf[104] = { 0 };
  for (int i = 0; i < 10; i++) {
    *reinterpret_cast<int *>(buf) = 100;
    const char * msg = "Hello Word!";
    memcpy(buf + 4, msg, strlen(msg));
    base::TcpSend(fd, buf, sizeof(buf));
    char res[104] = { 0 };
    base::TcpRecvLen(fd, res, sizeof(res), 100);
    LOG(INFO) << res + 4;
  }
  close(fd);
  return 0;
}

