// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-07-08 21:02:31
// File  : connection_pool.cc
// Brief :
// https://groups.google.com/forum/#!topic/comp.os.linux.networking/cLbMGRNw8EA

#include "third_part/connection_pool/public/connection_pool.h"

namespace connection_pool {

DEFINE_int32(keepalive_offset, 100, "");
DEFINE_int32(keepalive_interval, 10, "");
DEFINE_int32(keepalive_probes, 10, "");
DEFINE_int32(read_timeout, 10, "");
DEFINE_int32(epoll_timeout, 5, "");

static int event_value =  EPOLLHUP | EPOLLERR | EPOLLRDHUP;

ConnectionHandler::ConnectionHandler(const std::string & ip, int port)
    : fd_(-1) {
  if (!Connect(ip, port)) {
    LOG(WARNING) << "can not connect to " << ip << ":" << port;
  }
}

ConnectionHandler::~ConnectionHandler() {
  Close();
}

void ConnectionHandler::Close() {
  if (!Valid()) close(fd_);
  fd_ = -1;
}

bool ConnectionHandler::Connect(const std::string & ip, int port) {
  Close();
  fd_ = base::TcpConnect(ip.c_str(), port, 100);
  if (Valid()) {
    if (!base::TcpSetKeepAliveAndReuse(fd_, FLAGS_keepalive_offset,
                                       FLAGS_keepalive_interval,
                                       FLAGS_keepalive_probes)) {
      LOG(WARNING) << "set keepalive error!";
      return false;
    }
    LOG(INFO) << "connect to " << ip << ":" << port << " " << fd_;
    return true;
  } else {
    LOG(WARNING) << "can not connect to " << ip << ":" << port << " " << fd_;
    return false;
  }
}


int ConnectionHandler::Write(const char * buffer, int size) {
  if (Valid()) {
    return base::TcpSend(fd_, buffer, size);
  } else {
    return -1;
  }
}

int ConnectionHandler::Read(char * buffer, int size) {
  if (Valid()) {
    return base::TcpRecvLen(fd_, buffer, size, FLAGS_read_timeout);
  } else {
    return -1;
  }
}

void ConnectionPool::EpollAdd(ConnectionHandler * ptr, int events)  {
  struct epoll_event ev;
  ev.data.fd = ptr->fd_;
  ev.data.ptr = ptr;
  ev.events = events;
  CHECK_GE(epoll_ctl(ep_fd_, EPOLL_CTL_ADD, ptr->fd_, &ev), 0);
}

void ConnectionPool::EpollDelete(ConnectionHandler * ptr, int events) {
  struct epoll_event ev;
  ev.data.fd = ptr->fd_;
  ev.data.ptr = ptr;
  ev.events = events;
  CHECK_GE(epoll_ctl(ep_fd_, EPOLL_CTL_DEL, ptr->fd_, &ev), 0);
}

ConnectionPool::ConnectionPool(const std::string & ip, int port, int num)
    : base::Thread(true) {
  max_number_ = num;
  ip_ = ip;
  port_ = port;
  events_ = new epoll_event[max_number_];
  CHECK_GE((ep_fd_ = epoll_create(max_number_)), 0);

  for (int i = 0; i < max_number_; i++) {
    ConnectionHandler * ptr = new ConnectionHandler(ip_, port_);
    if (ptr->Valid()) {
      EpollAdd(ptr,  event_value);
    } else {
      invalid_.push_back(ptr);
    }
    base::InstancePool<ConnectionHandler>::Get().AddInstance(ptr);
    handlers_.push_back(ptr);
  }
}

ConnectionPool::~ConnectionPool() {
  for (int i = 0; i < handlers_.size(); i++) {
    delete handlers_[i];
    handlers_[i] = NULL;
  }
  delete events_;
  close(ep_fd_);
}

void ConnectionPool::Run() {
  while (true) {
    int cnt = epoll_wait(ep_fd_, events_, max_number_, FLAGS_epoll_timeout);
    if (cnt < 0 && errno == EINTR) continue;
    CHECK_GE(cnt, 0) << "epoll wait error. errno=" << errno;
    for (int i = 0; i < cnt; i++) {
      LOG(INFO) << "Find faild event and begin to reconnect.";
      ConnectionHandler * ptr =
          reinterpret_cast<ConnectionHandler *>(events_[i].data.ptr);
      EpollDelete(ptr, event_value);
      ptr->Connect(ip_, port_);
      if (ptr->Valid()) {
        EpollAdd(ptr, event_value);
      } else {
        invalid_.push_back(ptr);
      }
    }

    for (std::list<ConnectionHandler *>::iterator i = invalid_.begin();
         i != invalid_.end();) {
      (*i)->Connect(ip_, port_);
      if ((*i)->Valid()) {
        EpollAdd(*i,  event_value);
        i = invalid_.erase(i);
      } else {
        i++;
      }
    }
  }
}

void ConnectionPool::GetOneHandler(
    scoped_ptr<base::InstanceWrapper<ConnectionHandler> > * ptr) {
  base::InstancePool<ConnectionHandler>::Get().GetInstanceWrapper(ptr);
}

}  // namespace connection_pool
