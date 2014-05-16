// Copyright 2014 Jike Inc. All Rights Reserved.
// Date  : 2014-05-06 20:20:33
// File  : server.cc
// Brief :
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/public/thread.h"
#include "../public/ependingpool.h"
#include "base/public/net.h"

class LisenThread : public base::Thread {
 public:
  LisenThread(ependingpool * pool) : base::Thread(true) {
    pool_ = pool;
  }
 protected:
  virtual void Run() {
    signal(SIGPIPE, SIG_IGN);
    signal(SIGUSR2, SIG_IGN);
    signal(SIGUSR1, SIG_IGN);
    int sock = base::TcpListen(8003, 100);
    base::SetNonblocking(sock);
    pool_->set_listen_fd(sock);
    while (pool_->is_run()) {
      pool_->check_item();
    }
  }
 private:
  ependingpool * pool_;
};

class WorkThread : public base::Thread {
 public:
  WorkThread(ependingpool * pool) : base::Thread(true) {
    pool_ = pool;
  }
 protected:
  virtual void Run() {
    int handle;
    int fd;
    std::string str = "hello word!";
    while (pool_->is_run()) {
      if(pool_->fetch_item(&handle, &fd) != 0) {
        LOG(INFO) << "continue";
        continue;
      }
      LOG(INFO) << str;
      base::TcpSend(fd, str.c_str(), str.size());
      pool_->reset_item(handle, false);
    }
  }
 private:
  ependingpool * pool_;
};

int main(int argc, char** argv) {
  ependingpool pool;
  pool.set_conn_timeo(10);
  LisenThread lisen(&pool);
  WorkThread work(&pool);
  lisen.Start();
  work.Start();
  lisen.Join();
  work.Join();
  return 0;
}
