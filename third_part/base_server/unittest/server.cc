// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-08 22:18:05
// File  : server.cc
// Brief :

#include "third_part/base_server/public/base_server.h"

using base_server::BaseRouter;
using base_server::BaseServer;

class Router : public BaseRouter {
 public:
  virtual bool Process(const std::string & content, struct bufferevent * bev,
                       const std::pair<std::string, int> & ip_port ) {
    LOG(INFO) << content.data() + 4;
    BaseServer::Send(bev, content);
    return true;
  }
};

int main(int argc, char** argv) {
  int port = 30008;
  base::shared_ptr<BaseRouter> router(new Router());
  int size = 1;

  BaseServer server(port, router, size);
  server.Start();
  return 0;
}
