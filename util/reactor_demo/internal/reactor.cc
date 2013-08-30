// Copyright 2013

#include <assert.h>
#include "../public/reactor.h"
#include "../public/event_handler.h"
#include "../public/epoll_demultiplexer.h"
#include "base/public/logging.h"

namespace reactor {

class ReactorImplementation {
 public:
  ReactorImplementation();
  ~ReactorImplementation();

  int RegisterHandler(EventHandler * handler, event_t evt);
  int RemoveHandler(EventHandler * handler);
  void HandleEvents(int timeout);

 private:
  EventDemultiplexer * demultiplexer_;
  std::map<handle_t, EventHandler *>  handlers_;
};

Reactor::Reactor() {
  reactor_impl_ = new ReactorImplementation();
}

Reactor::~Reactor() {
  delete reactor_impl_;
}

int Reactor::RegisterHandler(EventHandler * handler, event_t evt) {
  return reactor_impl_->RegisterHandler(handler, evt);
}

int Reactor::RemoveHandler(EventHandler * handler) {
  return reactor_impl_->RemoveHandler(handler);
}

void Reactor::HandleEvents(int timeout) {
  VLOG(3) << "handle events";
  reactor_impl_->HandleEvents(timeout);
}



ReactorImplementation::ReactorImplementation() {
  demultiplexer_ = new EpollDemultiplexer();
}

ReactorImplementation::~ReactorImplementation() {
  delete demultiplexer_;
}

int ReactorImplementation::RegisterHandler(EventHandler * handler,
                                           event_t evt) {
  handle_t handle = handler->GetHandle();
  VLOG(3) << "register handler:" << handle << " event:" << evt;
  std::map<handle_t, EventHandler *>::iterator it = handlers_.find(handle);
  if (it == handlers_.end()) {
    handlers_[handle] = handler;
  }
  return demultiplexer_->RequestEvent(handle, evt);
}

int ReactorImplementation::RemoveHandler(EventHandler * handler) {
  handle_t handle = handler->GetHandle();
  VLOG(3) << "remove handler:" << handle;
  handlers_.erase(handle);
  return demultiplexer_->UnrequestEvent(handle);
}

void ReactorImplementation::HandleEvents(int timeout) {
  VLOG(3) << "handler event, timeout:" << timeout;
  demultiplexer_->WaitEvents(&handlers_);
}

}  // namespace reactor
