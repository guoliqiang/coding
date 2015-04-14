// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-13 02:26:40
// File  : udp_server.h
// Brief :

#ifndef  __UDP_SERVER_H_
#define  __UDP_SERVER_H_

#include <vector>
#include "base/public/logging.h"
#include "base/public/thread.h"
#include "base/public/shared_ptr.h"

#include "third_part/libevent/include/event2/bufferevent.h"
#include "third_part/libevent/include/event2/bufferevent.h"
#include "third_part/libevent/include/event2/bufferevent_struct.h"
#include "third_part/libevent/include/event2/thread.h"
#include "third_part/libevent/include/event2/event-config.h"
#include "third_part/libevent/include/event2/buffer.h"
#include "third_part/libevent/include/event2/event_compat.h"

namespace udp_server {

struct Node {
  Node() : fd_(-1), rfd_(-1) {}
  int fd_;
  int rfd_;
};

class BaseRouter {
 public:
  BaseRouter() {}
  virtual ~BaseRouter() {}
  virtual bool Process(const std::string & data, const Node & node) = 0;
};

class UdpServer;

class Worker : public base::Thread {
  public:
   Worker(int fd, UdpServer * server_);
   ~Worker();
   void Run();

  private:
   event evet_;
   event_base * evbase_;
   int fd_;
   int rfd_;
   UdpServer * server_;
};

class UdpServer {
 public:
  UdpServer(const std::string & host, int port,
            int worker_num, base::shared_ptr<BaseRouter> router);
  ~UdpServer();
  void Start();
  static bool Send(const Node & node, const std::string & content);
  static bool Send(const Node & node, const char * str);

 private:
  std::vector<base::shared_ptr<Worker> > workers_;
  int fd_;
  std::string host_;
  int port_;
  base::shared_ptr<BaseRouter> router_;
};

}  // namespace udp_server


#endif  //__UDP_SERVER_H_
