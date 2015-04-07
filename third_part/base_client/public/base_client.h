// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-06 19:19:16
// File  : base_client.h
// Brief :

#ifndef  __BASE_CLIENT_H_
#define  __BASE_CLIENT_H_

#include "base/public/logging.h"
#include "base/public/net.h"

namespace base_client {

class BaseClient {
 public:
  BaseClient(const std::string & ip, int port, int try_time = 1);
  ~BaseClient();
  bool Connect();
  void Close();
  bool IsValid();
  bool NeedConnect();

  bool Send(int8_t cmdid, char * data, int data_size);
  bool Receive(int8_t * cmdid, std::string * data);
  bool Request(int8_t cmdid, char * data,
               int data_size, std::string * response);
  
 private:
  std::string ip_;
  int port_;
  int fd_;
  int try_time_;
};

}  // namespace client

#endif  //__BASE_CLIENT_H_
