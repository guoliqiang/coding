#ifndef COMMON_H_
#define COMMON_H_

namespace reactor {
  // event type
  typedef unsigned int event_t;
  // handle type
  typedef int handle_t;

  enum {
    kReadEvent    = 0x01, // read event
    kWriteEvent   = 0x02, // write event
    kErrorEvent   = 0x04, // error event
    kEventMask    = 0xff  // event mask code
  };

}

#endif // REACTOR_TEST_COMMON_H_
