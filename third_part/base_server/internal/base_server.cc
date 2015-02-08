// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-02-01 18:01:38
// File  : base_server.cc
// Brief :

#include <string>
#include "third_part/base_server/public/base_server.h"

namespace base_server {

DEFINE_int32(max_conns, 128, "");
DEFINE_int32(keepalive_offset, 100, "");
DEFINE_int32(keepalive_interval, 100, "");
DEFINE_int32(keepalive_probes, 100, "");

static void DoRead(struct bufferevent *bev, void * arg) {
  BaseServer * server = static_cast<BaseServer*>(arg);
  base::shared_ptr<Node> node(new Node());
  node->bev = bev;
  std::pair<std::string, int> ip_port = server->FindFd(bufferevent_getfd(bev));
  node->ip = ip_port.first;
  node->port = ip_port.second;
  node->content = base::shared_ptr<std::string>(new std::string());

  server->PushNode(node);
  bufferevent_disable(bev, EV_READ);
  LOG(INFO) << "Push fd = " << bufferevent_getfd(bev) << " to work queue";
}


static void DoError(struct bufferevent *bev, int16_t error, void * arg) {
  int fd = bufferevent_getfd(bev);
  if (error & BEV_EVENT_EOF) {
    LOG(INFO) << "Read EOF for " << fd;
  } else if (error & BEV_EVENT_ERROR) {
    LOG(ERROR) << "Read ERROR for " << fd;
  } else if (error & BEV_EVENT_TIMEOUT) {
    LOG(ERROR) << "Read TIMEOUT for " << fd;
  } else {
    LOG(ERROR) << "Unknown error for " << fd;
  }
  BaseServer * server = static_cast<BaseServer*>(arg);
  server->EraseFd(fd);
  bufferevent_free(bev);
}

static void DoAccept(evutil_socket_t listener, int16_t event, void * arg) {
  BaseServer * server = static_cast<BaseServer*>(arg);
  event_base * evbase = server->GetEventBase();
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_fd = accept(listener,
                         reinterpret_cast<struct sockaddr*>(&client_addr),
                         &client_len);
  if (client_fd < 0) {
    LOG(ERROR) << "accept error";
    return;
  }
  evutil_make_socket_nonblocking(client_fd);
  int optval = 1;
  socklen_t optlen = sizeof(optval);
  if (setsockopt(client_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
    LOG(ERROR) << "set SO_KEEPALIVE on client fd=" << client_fd << " failed";
    return;
  }

  optval = FLAGS_keepalive_offset;
  if (setsockopt(client_fd, SOL_TCP, TCP_KEEPIDLE, &optval, optlen) < 0) {
    LOG(ERROR) << "set TCP_KEEPIDLE on client fd=" << client_fd << " failed";
    return;
  }

  optval = FLAGS_keepalive_interval;
  if (setsockopt(client_fd, SOL_TCP, TCP_KEEPINTVL, &optval, optlen) < 0) {
    LOG(ERROR) << "set TCP_KEEPINTVL on client fd=" << client_fd << " failed";
    return;
  }

  optval = FLAGS_keepalive_probes;
  if (setsockopt(client_fd, SOL_TCP, TCP_KEEPCNT, &optval, optlen) < 0) {
    LOG(ERROR) << "set TCP_KEEPCNT on client fd=" << client_fd << " failed";
    return;
  }
  optval = 1;
  if (setsockopt(client_fd, IPPROTO_TCP, TCP_NODELAY, &optval,
                 sizeof(optval)) < 0) {
    LOG(ERROR) << "set TCP_NODELAY on client fd=" << client_fd << " failed";
    return;
  }
  bufferevent * bev =
    bufferevent_socket_new(evbase, client_fd, BEV_OPT_THREADSAFE);
  if (bev == NULL) {
    LOG(ERROR) << "create bufferevent on client fd=" << client_fd << " failed";
    close(client_fd);
    return;
  }

  bufferevent_setcb(bev, DoRead, NULL, DoError, server);
  if (bufferevent_enable(bev, EV_READ | EV_WRITE) != 0) {
    LOG(ERROR) << "enable bufferevent on client fd=" << client_fd << " failed";
    bufferevent_free(bev);
    return;
  }
  server->AddFd(client_fd, inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));
}


void BaseServer::Start() {
  listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  CHECK_GE(listen_fd_, 0) << "create fd error!";
  CHECK_EQ(evutil_make_socket_nonblocking(listen_fd_), 0)
    << "set linsten sock non block error!";
  int optval = 1;
  setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  struct sockaddr_in listen_addr;
  bzero(&listen_addr, sizeof(listen_addr));
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_addr.s_addr = 0;
  listen_addr.sin_port = htons(port_);
  CHECK_EQ(bind(listen_fd_, (struct sockaddr *) &listen_addr,
        sizeof(listen_addr)), 0)
    << "bind listen sock error, start listen failed";
  CHECK_EQ(listen(listen_fd_, FLAGS_max_conns), 0)
    << "begin listen sock error, start listen failed";

  CHECK_EQ(evthread_use_pthreads(), 0)
    << "enable pthreads for libevent failed!";
  evbase_ = event_base_new();
  CHECK(evbase_ != NULL) << "new base event error!";
  event * evlisten = event_new(evbase_, listen_fd_, EV_READ | EV_PERSIST,
                               DoAccept, this);
  event_add(evlisten, NULL);

  for (int i = 0; i < worker_.size(); i++) {
    worker_[i]->Start();
  }

  event_base_dispatch(evbase_);
  for (int i = 0; i < worker_.size(); i++) {
    worker_[i]->Join();
  }
}

BaseServer::BaseServer(int port, base::shared_ptr<BaseRouter> router,
                       int size) {
  port_ = port;
  router_ = router;
  evbase_ = NULL;
  for (int i = 0; i < size; i++) {
    worker_.push_back(base::shared_ptr<Worker>(new Worker(this)));
  }
}

bool BaseServer::Read(base::shared_ptr<Node> node) {
  struct evbuffer * input = bufferevent_get_input(node->bev);
  CHECK(input != NULL)
      << "Read " << node->ip << " " << node->port << " error";
  if (input == NULL) {  // not enough data
    LOG(INFO) << "not enough data, input == NULL";
    return false;
  }

  unsigned char * buff = evbuffer_pullup(input, 4);
  if (buff == NULL) {
    LOG(INFO) << "not enough data, buff == NULL";
    return false;
  }

  uint32_t size = *(reinterpret_cast<uint32_t*>(buff));
  int real_size = evbuffer_get_length(input);
  int total_size = size + 4;

  if (real_size < total_size) {  // not enough data
    LOG(INFO) << "not enough data, real_size = "
              << real_size << " total_size = " << total_size;
    return false;
  } else {
    node->content->resize(total_size);
    int cnt = evbuffer_remove(input,
        const_cast<char *>(node->content->data()), total_size);
    CHECK(cnt == total_size) << cnt << " " << total_size;
    return true;
  }
}

bool BaseServer::Send(base::shared_ptr<Node> node) {
  int rs = bufferevent_write(node->bev, node->content->data(),
      node->content->size());
  return rs == 0;
}

}  // namespace base_server
