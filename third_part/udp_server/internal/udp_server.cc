// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-13 02:26:54
// File  : udp_server.cc
// Brief :

#include "third_part/udp_server/public/udp_server.h"
#include <fcntl.h>
#include <errno.h>
#include <string>

#define MAX_SENDBUF_SIZE (256 * 1024 * 1024)

DEFINE_int32(buffer_size, 1000, "");

namespace udp_server {

static void MaximizeSendBuff(int fd) {
  int old_size = 0;
  socklen_t intsize = sizeof(old_size);
  if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &old_size, &intsize) != 0) {
    LOG(ERROR) << "get buffer size error";
  } else {
    int begin = old_size;
    int end = MAX_SENDBUF_SIZE;
    while (begin <= end) {
      int mid = begin + (end - begin) / 2;
      if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF,
                     static_cast<void *>(&mid), intsize) == 0) {
        begin = mid + 1;
      } else {
        end = mid - 1;
      }
    }
  }
}

UdpServer::UdpServer(int port, int worker_num,
                     base::shared_ptr<BaseRouter> router) {
  fd_ = -1;
  port_ = port;
  router_ = router;
  int flags = 0;

  CHECK_NE((fd_ = socket(AF_INET, SOCK_DGRAM, 0)), -1);
  CHECK_GE((flags = fcntl(fd_, F_GETFL, 0)), 0);
  CHECK_GE(fcntl(fd_, F_SETFL, flags | O_NONBLOCK), 0);
  flags = 1;
  setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,
             static_cast<void *>(&flags), sizeof(flags));
  MaximizeSendBuff(fd_);

  sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port_);
  CHECK_GE(bind(fd_, (struct sockaddr *)&servaddr, sizeof(servaddr)), 0);

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

bool UdpServer::Send(const Node & node, const char * data, int len) {
  int rs = sendto(node.rfd, data, len, 0,
                  (struct sockaddr *)&(node.client_addr),
                  sizeof(node.client_addr));
  if (rs == len) {
    return true;
  } else {
    LOG(ERROR) << "when send udp data, errno=" << errno
               << " " << strerror(errno);
    return false;
  }
}

bool UdpServer::Send(const Node & node, const std::string & data) {
  return Send(node, data.c_str(), data.size());
}

Worker::Worker(int fd, UdpServer * server) : base::Thread(true) {
  rfd_ = -1;
  fd_ = fd;
  server_ = server;
  AllocateReplyFd(server_->GetWorkerNum() * 2);
}

Worker::~Worker() {
  if (rfd_ != -1) close(rfd_);
}

// If an UDP socket is unbound and either sendto or connect are called on it,
// the system will automatically bind it for you and thus the recvfrom call
// later on will succeed. recvfrom will not bind a socket, though, this call
// expects the socket to have been bound already or an error is thrown.
void Worker::AllocateReplyFd(int times) {
  int flags = 0;
  CHECK_NE((rfd_ = socket(AF_INET, SOCK_DGRAM, 0)), -1);
  CHECK_NE((flags = fcntl(rfd_, F_GETFL, 0)), 0);
  CHECK_GE(fcntl(rfd_, F_SETFL, flags | O_NONBLOCK), 0);
  flags = 1;
  setsockopt(rfd_, SOL_SOCKET, SO_REUSEADDR, &flags, sizeof(flags));
  MaximizeSendBuff(rfd_);

  sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int index = 0;
  for (index = 0; index < times; index++) {
    servaddr.sin_port = htons(server_->GetPort() + index + 1);
    if (bind(rfd_, (struct sockaddr *)&servaddr, sizeof(servaddr)) >= 0) {
      break;
    }
  }
  if (index == times) {
    close(rfd_);
    rfd_ = fd_;
  }
}

static void WorkerRead(const int fd, const int16_t which, void * arg) {
  Worker * worker = static_cast<Worker *>(arg);
  Node node;
  node.rfd = worker->GetRfd();
  std::string buffer(FLAGS_buffer_size, 0);
  char * data = const_cast<char *>(buffer.c_str());
  socklen_t len = sizeof(node.client_addr);
  int rs = recvfrom(fd, data, buffer.size(), 0,
                    (struct sockaddr *)&(node.client_addr), &len);
  if (rs == -1) {
    LOG(ERROR) << "recvfrom error errno=" << errno << " " << strerror(errno);
  } else if (rs == 0) {
    LOG(ERROR) << "client is shutdown";
  } else if (rs == buffer.size()) {
    LOG(ERROR) << "may read not full data, then we change data buff from "
               << FLAGS_buffer_size << " to " << FLAGS_buffer_size * 2;
    FLAGS_buffer_size *= 2;
  } else {
    worker->GetUdpServer()->GetRouter()->Process(data, node);
  }
}

void Worker::Run() {
  evbase_ = event_base_new();
  event_set(&event_, fd_, EV_READ | EV_PERSIST, WorkerRead, this);
  event_base_set(evbase_, &event_);
  event_add(&event_, NULL);
  event_base_dispatch(evbase_);
}

}  // namespace udp_server
