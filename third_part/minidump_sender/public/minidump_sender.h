// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author: bingbai@yunrang.com (Bing Bai)
//
#ifndef UTIL_MINIDUMP_SENDER_MINIDUMP_SENDER_H_
#define UTIL_MINIDUMP_SENDER_MINIDUMP_SENDER_H_
#include <string>
#include "base/public/singleton.h"

namespace util {
const uint32 CrashpadTracelogSegmentSize = 10240;
const uint32 PathBufMaxSize = 1024;
const uint32 ParamSeriesSize
      = CrashpadTracelogSegmentSize + 255 * 12 + PathBufMaxSize;
const uint32 CrashPadUrlMaxSize = 1024;

class MiniDumpSender {
 public:
  MiniDumpSender();
  ~MiniDumpSender();
  bool Complete(const std::string& dump_path);
  friend struct DefaultSingletonTraits<MiniDumpSender>;
  static MiniDumpSender* Sender();
  void Init();
  void Hook();
 
 private:
  static bool DumpCallback(const char* dump_path,
                           const char* minidump_id,
                           void* context,
                           bool succeeded);
  void InitBuildParam();
  void InitBinaryAndMachineInfo();
  std::string GetBinaryPath();
  void ConstructCrashParam(const std::string& dump_path);
  void AppendFinalSegmentOfTraceLog();
  void AppendParam(const std::string& key, const std::string& value);
 
 private:
  char crashpad_http_server_[CrashPadUrlMaxSize];
  char trace_log_path_[PATH_MAX];
  char param_series_[CrashpadTracelogSegmentSize];
  bool initialized_;
  bool hooked_;
  DISALLOW_COPY_AND_ASSIGN(MiniDumpSender);
};

}
#endif  // UTIL_MINIDUMP_SENDER_MINIDUMP_SENDER_H_
