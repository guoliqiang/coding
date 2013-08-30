#ifndef  EVENT_HANDLER_H_
#define  EVENT_HANDLER_H_

#include "common.h"

namespace reactor {

class EventHandler {
 public:
  virtual handle_t GetHandle() const = 0;
  virtual void HandleRead() {}
  virtual void HandleWrite() {}
  virtual void HandleError() {}  
 
 protected:
  EventHandler() {}
  virtual ~EventHandler() {}
};
}  // namespace reactor

#endif  // EVENT_HANDLER_H_
