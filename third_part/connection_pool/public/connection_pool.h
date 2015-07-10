// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-07-08 20:29:48
// File  : connection_pool.h
// Brief :

#ifndef  __CONNECTION_POOL_H_
#define  __CONNECTION_POOL_H_

#include <sys/epoll.h>
#include <vector>
#include <list>
#include <string>
#include "base/public/thread.h"
#include "base/public/net.h"
#include "base/public/scoped_ptr.h"
#include "base/public/instance_pool.h"

namespace connection_pool {

class ConnectionHandler {
 public:
  friend class ConnectionPool;
  ConnectionHandler(const std::string & ip, int port);
  ~ConnectionHandler();

  int Read(char * buffer, int size);
  int Write(const char * buffer, int size);

 private:
  inline void Close();
  inline bool Connect(const std::string & ip, int port);
  bool Valid() { return fd_ != -1; }

 private:
  mutable int fd_;
};

class ConnectionPool : public base::Thread {
 public:
  ConnectionPool(const std::string & ip, int port, int num);
  ~ConnectionPool();

  void EpollAdd(ConnectionHandler * ptr, int events);
  void EpollDelete(ConnectionHandler * ptr, int events);
  void StartMonitor() { Start(); }
  virtual void Run();
  void GetOneHandler(
      scoped_ptr<base::InstanceWrapper<ConnectionHandler> > * ptr);

 private:
  int max_number_;
  base::shared_ptr<base::InstancePool<ConnectionHandler> > pool_;
  std::vector<ConnectionHandler *>  handlers_;
  std::list<ConnectionHandler *> invalid_;
  struct epoll_event * events_;
  int ep_fd_;
  int port_;
  std::string ip_;
};

}  // namespace connection_pool


#endif  //__CONNECTION_POOL_H_
