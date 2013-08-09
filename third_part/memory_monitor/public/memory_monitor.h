// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: yangzhang@yunrang.com (yangzhang)

#ifndef UTIL_MEMORY_MONITOR_MEMORY_MONITOR_H_
#define UTIL_MEMORY_MONITOR_MEMORY_MONITOR_H_
#include <map>
#include <string>
#include <vector>

#include "base/public/callback.h"
#include "base/public/scoped_ptr.h"
#include "third_part/string/public/string_enum.h"
#include "third_part/libproc/include/readproc.h"

namespace util {
namespace memory_monitor {
enum MemoryMonitorMode {
  kRss = 0,
  kRssIncrease = 1,
  kVSize = 2,
  kVSizeIncrease = 3,
};

struct RegistMonitorInfo {
  RegistMonitorInfo(
      MemoryMonitorMode md,
      base::ResultCallback2<bool, int64, std::string*>* cb,
      int64 value) {
    mode = md;
    call_back = cb;
    threshold = value;
  }
  MemoryMonitorMode mode;
  base::ResultCallback2<bool, int64, std::string*> *call_back;
  int64 threshold;
};

class MemoryMonitor {
 public:
  MemoryMonitor();
  virtual ~MemoryMonitor();
  bool SetMonitor(MemoryMonitorMode mode, int64 threshold);
  bool CheckProcStat(std::vector<std::string>* alarm_info);

  // Get Process info
  int64 GetRssMemory();
  int64 GetIncRssMemory();
  int64 GetVsizeMemory();
  int64 GetIncVsizeMemory();
  proc_t GetProcInfo();

 private:
  bool RssCheck(int64 threshold, std::string* alarm_message);
  bool RssIncreaseCheck(int64 threshold, std::string* alarm_message);
  bool VSizeCheck(int64 threshold, std::string* alarm_message);
  bool VSizeIncreaseCheck(int64 threshold, std::string* alarm_message);

 private:
  // Process info
  proc_t proc_info_;
  proc_t previous_proc_info_;

  // MemoryMonitorMode-Callback map
  std::map<MemoryMonitorMode,
      base::ResultCallback2<bool, int64, std::string*>*>
      callback_map_info_;

  // Registed type
  std::vector<RegistMonitorInfo> monitor_container_;

  // EnumToString parser
  util::StringEnumParser<util::memory_monitor::MemoryMonitorMode> enum_parser_;

  DISALLOW_COPY_AND_ASSIGN(MemoryMonitor);
};
}
}
#endif  // UTIL_MEMORY_MONITOR_MEMORY_MONITOR_H_
