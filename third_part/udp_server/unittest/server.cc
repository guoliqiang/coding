// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-15 00:57:10
// File  : server.cc
// Brief :

#include "third_part/udp_server/public/udp_server.h"

using udp_server::UdpServer;
using udp_server::Node;
using udp_server::BaseRouter;

class Router : public BaseRouter {
 public:
  void Process(const std::string & data, const Node & node) {
    LOG(INFO) << data;
    UdpServer::Send(node, data);
  }
};

int main(int argc, char** argv) {
  base::shared_ptr<BaseRouter> router(new Router());
  UdpServer server(9003, 10, router);
  server.Start();
  return 0;
}
