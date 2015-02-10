// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-08 22:18:05
// File  : server.cc
// Brief :

#include "third_part/base_server/public/base_server.h"

using base_server::BaseRouter;
using base_server::BaseServer;
using base_server::Node;

class Router : public BaseRouter {
 public:
  virtual bool Process(const std::string & content, const Node & node) {
    // LOG(INFO) << content.data() + 4;
    BaseServer::Send(node.bev, content);
    return true;
  }
};

class Monitor : public base::Thread {
 public:
  explicit Monitor(BaseServer * server)
      : base::Thread(true), server_(server) { }

 protected:
  void Run() {
    while (true) {
      std::string tmp;
      server_->Dump(&tmp);
      LOG(INFO) << tmp;
    }
  }

 private:
  BaseServer * server_;
};

int main(int argc, char** argv) {
  int port = 30008;
  base::shared_ptr<BaseRouter> router(new Router());
  int size = 5;

  BaseServer server(port, router, size);
  Monitor monitor(&server);
  monitor.Start();

  server.Start();
  return 0;
}
