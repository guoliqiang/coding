// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Zhongwu Zhai(zhaizhongwu@jike.com)
// Date  : 2014-05-06 22:24:35
// File  : client.cc
// Brief :
#include "base/public/net.h"
#include "base/public/logging.h"

int main(int argc, char** argv) {
  int fd = base::TcpConnect("127.0.0.1", 8003, 100);
  char buffer[1000] = {0};
  int rs = base::TcpSend(fd, buffer, 1000);
  LOG(INFO) << rs;
  int len = base::TcpRecv(fd, buffer, 1000, 100);
  LOG(INFO) << len;
  LOG(INFO) << buffer;
  return 0;
}
