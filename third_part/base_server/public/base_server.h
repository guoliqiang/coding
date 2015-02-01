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

namespace base_server {

struct Node {
  bufferevent * bev;
  int fd;
  std::string ip;
  int port;
  base::shared_ptr<std::string> content;
};

class BaseRouter {
 public:
  BaseRouter() {}
  virtual ~BaseRouter() {}
  virtual bool Process(base::shared_ptr<Node> client) = 0;
};

class Worker;

class BaseServer {
 public:
  BaseServer(int prot, base::shared_ptr<BaseRouter> router,
             int size, bool is_sync = true);
  virtual ~BaseServer() {
    if (evbase_ != NULL) event_base_free(evbase_);
  }

  static bool Send(base::shared_ptr<Node> client);
  static bool Read(base::shared_ptr<Node> client);

  void Start();
  bool IsSync() { return is_sync_; }
  event_base * GetBaseEvent() { return evbase_; }

  void AddFd(int fd, const std::string & ip, int port) {
    CHECK(fd_client_.count(fd) == false) << "find " << fd;
    fd_client_.insert(std::make_pair(fd, std::make_pair(ip, port)));
  }

  void EraseFd(int fd) {
    CHECK(fd_client_.count(fd)) << "not find " << fd << " to erase!";
    fd_client_.erase(fd);
  }

  const std::pair<std::string, int> FindFd(int fd) {
    CHECK(fd_client_.count(fd)) << "not find " << fd << " to erase!";
    return fd_client_[fd];
  }
  void PushNode(const base::shared_ptr<Node> & node) { queue_.Push(node); }
  void PopNode(base::shared_ptr<Node> & node) { queue_.Pop(node); }
  BaseRouter * GetRouter() { return router_.get(); }
  event_base * GetEventBase() { return evbase_; }

 protected:
  base::shared_ptr<BaseRouter> router_;
  base::ConcurrentQueue<base::shared_ptr<Node> > queue_;
  event_base * evbase_;
  std::vector<base::shared_ptr<Worker> > worker_;
  int listen_fd_;
  int port_;
  std::map<int, std::pair<std::string, int> > fd_client_;
  bool is_sync_;
};

class Worker : public base::Thread {
 public:
  Worker(BaseServer * server) : base::Thread(true), server_(server) {}

 protected:
  virtual void Run() {
    while (true) {
      base::shared_ptr<Node> node;
      server_->PopNode(node);
      server_->GetRouter()->Process(node);
      if (server_->IsSync()) {
        bufferevent_enable(node->bev, EV_READ);
      }
    }
  }

 private:
  BaseServer * server_;
};

}  // namespace base_server


#endif  //__BASE_SERVER_H_
