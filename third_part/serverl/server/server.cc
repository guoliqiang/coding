// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-09-28 22:11:35
// File  : server.cc
// Brief :

#include "../public/serverl.h"
#include "base/public/logging.h"

class PacketRouter : public IPacketRouter {
 public:
  PacketRouter() {};
  virtual ~PacketRouter() {}
  virtual void OnReceivePacket(socketClientData_t sClient,
                               const char * pData, int nDataLen) {
    LOG(INFO) << "get it";
  }
};

void ClientTCPCloseCB(void* param) {
  uint64_t sockkey = *((uint64_t*)param);
  LOG(INFO) << "close " << sockkey;
}
int main(int argc, char** argv) {
  g_serverl.setPort(30006);
  g_serverl.setPacketRouter(new PacketRouter());
  g_serverl.setWorkerCount(10);
  g_serverl.setClientTcpDisconnectCB(ClientTCPCloseCB);
  g_serverl.start();
  return 0;
}

