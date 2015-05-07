// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-04-06 19:25:26
// File  : base_client.cc
// Brief :

#include <string>
#include "third_part/base_client/public/base_client.h"

namespace base_client {

DEFINE_int32(connect_timeout, 100, "100ms");
DEFINE_int32(receive_timeout, 300, "300ms");

BaseClient::BaseClient(const std::string & ip, int port, int try_time) {
  ip_ = ip;
  port_ = port;
  fd_ = -1;
  try_time_ = try_time;
  CHECK(Connect()) << "connect server " << ip_ << ":" << port_ << " error";
}

BaseClient::~BaseClient() {
  Close();
}

bool BaseClient::Connect() {
  fd_ = base::TcpConnect(ip_.c_str(), port_, FLAGS_connect_timeout);
  return fd_ != -1;
}

void BaseClient::Close() {
  if (IsValid()) {
    close(fd_);
    fd_ = -1;
  }
}

bool BaseClient::IsValid() {
  return fd_ > 0;
}

bool BaseClient::NeedConnect() {
  if (!IsValid()) return Connect();
  return true;
}

bool BaseClient::Send(int8_t cmdid, char * data, int data_size) {
  if (!NeedConnect()) {
    LOG(ERROR) << "Connect "  << ip_ << ":" << port_ << " error";
    return false;
  }
  int size = data_size + 5;
  std::string buff(size, '0');
  char * ptr = const_cast<char*>(buff.data());
  *(reinterpret_cast<int32_t *>(ptr)) = size - 4;
  ptr += 4;
  *(reinterpret_cast<int8_t *>(ptr)) = cmdid;
  ptr += 1;
  memcpy(ptr, data, data_size);

  int cnt = 0;
  for (cnt = 0; cnt < try_time_; cnt++) {
    if (base::TcpSend(fd_, buff.data(), buff.size()) != -1) break;
    Close();
    if (!NeedConnect()) {
      LOG(ERROR) << "Connect "  << ip_ << ":" << port_ << " error";
      return false;
    }
  }
  if (cnt == try_time_) {
    Close();
    return false;
  }
  return true;
}

bool BaseClient::Receive(int8_t * cmdid, std::string * data) {
  if (!IsValid()) {
    LOG(ERROR) << "connetion is not build, cant not reveive any data";
    if (!NeedConnect()) {
      LOG(ERROR) << "Connect "  << ip_ << ":" << port_ << " error";
      return false;
    }
  }
  int error_no = 0;
  int size = 0;
  if ((error_no = base::TcpRecvLen(fd_, &size, 4, FLAGS_receive_timeout)) < 0) {
    LOG(ERROR) << "receive data size error, error_no=" << error_no;
    Close();
    return false;
  }

  if ((error_no = base::TcpRecvLen(fd_, cmdid, 1, FLAGS_receive_timeout)) < 0) {
    LOG(ERROR) << "receive cmdid error, error_no=" << error_no;
    Close();
    return false;
  }

  data->resize(size - 1);
  if ((error_no = base::TcpRecvLen(fd_, data, size - 1,
                                   FLAGS_receive_timeout)) < 0) {
    LOG(ERROR) << "receive data error, error_no=" << error_no;
    Close();
    return false;
  }
  return true;
}

bool BaseClient::Request(int8_t cmdid, char * data, int data_size,
                         std::string * response) {
  if (!Send(cmdid, data, data_size)) return false;
  if (!Receive(&cmdid, response)) return false;
  return true;
}

}   // namespace base_client
