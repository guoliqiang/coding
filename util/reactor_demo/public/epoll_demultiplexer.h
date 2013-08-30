#ifndef  EPOLL_DEMULTIPLEXER_H_
#define  EPOLL_DEMULTIPLEXER_H_

#include "common.h"
#include "event_demultiplexer.h"

namespace reactor {

class EventHandler;

class EpollDemultiplexer : public EventDemultiplexer {
 public:
  EpollDemultiplexer();
  ~EpollDemultiplexer();
  
  virtual int WaitEvents(std::map<handle_t, EventHandler *> * handlers,
                         int timeout = 0); 
  virtual int RequestEvent(handle_t handle, event_t evt);
  virtual int UnrequestEvent(handle_t handle);

 private:
   int  epoll_fd_;  // the fd of epoll
   int  fd_num_;    // the number of fd registed in epoll
};

}  // namespace reactor

#endif  // EPOOL_DEMULTIPLEXER_H_
