// Copyright 2013


#include <errno.h>
#include <sys/epoll.h>
#include <vector>
#include "../public/epoll_demultiplexer.h"
#include "../public/event_handler.h"
#include "base/public/logging.h"

namespace reactor {

EpollDemultiplexer::EpollDemultiplexer() {
  VLOG(3) << "epoll_create";
  epoll_fd_ = ::epoll_create(FD_SETSIZE);
  CHECK_NE(epoll_fd_, -1);
  fd_num_ = 0;
}

EpollDemultiplexer::~EpollDemultiplexer() {
  ::close(epoll_fd_);
}

int EpollDemultiplexer::WaitEvents(std::map<handle_t, EventHandler *> * handlers,  // NOLINT
                                   int timeout) {
  std::vector<epoll_event> ep_evts(fd_num_);
  int num = epoll_wait(epoll_fd_, &ep_evts[0], ep_evts.size(), timeout);
  VLOG(3) << "epoll_wait return:" << num;
  if (num > 0) {
    for (int idx = 0; idx < num; ++idx) {
      handle_t handle = ep_evts[idx].data.fd;
      CHECK(handlers->find(handle) != handlers->end());
      if ((ep_evts[idx].events & EPOLLERR) ||
          (ep_evts[idx].events & EPOLLHUP)) {
        (*handlers)[handle]->HandleError();
      } else {
        if (ep_evts[idx].events & EPOLLIN) {
          (*handlers)[handle]->HandleRead();
        }
        if (ep_evts[idx].events & EPOLLOUT) {
          (*handlers)[handle]->HandleWrite();
        }
      }
    }
  }
  return num;
}

int EpollDemultiplexer::RequestEvent(handle_t handle, event_t evt) {
  epoll_event ep_evt;
  ep_evt.data.fd = handle;
  ep_evt.events = 0;
  if (evt & kReadEvent) {
    ep_evt.events |= EPOLLIN;
  }
  if (evt & kWriteEvent) {
    ep_evt.events |= EPOLLOUT;
  }
  ep_evt.events |= EPOLLONESHOT;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, handle, &ep_evt) != 0) {
    if (errno == ENOENT) {
      if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, handle, &ep_evt) != 0) {
        return -errno;
      }
      VLOG(3) << "add handle:" << handle << " event:" << evt;
      ++fd_num_;
    }
  } else {
    VLOG(3) << "modified handler:" << handle << " event:" << evt;
  }
  return 0;
}

int EpollDemultiplexer::UnrequestEvent(handle_t handle) {
  epoll_event ep_evt;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, handle, &ep_evt) != 0) {
    return -errno;
  }
  VLOG(3) << "delete handle:" << handle;
  --fd_num_;
  return 0;
}

}  // namespace reactor
