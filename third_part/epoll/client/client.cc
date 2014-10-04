// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Zhongwu Zhai(zhaizhongwu@jike.com)
// Date  : 2014-05-06 22:24:35
// File  : client.cc
// Brief :
#include "base/public/net.h"
#include "base/public/logging.h"
#include "third_part/epoll/proto/epoll_test.pb.h"

int main(int argc, char** argv) {
  int fd = base::TcpConnect("127.0.0.1", 8003, 100);
  char buffer[100000] = {0};
  epoll::Request request;
  request.set_str("request guoliqiang");
  int len = request.ByteSize();
  char * ptr = buffer;
  *((int *)ptr) = len + 4 + 4;
  ptr += 4;
  *((int *)ptr) = static_cast<int>(epoll::OPCODE);
  ptr += 4;
  request.SerializeToArray(ptr, len);
  int rs = base::TcpSend(fd, buffer, len + 8);
  LOG(INFO) << rs;
  len = base::TcpRecv(fd, buffer, 100000, 100);
  epoll::Response response;
  ptr = buffer;
  len = *((int *) ptr) - 4;
  ptr += 4;
  response.ParseFromArray(ptr, len);
  LOG(INFO) << response.DebugString();

  close(fd);
  return 0;
}
