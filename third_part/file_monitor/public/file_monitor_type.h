// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_FILE_MONITOR_TYPE_H_
#define UTIL_FILE_MONITOR_FILE_MONITOR_TYPE_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include "base/public/time.h"
#include "base/public/callback.h"
#include "base/public/shared_ptr.h"
#include "third_part/file_monitor/public/file_monitor_common.h"

namespace util {
// forward delcaration
struct RegisterNode;
struct EventParam;
struct FileInfoValue;
struct MCallbackValueNode;
struct MCallbackNode;
typedef std::vector<base::shared_ptr<base::Closure> > CallbackSet;
typedef std::list<EventParam> MergedEventList;
typedef std::pair<base::TimeTicks, EventParam> EventEntry;
typedef std::list<EventEntry> EventList;
typedef std::map<int, RegisterNode> WDMap;
typedef std::map<std::string, FileInfoValue> ContentMap;
typedef std::map<std::string, ContentMap> FileMap;
typedef std::map<std::string, CallbackSet> DecedentMap;
typedef std::map<int, DecedentMap> DelegateMap;
typedef std::map<int, MCallbackValueNode> CallbackMap;
typedef std::map<int, std::string> Md5MappingMap;
typedef std::map<std::string, Md5MappingMap> Md5CalculatingMap;

struct EventParam {
  int wd;
  std::string fullpath;
  WatchEventType type;
};

struct RegisterNode {
  std::string path;
  WatchType type;
  CallbackSet callbackset;
};

struct FileInfoValue {
  FileInfoValue() : md5value(""), filesize(0) {}
  std::string md5value;
  size_t filesize;
};
struct MCallbackValueNode {
  std::string path;
  WatchEventType  eventtype;
  CallbackSet callbackset;
  base::TimeTicks tick;
};
struct MCallbackNode {
  int wd;
  MCallbackValueNode value;
};
}

#endif  // UTIL_FILE_MONITOR_FILE_MONITOR_TYPE_H_
