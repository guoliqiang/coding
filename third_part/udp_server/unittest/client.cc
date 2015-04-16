// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-15 01:01:25
// File  : client.cc
// Brief :

#include "base/public/logging.h"
#include "base/public/net.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

int main(int argc, char** argv) {
  int fd = socket(AF_INET,SOCK_DGRAM,0);
  struct sockaddr_in servaddr;
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(9005);

  for (int i = 0; i < 100; i++) {
    std::string data =  "Hello World";
    sendto(fd, data.c_str(), data.size(), 0,
        (sockaddr *)(&servaddr), sizeof(servaddr));
    char buffer[1000] = { 0 };
    int rs = recvfrom(fd, buffer, sizeof(buffer), 0, NULL, NULL);
    LOG(INFO) << rs << " " << buffer;
  }
  return 0;
}
