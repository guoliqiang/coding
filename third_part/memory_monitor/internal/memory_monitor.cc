// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: yangzhang@yunrang.com (yangzhang)

#include "../public/memory_monitor.h"
#include <algorithm>

#include "base/public/string_util.h"
#include "third_part/gtl/public/stl_util-inl.h"

BEGIN_STRINGMAP_PARSER(util::memory_monitor::MemoryMonitorMode)
  STRINGMAP_ENUM(util::memory_monitor::kRss)
  STRINGMAP_ENUM(util::memory_monitor::kRssIncrease)
  STRINGMAP_ENUM(util::memory_monitor::kVSize)
  STRINGMAP_ENUM(util::memory_monitor::kVSizeIncrease)
END_STRINGMAP_PARSER()

namespace util {
namespace memory_monitor {

using namespace base; // NOLINT
using namespace std; // NOLINT

MemoryMonitor::MemoryMonitor() {
  ResultCallback2<bool, int64, std::string*> *call_back;
  call_back =
      NewPermanentCallback(this, &MemoryMonitor::RssCheck);
  callback_map_info_.insert(make_pair(kRss, call_back));
  call_back =
      NewPermanentCallback(this, &MemoryMonitor::RssIncreaseCheck);
  callback_map_info_.insert(make_pair(kRssIncrease, call_back));
  call_back =
      NewPermanentCallback(this, &MemoryMonitor::VSizeCheck);
  callback_map_info_.insert(make_pair(kVSize, call_back));
  call_back =
      NewPermanentCallback(this, &MemoryMonitor::VSizeIncreaseCheck);
  callback_map_info_.insert(make_pair(kVSizeIncrease, call_back));

  look_up_our_self(&proc_info_);
  previous_proc_info_ = proc_info_;
}

MemoryMonitor::~MemoryMonitor() {
  gtl::STLDeleteValues(&callback_map_info_);
}

int64 MemoryMonitor::GetRssMemory() {
  return proc_info_.rss * 4;
}

int64 MemoryMonitor::GetIncRssMemory() {
  return (proc_info_.rss - previous_proc_info_.rss) * 4;
}

int64 MemoryMonitor::GetVsizeMemory() {
  return proc_info_.vsize;
}

int64 MemoryMonitor::GetIncVsizeMemory() {
  return proc_info_.vsize - previous_proc_info_.vsize;
}

proc_t MemoryMonitor::GetProcInfo() {
  return proc_info_;
}

bool MemoryMonitor::RssCheck(int64 threshold, string* alarm_message) {
  int64 cur_rss =
    static_cast<int64>(proc_info_.rss * 4);
  // 4KB for one page
  if (cur_rss < threshold) {
    return false;
  }
  CHECK(enum_parser_.Enum2String(kRss, alarm_message));
  alarm_message->append(" The Rss size is ");
  alarm_message->append(Int64ToString(cur_rss));
  return true;
}

bool MemoryMonitor::RssIncreaseCheck(int64 threshold, string* alarm_message) {
  int64 increment =
    static_cast<int64>((proc_info_.rss - previous_proc_info_.rss) * 4);
  if (increment < threshold) {
    return false;
  }
  CHECK(enum_parser_.Enum2String(kRssIncrease, alarm_message));
  alarm_message->append(" The Rss Increasement is ");
  alarm_message->append(Int64ToString(increment));
  return true;
}

bool MemoryMonitor::VSizeCheck(int64 threshold, string* alarm_message) {
  if (proc_info_.vsize < threshold) {
    return false;
  }
  CHECK(enum_parser_.Enum2String(kVSize, alarm_message));
  alarm_message->append(" The VM size is More Than ");
  alarm_message->append(Uint64ToString(threshold));
  return true;
}

bool MemoryMonitor::VSizeIncreaseCheck(int64 threshold, string* alarm_message) {
  if (proc_info_.vsize - previous_proc_info_.vsize < threshold) {
    return false;
  }
  CHECK(enum_parser_.Enum2String(kVSizeIncrease, alarm_message));
  alarm_message->append(" The VM Increase is More Than ");
  alarm_message->append(Uint64ToString(threshold));
  return true;
}


bool MemoryMonitor::SetMonitor(MemoryMonitorMode mode, int64 threshold) {
  CHECK_GT(threshold, 0) << "We just support the positive value now";
  ResultCallback2<bool, int64, std::string*>* call_back =
      callback_map_info_[mode];
  CHECK(call_back) << "Please Regist the Call_back First";
  RegistMonitorInfo regist_monitor(mode, call_back, threshold);
  monitor_container_.push_back(regist_monitor);
  return true;
}

bool MemoryMonitor::CheckProcStat(vector<string>* alarm_info) {
  CHECK(alarm_info);
  alarm_info->clear();
  previous_proc_info_ = proc_info_;
  look_up_our_self(&proc_info_);
  string msg;
  for (int i = 0; i < monitor_container_.size(); ++i) {
    RegistMonitorInfo& regist_monitor = monitor_container_[i];
    if (regist_monitor.call_back->Run(
            regist_monitor.threshold,
            &msg)) {
      alarm_info->push_back(msg);
    }
  }
  return !alarm_info->empty();
}
}
}
