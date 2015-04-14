// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-13 02:26:54
// File  : udp_server.cc
// Brief :

#include <string>
#include "third_part/udp_server/public/udp_server.h"

namespace udp_server {

UdpServer::UdpServer(const std::string & host, int port,
                     int worker_num, base::shared_ptr<BaseRouter> router) {
  fd_ = -1;
  host_ = host;
  port_ = port;
  router_ = router;

  for (int i = 0; i < worker_num; i++) {
    workers_.push_back(base::shared_ptr<Worker>(new Worker(fd_, this)));
  }
}

UdpServer::~UdpServer() {
  if (fd_ != -1) close(fd_);
}

void UdpServer::Start() {
  for (int i = 0; i < workers_.size(); i++) workers_[i]->Start();
  for (int i = 0; i < workers_.size(); i++) workers_[i]->Join();
}

Worker::Worker(int fd, UdpServer * server) : base::Thread(true) {
  rfd_ = -1;
  fd_ = fd;
  server_ = server;
}

Worker::~Worker() {
  if (rfd_ != -1) close(rfd_);
}
}  // namespace udp_server
