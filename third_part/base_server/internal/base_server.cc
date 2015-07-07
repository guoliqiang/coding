// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-01 18:01:38
// File  : base_server.cc
// Brief :

#include <string>
#include "third_part/base_server/public/base_server.h"
#include "base/public/string_util.h"

namespace base_server {

DEFINE_int32(max_conns, 128, "");
DEFINE_int32(keepalive_offset, 100, "");
DEFINE_int32(keepalive_interval, 100, "");
DEFINE_int32(keepalive_probes, 100, "");

static void Accept(evutil_socket_t listener, int16_t event, void * arg) {
  BaseServer * server = static_cast<BaseServer*>(arg);
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  size_t client_fd = accept(listener,
                           reinterpret_cast<sockaddr*>(&client_addr),
                           &client_len);
  CHECK_GE(client_fd, 0) << "accept error";
  evutil_make_socket_nonblocking(client_fd);

  int optval = 1;
  socklen_t optlen = sizeof(optval);
  // Open the keepalive attribute for fd.
  CHECK_GE(setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen), 0)
    << "set SO_KEEPALIVE on client fd=" << client_fd << " failed";

  // From the time that the program do not used fd, after TCP_KEEPIDLE ms,
  // it will detect whether it is connected.
  optval = FLAGS_keepalive_offset;
  CHECK_GE(setsockopt(client_fd, SOL_TCP, TCP_KEEPIDLE, &optval, optlen), 0)
    << "set TCP_KEEPIDLE on client fd=" << client_fd << " failed";

  // FLAGS_keepalive_interval is the interval value, when start to detect,
  // after FLAGS_keepalive_interval ms it will detect it again.
  optval = FLAGS_keepalive_interval;
  CHECK_GE(setsockopt(client_fd, SOL_TCP, TCP_KEEPINTVL, &optval, optlen), 0)
    << "set TCP_KEEPINTVL on client fd=" << client_fd << " failed";
  
  // After detecting FLAGS_keepalive_probes times, if it is still unconnected.
  // It will throw EPOLLRDHUP event under epoll.
  optval = FLAGS_keepalive_probes;
  CHECK_GE(setsockopt(client_fd, SOL_TCP, TCP_KEEPCNT, &optval, optlen), 0)
    << "set TCP_KEEPCNT on client fd=" << client_fd << " failed";

  optval = 1;
  CHECK_GE(setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &optval,
           sizeof(optval)), 0)
    << "set TCP_NODELAY on client fd=" << client_fd << " failed";
  Node node;
  node.fd = client_fd;
  node.ip = inet_ntoa(client_addr.sin_addr);
  node.port = ntohs(client_addr.sin_port);
  server->Push(node);
  server->NotifyWorker();
}

void BaseServer::NotifyWorker() {
  worker_[index_]->Notify();
  index_ = (index_ + 1) % worker_.size();
}

void BaseServer::Start() {
  size_t listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  CHECK_GE(listen_fd, 0) << "create fd error!";
  CHECK_EQ(evutil_make_socket_nonblocking(listen_fd), 0)
    << "set linsten sock non block error!";

  int optval = 1;
  // http://www.cnblogs.com/mydomain/archive/2011/08/23/2150567.html
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  sockaddr_in listen_addr;
  bzero(&listen_addr, sizeof(listen_addr));
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = 0;
  listen_addr.sin_port = htons(port_);
  CHECK_EQ(bind(listen_fd, reinterpret_cast<sockaddr*>(&listen_addr),
        sizeof(listen_addr)), 0)
    << "bind listen sock error, start listen failed";
  CHECK_EQ(listen(listen_fd, FLAGS_max_conns), 0)
    << "begin listen sock error, start listen failed";

  CHECK_EQ(evthread_use_pthreads(), 0)
    << "enable pthreads for libevent failed!";
  evbase_ = event_base_new();
  CHECK(evbase_ != NULL) << "new base event error!";
  event * evlisten = event_new(evbase_, listen_fd, EV_READ | EV_PERSIST,
                               Accept, this);
  event_add(evlisten, NULL);

  for (size_t i = 0; i < worker_.size(); i++) worker_[i]->Start();
  // dead loop
  event_base_dispatch(evbase_);
  CHECK(false) << "should never be reached!";
}

BaseServer::BaseServer(size_t port, base::shared_ptr<BaseRouter> router,
                       size_t size) {
  port_ = port;
  router_ = router;
  evbase_ = NULL;
  index_ = 0;
  for (size_t i = 0; i < size; i++) {
    worker_.push_back(base::shared_ptr<Worker>(new Worker(this)));
  }
}

BaseServer::~BaseServer() {
  if (evbase_ != NULL) event_base_free(evbase_);
}

bool BaseServer::Read(bufferevent *bev, std::string * content) {
  evbuffer * input = bufferevent_get_input(bev);
  CHECK(input != NULL) << "get input error ";

  unsigned char * buff = evbuffer_pullup(input, 4);
  if (buff == NULL) {
    LOG(INFO) << "not enough data, buff == NULL";
    return false;
  }

  uint32_t size = *(reinterpret_cast<uint32_t*>(buff));
  uint32_t real_size = evbuffer_get_length(input);
  uint32_t total_size = size + 4;

  if (real_size < total_size) {  // not enough data
    LOG(INFO) << "not enough data, real_size = "
              << real_size << " total_size = " << total_size;
    return false;
  } else {
    content->resize(total_size);
    uint32_t cnt = evbuffer_remove(input, const_cast<char *>(content->data()),
                                   total_size);
    CHECK(cnt == total_size) << cnt << " " << total_size;
    return true;
  }
}

bool BaseServer::Send(bufferevent *bev, const std::string & content) {
  return bufferevent_write(bev, content.data(), content.size()) == 0;
}

void BaseServer::Dump(std::string * rs) {
  rs->clear();
  rs->append("\nconn queue size = " + IntToString(queue_.Size()) + "\n");
  for (size_t i = 0; i < worker_.size(); i++) {
    rs->append("thread [" + IntToString(i) + "] : ");
    worker_[i]->Dump(rs);
    rs->append("\n");
  }
}

static void WorkerRead(bufferevent *bev, void * arg) {
  Worker * worker = static_cast<Worker*>(arg);
  Node node = worker->FindFd(bufferevent_getfd(bev));
  std::string content;
  if (BaseServer::Read(bev, &content)) {
    worker->GetServer()->GetRouter()->Process(content, node);
  } else {
    LOG(WARNING) << "read illlegal format data and close the connection!";
    size_t fd = bufferevent_getfd(bev);
    worker->EraseFd(fd);
    bufferevent_free(bev);
  }
}

static void WorkerError(bufferevent *bev, int16_t error, void * arg) {
  size_t fd = bufferevent_getfd(bev);
  int last_error = evutil_socket_geterror(fd);
  if (error & BEV_EVENT_EOF) {
    LOG(ERROR) << "Read EOF for " << fd;
  } else if (error & BEV_EVENT_ERROR) {
    LOG(ERROR) << "Read ERROR for " << fd << " "
               << evutil_socket_error_to_string(last_error);
  } else if (error & BEV_EVENT_TIMEOUT) {
    LOG(ERROR) << "Read TIMEOUT for " << fd;
  } else {
    LOG(ERROR) << "Unknown error for " << fd << " "
               << evutil_socket_error_to_string(last_error);
  }
  Worker * worker = static_cast<Worker*>(arg);
  worker->EraseFd(fd);
  bufferevent_free(bev);
}

static void WorkerAccept(int fd, int16_t which, void * arg) {
  Worker * worker = static_cast<Worker*>(arg);
  char buff[1] = { 0 };
  if (read(fd, buff, 1) == 1) {
    Node node;
    worker->GetServer()->Pop(node);
    // http://blog.sina.com.cn/s/blog_56dee71a0100qx4s.html
    bufferevent * bev = bufferevent_socket_new(worker->GetEvBase(),
        node.fd, BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE);
    CHECK(bev != NULL) << "create bufferevent on client fd=" << node.fd;
    node.bev = bev;
    worker->AddFd(node);

    bufferevent_setcb(bev, WorkerRead, NULL, WorkerError, arg);
    CHECK_EQ(bufferevent_enable(bev, EV_READ), 0)
      << "enable " << node.fd << " " << node.ip << " "
      << node.port << " error!";
  }
}

void Worker::AddFd(const Node & node) {
  base::MutexLock lock(&mutex_);
  CHECK(fd_client_.count(node.fd) == false) << "find " << node.fd;
  fd_client_.insert(std::make_pair(node.fd, node));
}

void Worker::EraseFd(size_t fd) {
  base::MutexLock lock(&mutex_);
  CHECK(fd_client_.count(fd)) << "not find " << fd << " to erase!";
  fd_client_.erase(fd);
}

Node Worker::FindFd(size_t fd) {
  base::MutexLock lock(&mutex_);
  CHECK(fd_client_.count(fd)) << "not find " << fd << " to erase!";
  return fd_client_[fd];
}

void Worker::Dump(std::string * rs) {
  base::MutexLock lock(&mutex_);
  for (std::map<size_t, Node>::iterator i = fd_client_.begin();
       i != fd_client_.end(); i++) {
    if (i != fd_client_.begin()) rs->append(" ");
    rs->append(i->second.ip + ":" + IntToString(i->second.port));
  }
}

Worker::Worker(BaseServer * server) : base::Thread(true), server_(server) {
  int fds[2] = { 0 };
  CHECK_EQ(pipe(fds), 0) << "create Pipes error!";
  notify_receive_fd_ = fds[0];
  notify_send_fd_ = fds[1];

  evbase_ = event_base_new();
  CHECK(evbase_ != NULL) << "new base event error!";
  event_set(&notify_event_, notify_receive_fd_, EV_READ | EV_PERSIST,
            WorkerAccept, this);
  event_base_set(evbase_, &notify_event_);
  event_add(&notify_event_, NULL);
}

Worker::~Worker() {
  event_base_free(evbase_);
  close(notify_receive_fd_);
  close(notify_send_fd_);
}

void Worker::Run() {
  // dead loop
  event_base_dispatch(evbase_);
  CHECK(false) << "should never be reached!";
}

}  // namespace base_server
