// Copyright 2014 Jike Inc. All Rights Reserved.
// Date  : 2014-05-06 20:20:33 // File  : server.cc // Brief :
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "base/public/thread.h"
#include "../public/ependingpool.h"
#include "base/public/net.h"
#include "third_part/epoll/proto/epoll_test.pb.h"

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
    char read_buffer[100000] = {'\0'};
    while (pool_->is_run()) {
      if(pool_->fetch_item(&handle, &fd) != 0) {
        LOG(INFO) << "continue";
        continue;
      }
      int len = base::TcpRecv(fd, read_buffer, sizeof(read_buffer), 100);
      char * ptr = read_buffer;
      len = *((int *)ptr);
      ptr += 4;
      int cmd = *((int *)ptr);
      ptr += 4;
      len -= 8;
      epoll::Request request;
      request.ParseFromArray(ptr, len);
      LOG(INFO) << cmd << " " << request.DebugString();

      epoll::Response response;
      response.set_str("response guoliqiang");
      ptr = read_buffer;
      len = response.ByteSize() + 4;
      *((int *) ptr) = len;
      ptr += 4;
      response.SerializeToArray(ptr, response.ByteSize());
      base::TcpSend(fd, read_buffer, len);

      pool_->reset_item(handle, false);
    }
  }
 private:
  ependingpool * pool_;
};

// long conn
class WorkThreadLongConn : public base::Thread {
 public:
  WorkThreadLongConn(ependingpool * pool) : base::Thread(true) {
    pool_ = pool;
  }
 protected:
  virtual void Run() {
    int handle;
    int fd;
    std::string str = "hello word!";
    char read_buffer[100000] = {'\0'};
    while (pool_->is_run()) {
      if(pool_->fetch_item(&handle, &fd) != 0) {
        continue;
      }
      if (base::TcpRecv(fd, read_buffer, sizeof(read_buffer), 100) < 0) {
        pool_->reset_item(handle, false); // close the handle
        LOG(INFO) << "read error!";
        continue;
      }
      LOG(INFO) << "read:" << read_buffer;
      if (base::TcpSend(fd, str.c_str(), str.size()) == -1) {
        pool_->reset_item(handle, false); // close the handle
      } else pool_->reset_item(handle, true);  // keep the handle
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
  // WorkThreadLongConn work(&pool);
  lisen.Start();
  work.Start();
  lisen.Join();
  work.Join();
  return 0;
}
