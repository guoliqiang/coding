#ifndef REACTOR_REACTOR_H_
#define REACTOR_REACTOR_H_

#include <stdint.h>
#include <unistd.h>
#include <sys/epoll.h>
#include "common.h"
#include "base/public/logging.h"

namespace reactor {

class ReactorImplementation;
class EventHandler;

class Reactor {
 public:
  Reactor();
  ~Reactor();

  int RegisterHandler(EventHandler * handler, event_t evt);
  int RemoveHandler(EventHandler * handler);
  void HandleEvents(int timeout = 0);

 private:
  ReactorImplementation * reactor_impl_;
  DISALLOW_COPY_AND_ASSIGN(Reactor);
};

} // namespace reactor

#endif // REACTOR_REACTOR_H_
