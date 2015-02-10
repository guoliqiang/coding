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

struct Node {
  Node() : fd(-1), ip(""), port(-1), bev(NULL) { }
  int fd;
  std::string ip;
  int port;
  bufferevent * bev;
};

class BaseRouter {
 public:
  BaseRouter() {}
  virtual ~BaseRouter() {}
  virtual bool Process(const std::string & data, const Node & node) = 0;
};

class Worker;

class BaseServer {
 public:
  BaseServer(int prot, base::shared_ptr<BaseRouter> router, int size);
  virtual ~BaseServer();

  static bool Send(bufferevent * bev, const std::string & content);
  static bool Read(bufferevent * bev, std::string * content);

  void Start();
  void NotifyWorker();
  void Dump(std::string * rs);
  event_base * GetBaseEvent() { return evbase_; }
  void Push(const Node & node) { queue_.Push(node); }
  void Pop(Node & node) { queue_.Pop(node); }
  BaseRouter * GetRouter() { return router_.get(); }
  event_base * GetEventBase() { return evbase_; }

 protected:
  base::shared_ptr<BaseRouter> router_;
  base::ConcurrentQueue<Node> queue_;
  event_base * evbase_;
  std::vector<base::shared_ptr<Worker> > worker_;
  int port_;
  int index_;
};

class Worker : public base::Thread {
 public:
  Worker(BaseServer * server);
  ~Worker();

  void Notify() { write(notify_send_fd_, " ", 1); }
  BaseServer * GetServer() { return server_; }
  event_base * GetEvBase() { return evbase_; }

  void AddFd(const Node & node);
  void EraseFd(int fd);
  Node FindFd(int fd);
  void Dump(std::string * rs);

 protected:
  virtual void Run();

 private:
  BaseServer * server_;
  event notify_event_;
  int notify_receive_fd_;
  int notify_send_fd_;
  event_base * evbase_;
  std::map<int, Node> fd_client_;
  base::Mutex mutex_;
};

}  // namespace base_server

#endif  //__BASE_SERVER_H_
