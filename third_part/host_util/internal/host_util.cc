// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author: bingbai@yunrang.com (Bing Bai)

#include "third_part/host_util/public/host_util.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "base/public/logging.h"


DEFINE_bool(use_ip_address,
    false,
    "true to get local ip address instead of hostname");

namespace util {
host_util::host_util() {
}

host_util::~host_util() {
}

std::string host_util::LocalHostName() {
  struct addrinfo hints, *info;
  int gai_result;
  char hostname[1024];
  hostname[1023] = '\0';
  // http://hi.baidu.com/zengzhaonong/item/1fc52718233a2e0de75c3673
  gethostname(hostname, 1023);
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;
  std::string host;
  VLOG(3) << hostname;
  if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
    LOG(WARNING) << "getaddrinfo:" <<  gai_strerror(gai_result);
    host = hostname;
  } else {
    if (FLAGS_use_ip_address) {
      HERE(3);
      char ip4[INET_ADDRSTRLEN];
      inet_ntop(AF_INET,
               &((reinterpret_cast<sockaddr_in *>(info->ai_addr))->sin_addr),
               ip4,
               INET_ADDRSTRLEN);
      host = ip4;
    } else {
      // hostname == info->ai_canonname
      host = info->ai_canonname;
    }
    freeaddrinfo(info);
  }
  return host;
}
}
