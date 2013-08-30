// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/global_init.h"

#include <string>
#include <utility>
#include <vector>

#include "base/public/affinity.h"
#include "base/public/env.h"
#include "base/public/flags.h"
#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "third_part/daemonizer/public/daemonizer.h"
#include "third_part/release/public/release_flag.h"
#include "third_part/minidump_sender/public/minidump_sender.h"
#include "third_part/production/public/production_safety_checker.h"
#include "develop/building_info.h"

using namespace base;  // NOLINT
using std::string;
using std::vector;

namespace {


void AddBuildingEnvs() {
  if (strlen(BUILD_TIME) != 0)
    SetBuildingEnv("time", BUILD_TIME);
  if (strlen(BUILD_HOST) != 0)
    SetBuildingEnv("host", BUILD_HOST);
  if (strlen(BUILD_COMPILER) != 0)
    SetBuildingEnv("compiler", BUILD_COMPILER);
  if (strlen(BUILD_MODE) != 0)
    SetBuildingEnv("mode", BUILD_MODE);
  if (strlen(BUILD_PLATFORM) != 0)
    SetBuildingEnv("platform", BUILD_PLATFORM);

  // extract the target path from targets map
  vector<string> targets;
  SplitString(BUILD_TARGET, ',', &targets);
  string target = ProgramInvocationShortName();
  for (int i = 0; i < targets.size(); ++i) {
    string::size_type idx = targets[i].find(":");
    if (idx == string::npos) continue;
    if (target == targets[i].substr(0, idx)) {
      target = targets[i].substr(idx + 1);
      break;
    }
  }
  SetBuildingEnv("binary", ProgramInvocationShortName());
  SetBuildingEnv("target", target);
}

void PrintBuildEnvs() {
  LOG(INFO) << "Building environment:\n" << GetVersionInfo();
}

}  // namespace

namespace util {
GlobalInit::GlobalInit(int *argc, char ***argv) {
  release::ParseFlags(argc, argv);
  AddBuildingEnvs();
  HandleCommandLineHelpFlags();
  if (!ProductionSafetyChecker::CheckUser()) {
    LOG(ERROR) << "check production user failed, exit...";
    exit(1);
  }
  util_daemonizer::daemonize();
  util::MiniDumpSender::Sender()->Hook();
  PrintBuildEnvs();
  base::ApplyAffinitySetting();
}
}  // namespace util
