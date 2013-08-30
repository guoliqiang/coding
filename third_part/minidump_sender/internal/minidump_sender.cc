// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author: bingbai@yunrang.com (Bing Bai)

#include "third_part/minidump_sender/public/minidump_sender.h"
#include <map>
#include <vector>
#include "base/public/string_util.h"
#include "base/public/scoped_ptr.h"
#include "file/public/file_base.h"
#include "file/public/file.h"
#include "base/public/env.h"
#include "third_part/breakpad/client/linux/handler/exception_handler.h"
#include "third_part/breakpad/common/linux/http_upload.h"

DEFINE_string(crashpad_http_server,
              "http://crashpad:8421",
              "http crashpad_server, e.g.: http://10.100.16.78:8080");

DEFINE_string(dumps_path, "/tmp/dumps",
              "The path to generate mini dump");

DEFINE_bool(send_minidump, true,
            "true to send minidump to crashpad");

DEFINE_bool(disable_minidump, false,
            "true to disable minidump, back to normal full dump");

using google_breakpad::ExceptionHandler;

namespace util {
const std::string DefaultValue = "unavailable";
const std::string ParamSeparator = "```";
const std::string KeyValueSeparator = "`:`";
const std::string FirstParamFormatter = StringPrintf("%%s%s%%s",
                                                   KeyValueSeparator.c_str());
const std::string NonFirstParamFormatter = StringPrintf("%s%%s%s%%s",
                                                   ParamSeparator.c_str(),
                                                   KeyValueSeparator.c_str());

MiniDumpSender::MiniDumpSender() {
  memset(param_series_, '\0', sizeof(param_series_));
  memset(crashpad_http_server_, '\0', sizeof(crashpad_http_server_));
  memset(trace_log_path_, '\0', sizeof(trace_log_path_));
  initialized_ = false;
  hooked_ = false;
}

MiniDumpSender* MiniDumpSender::Sender() {
  return Singleton<MiniDumpSender>::get();
}

MiniDumpSender::~MiniDumpSender() {}

void MiniDumpSender::Hook() {
  if (FLAGS_disable_minidump) {
    LOG(INFO) << "minidump is disabled, no minidump will be generated. "
              << "Full core dump used";
    return;
  }
  if (hooked_) {
    return;
  }
  Init();
  // it is safe to not release this object since it is a
  // global object.
  new ExceptionHandler(FLAGS_dumps_path,
                       NULL,
                       MiniDumpSender::DumpCallback,
                       NULL,
                       true);
  hooked_ = true;
}

void MiniDumpSender::Init() {
  if (initialized_) {
    return;
  }
  InitBinaryAndMachineInfo();
  InitBuildParam();
  initialized_ = true;
}

bool MiniDumpSender::DumpCallback(const char* dump_path,
                                  const char* minidump_id,
                                  void* context,
                                  bool succeeded) {
  std::string dump_fullpath = StringPrintf("%s/%s.dmp",
                                           dump_path,
                                           minidump_id);
  LOG(ERROR) << "Crash was catched, and mini dump was generated at "
             << dump_fullpath;
  if (succeeded && FLAGS_send_minidump) {
    util::MiniDumpSender::Sender()->Complete(dump_fullpath);
  }
  if (succeeded) {
    // if minidump generated successfully, we could exit the process
    // now. Otherwise the SIGSEGV will caused the default handler -
    // full core dump.
    // if minidump failed to generate, we let it continue to able to
    // generate full core dump
    _exit(1);
  }
  return succeeded;
}


void MiniDumpSender::InitBinaryAndMachineInfo() {
  strncat(crashpad_http_server_,
          FLAGS_crashpad_http_server.c_str(),
          sizeof(crashpad_http_server_) - 1);
  std::string binary_name = base::ProgramInvocationShortName();
  AppendParam("binary_name", binary_name);
  // std::string trace_log_path = StringPrintf("%s/%s.log",
  //                               release::GetTraceLogPath().c_str(),
  //                               binary_name.c_str());
  // strncat(trace_log_path_,
  //         trace_log_path.c_str(),
  //         sizeof(trace_log_path_) - 1);
  // std::string binary_version(MAJOR_BUILD_NUMBER "." MINOR_BUILD_NUMBER);
  // AppendParam("binary_version", binary_version);
  // std::string machine_name = util::host_util::LocalHostName();
  // AppendParam("machine_name", machine_name);
  // std::string binary_path(GetBinaryPath());
  // AppendParam("binary_path", binary_path);
}

std::string MiniDumpSender::GetBinaryPath() {
  char buffer[PathBufMaxSize];
  std::string binary_path(DefaultValue);
  buffer[PathBufMaxSize -1] = '\0';
  int ret = readlink("/proc/self/exe", buffer, PathBufMaxSize-1);
  if (ret < 0 || ret > (PathBufMaxSize - 1)) {
    LOG(ERROR) << "error in geting binary path, ret: " << ret;
  } else {
    buffer[ret] = '\0';
    binary_path = StringPrintf("%s", buffer);
  }
  return binary_path;
}

void MiniDumpSender::InitBuildParam() {
  std::string build_time(DefaultValue);
  base::GetBuildingEnv("time", &build_time);
  AppendParam("build_time", build_time);

  std::string build_host(DefaultValue);
  base::GetBuildingEnv("host", &build_host);
  AppendParam("build_host", build_host);

  std::string build_compiler(DefaultValue);
  base::GetBuildingEnv("compiler", &build_compiler);
  AppendParam("build_compiler", build_compiler);

  std::string build_mode(DefaultValue);
  base::GetBuildingEnv("mode", &build_mode);
  AppendParam("build_mode", build_mode);

  std::string build_platform(DefaultValue);
  base::GetBuildingEnv("platform", &build_platform);
  AppendParam("build_platform", build_platform);

  std::string build_svn_base(DefaultValue);
  base::GetBuildingEnv("svn base", &build_svn_base);
  AppendParam("build_svn_base", build_svn_base);

  std::string build_svn_rev(DefaultValue);
  base::GetBuildingEnv("svn rev", &build_svn_rev);
  AppendParam("build_svn_rev", build_svn_rev);
}

void MiniDumpSender::ConstructCrashParam(const std::string& dump_path) {
  base::Time now = base::Time::Now();
  base::Time::Exploded exploded;
  now.UTCExplode(&exploded);
  std::string crashed_time = StringPrintf("%d/%02d/%02d %02d:%02d:%02d",
                                           exploded.year,
                                           exploded.month,
                                           exploded.day_of_month,
                                           exploded.hour,
                                           exploded.minute,
                                           exploded.second);
  AppendParam("crashed_time", crashed_time);
  std::string dump_name = dump_path.substr(dump_path.find_last_of('/') + 1,
                                           dump_path.size());
  AppendParam("dump_name", dump_name);
  AppendFinalSegmentOfTraceLog();
}

void MiniDumpSender::AppendFinalSegmentOfTraceLog() {
  std::string content(DefaultValue);
  std::string trace_log_path(trace_log_path_);
  size_t file_size(0);
  if (file::FileBase::Exists(trace_log_path)
      && file::File::FileSize(trace_log_path, &file_size)) {
    file::FileBase::size_type cursor
         = (file_size < CrashpadTracelogSegmentSize ?
            file_size : CrashpadTracelogSegmentSize);
    scoped_ptr<file::FileBase> file;
    file.reset(file::FileBase::Open(trace_log_path, file::FileBase::kRead));
    if (file.get() != NULL
        && file->Seek(-cursor, file::FileBase::kSeekEnd).ok()
        && file->Read(cursor, &content).ok()) {
      LOG(INFO) << "retrieving tracelog snippet successfully";
    } else {
      LOG(ERROR) << "error in retrieving tracelog snippet";
    }
  } else {
    LOG(ERROR) << "error in getting tracelog size";
  }
  AppendParam("tracelog_final_segment", content);
}

// construct crash info, and send minidump and parameters
bool MiniDumpSender::Complete(const std::string& dump_path) {
  bool ret(false);
  std::string response;
  std::string error;
  std::string crashpad_http_server(crashpad_http_server_);
  if (!crashpad_http_server.empty()) {
    std::map<std::string, std::string> param_map;
    ConstructCrashParam(dump_path);
    std::vector<std::string> params;
    SplitStringUsingSubstr(std::string(param_series_), ParamSeparator, &params);
    for (int i = 0; i < params.size(); i++) {
      std::vector<std::string> key_value;
      SplitStringUsingSubstr(params[i], KeyValueSeparator, &key_value);
      if (key_value.size() > 1 && !key_value[0].empty()) {
        param_map[key_value[0]] = key_value[1];
      }
    }
    ret =  google_breakpad::HTTPUpload::SendRequest(crashpad_http_server,
                                                    param_map,
                                                    dump_path,
                                                    "minidump",
                                                    "",
                                                    "",
                                                    "",
                                                    &response,
                                                    &error);
  }
  if (ret) {
    LOG(INFO) << "mini dump " << dump_path
              << " is sent to " << crashpad_http_server
              << " successfully";
  } else {
    LOG(ERROR) << "fail to send mini dump " << dump_path
               << " to " << crashpad_http_server
               << "\nresponse: " << response
               << "\nerror: " << error;
  }
  return ret;
}

void MiniDumpSender::AppendParam(const std::string& key,
                                 const std::string& value) {
  if (strlen(param_series_) == 0) {
    const char* formatter_first = FirstParamFormatter.c_str();
    snprintf(param_series_,
             sizeof(param_series_),
             formatter_first,
             key.c_str(),
             value.c_str());
  } else {
    const char * formatter_not_first = NonFirstParamFormatter.c_str();
    snprintf(param_series_ + strlen(param_series_),
             sizeof(param_series_) - strlen(param_series_),
             formatter_not_first,
             key.c_str(),
             value.c_str());
  }
}
}
