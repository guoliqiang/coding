// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-31 15:57:00
// File  : base_server.h
// Brief :

#ifndef  __BASE_SERVER_H_
#define  __BASE_SERVER_H_

#include <signal.h>
#include <netinet/tcp.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <map>

#include "base/public/logging.h"
#include "base/public/shared_ptr.h"
#include "base/public/thread.h"
#include "base/public/concurrent_queue.h"
#include "third_part/libevent/include/event2/bufferevent.h"
#include "third_part/libevent/include/event2/event.h"
#include "third_part/libevent/include/event2/bufferevent.h"
#include "third_part/libevent/include/event2/bufferevent_struct.h"
#include "third_part/libevent/include/event2/thread.h"
#include "third_part/libevent/include/event2/event-config.h"
#include "third_part/libevent/include/event2/buffer.h"
#include "third_part/libevent/include/event2/event_compat.h"

namespace base_server {

class BaseRouter {
 public:
  BaseRouter() {}
  virtual ~BaseRouter() {}
  virtual bool Process(const std::string & data, bufferevent * bev,
                       const std::pair<std::string, int> & ip_port) = 0;
};

class Worker;

class BaseServer {
 public:
  BaseServer(int prot, base::shared_ptr<BaseRouter> router, int size);
  virtual ~BaseServer();

  static bool Send(bufferevent * bev, const std::string & content);
  static bool Read(bufferevent * bev, std::string * content);

  void Start();
  void AddFd(int fd, const std::string & ip, int port);
  void EraseFd(int fd);
  std::pair<std::string, int> FindFd(int fd);

  event_base * GetBaseEvent() { return evbase_; }
  void Push(const int fd) { queue_.Push(fd); }
  void Pop(int & fd) { queue_.Pop(fd); }
  BaseRouter * GetRouter() { return router_.get(); }
  event_base * GetEventBase() { return evbase_; }
  void NotifyWorker();

 protected:
  base::shared_ptr<BaseRouter> router_;
  base::ConcurrentQueue<int> queue_;
  event_base * evbase_;
  std::vector<base::shared_ptr<Worker> > worker_;
  int listen_fd_;
  int port_;
  int index_;
  std::map<int, std::pair<std::string, int> > fd_client_;
};

class Worker : public base::Thread {
 public:
  Worker(BaseServer * server);
  ~Worker();

  void Notify() { write(notify_send_fd_, " ", 1); }
  BaseServer * GetServer() { return server_; }
  event_base * GetEvBase() { return evbase_; }

 protected:
  virtual void Run();

 private:
  BaseServer * server_;
  event notify_event_;
  int notify_receive_fd_;
  int notify_send_fd_;
  event_base * evbase_;
};

}  // namespace base_server


#endif  //__BASE_SERVER_H_
