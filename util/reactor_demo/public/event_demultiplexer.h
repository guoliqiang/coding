#ifndef EVENT_DEMULTIPLEXER_H_
#define EVENT_DEMULTIPLEXER_H_

#include <map>
#include "common.h"

namespace reactor {

class EventHandler;

class EventDemultiplexer {
 public:
  virtual ~EventDemultiplexer() {}
  virtual int WaitEvents(std::map<handle_t, EventHandler *> * handlers,
                         int timeout = 0) = 0;
  virtual int RequestEvent(handle_t handle, event_t evt) = 0;
  virtual int UnrequestEvent(handle_t handle) = 0;
};

} // namespace reactor

#endif // EVENT_DEMULTIPLEXER_H_
