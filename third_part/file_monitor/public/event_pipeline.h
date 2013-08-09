// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)
//

#ifndef UTIL_FILE_MONITOR_EVENT_PIPELINE_H_
#define UTIL_FILE_MONITOR_EVENT_PIPELINE_H_

#include "base/public/thread.h"
#include "base/public/scoped_ptr.h"
#include "third_part/file_monitor/public/file_monitor_type.h"

namespace base {
  class Mutex;
}
namespace util {
class FileMonitor;
class InotifyEventPipeline : public base::Thread {
 public:
  explicit InotifyEventPipeline(FileMonitor* monitor);
  ~InotifyEventPipeline();
  void StopRunning();
  void Push(const struct EventParam& event);
 protected:
  void Run();
 private:
  void Extract(const EventParam& event, MergedEventList* eventset);
  void Reduce(MergedEventList* list);
  bool IsCoolingLongEnough(int64 second, const EventEntry& entry);
  bool GetFrontOfEventList(EventEntry* entry);
  bool IsEventListEmpty();
  void Pop();
 private:
  EventList               event_list_;
  scoped_ptr<base::Mutex> mutex_for_queue_;
  FileMonitor*            monitor_;
  ThreadStatus            status_;
  DISALLOW_COPY_AND_ASSIGN(InotifyEventPipeline);
};
}
#endif  // UTIL_FILE_MONITOR_EVENT_PIPELINE_H_
