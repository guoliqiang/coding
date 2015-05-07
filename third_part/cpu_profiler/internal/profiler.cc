// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// Copyright (c) 2005, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ---
// Author: Sanjay Ghemawat
//         Chris Demetriou (refactoring)
//
// Profile current program by sampling stack-trace every so often

#include "third_part/cpu_profiler/public/profiler.h"

#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ucontext.h>
#include <sys/time.h>
#include <string>
#include "third_part/cpu_profiler/public/getpc.h"
#include "third_part/cpu_profiler/public/stacktrace.h"
#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/symbolize.h"
#include "base/public/string_util.h"
#include "third_part/cpu_profiler/public/profiledata.h"
#include "third_part/cpu_profiler/public/profile-handler.h"

using std::string;
using base::SpinLock;
using base::SpinLockHolder;

DEFINE_string(cpu_profiler_path, "cpu_profiler.prof", "");
DEFINE_string(cpu_profiler_symbol_path, "cpu_profiler.symbol", "");
DEFINE_int32(cpu_profiler_signal, 12, "");
DEFINE_bool(cpu_profiler_debug, false, "");

// Collects up all profile data. This is a singleton, which is
// initialized by a constructor at startup. If no cpu profiler
// signal is specified then the profiler lifecycle is either
// manaully controlled via the API or attached to the scope of
// the singleton (program scope). Otherwise the cpu toggle is
// used to allow for user selectable control via signal generation.
// This is very useful for profiling a daemon process without
// having to start and stop the daemon or having to modify the
// source code to use the cpu profiler API.
class CpuProfiler {
 public:
  CpuProfiler();
  ~CpuProfiler();

  // Start profiler to write profile info into fname
  bool Start(const char* fname, const char * sname,
             const ProfilerOptions* options);
  // Stop profiling and write the data to disk.
  void Stop();
  // Write the data to disk (and continue profiling).
  void FlushTable();
  bool Enabled();
  void GetCurrentState(ProfilerState* state);
  static CpuProfiler instance_;

 private:
  // This lock implements the locking requirements described in the ProfileData
  // documentation, specifically:
  //
  // lock_ is held all over all collector_ method calls except for the 'Add'
  // call made from the signal handler, to protect against concurrent use of
  // collector_'s control routines. Code other than signal handler must
  // unregister the signal handler before calling any collector_ method.
  // 'Add' method in the collector is protected by a guarantee from
  // ProfileHandle that only one instance of prof_handler can run at a time.
  SpinLock lock_;
  ProfileData collector_;
  // Filter function and its argument, if any.  (NULL means include all
  // samples).  Set at start, read-only while running.  Written while holding
  // lock_, read and executed in the context of SIGPROF interrupt.
  int (*filter_)(void*);  // NOLINT
  void * filter_arg_;
  // Opaque token returned by the profile handler. To be used when calling
  // ProfileHandlerUnregisterCallback.
  ProfileHandlerToken * prof_handler_token_;
  // Sets up a callback to receive SIGPROF interrupt.
  void EnableHandler();
  // Disables receiving SIGPROF interrupt.
  void DisableHandler();
  // Signal handler that records the interrupted pc in the profile data.
  static void prof_handler(int sig, siginfo_t*, void* signal_ucontext,
                           void* cpu_profiler);
};

// Signal handler that is registered when a user selectable signal
// number is defined in the environment variable CPUPROFILESIGNAL.
static void CpuProfilerSwitch(int signal_number) {
  bool static started = false;
  static unsigned profile_count = 0;
  if (!started) {
    char full_profile_name[1024] = { 0 };
    char full_symbol_name[1024] = { 0 };

    snprintf(full_symbol_name, sizeof(full_symbol_name), "%s.%u",
             FLAGS_cpu_profiler_symbol_path.c_str(), profile_count);
    snprintf(full_profile_name, sizeof(full_profile_name), "%s.%u",
             FLAGS_cpu_profiler_path.c_str(), profile_count++);

    if (!ProfilerStart(full_profile_name, full_symbol_name)) {
      LOG(FATAL) << "Can't turn on cpu profiling for " << full_profile_name
                 << " " << full_symbol_name << " " << strerror(errno);
    }
  } else {
    ProfilerStop();
  }
  started = !started;
}

// Profile data structure singleton: Constructor will check to see if profiling
// should be enabled.  Destructor will write profile data out to disk.
CpuProfiler CpuProfiler::instance_;

CpuProfiler::CpuProfiler() : prof_handler_token_(NULL) {
  if (getuid() != geteuid()) {
    LOG(WARNING) << "Cannot perform CPU profiling when running with setuid";
    return;
  }

  int signal_number = FLAGS_cpu_profiler_signal;
  if (signal_number >= 1 && signal_number <= 64) {
    intptr_t old_signal_handler =
        reinterpret_cast<intptr_t>(signal(signal_number, CpuProfilerSwitch));
    if (old_signal_handler == 0) {
      LOG(INFO) << "Using signal as cpu profiling switch " << signal_number;
    } else {
      LOG(FATAL) << "Signal already in use" << signal_number;
    }
  } else {
    LOG(FATAL) << "Signal number is invalid" << signal_number;
  }
}

bool CpuProfiler::Start(const char* fname, const char * sname,
                        const ProfilerOptions* options) {
  // register main thread/processor
  ProfilerRegisterThread();

  SpinLockHolder cl(&lock_);
  if (collector_.enabled()) {
    LOG(WARNING) << "profile has enabled";
    return false;
  }
  ProfileHandlerState prof_handler_state;
  ProfileHandlerGetState(&prof_handler_state);

  ProfileData::Options collector_options;
  collector_options.set_frequency(prof_handler_state.frequency);
  if (!collector_.Start(fname, sname, collector_options)) {
    LOG(WARNING) << "start profiledata error";
    return false;
  }

  filter_ = NULL;
  if (options != NULL && options->filter_in_thread != NULL) {
    filter_ = options->filter_in_thread;
    filter_arg_ = options->filter_in_thread_arg;
  }
  // Setup handler for SIGPROF interrupts
  EnableHandler();
  return true;
}

CpuProfiler::~CpuProfiler() {
  Stop();
}

// Stop profiling and write out any collected profile data
void CpuProfiler::Stop() {
  SpinLockHolder cl(&lock_);
  if (!collector_.enabled()) return;
  // Unregister prof_handler to stop receiving SIGPROF interrupts before
  // stopping the collector.
  DisableHandler();
  // DisableHandler waits for the currently running callback to complete and
  // guarantees no future invocations. It is safe to stop the collector.
  collector_.Stop();
}

void CpuProfiler::FlushTable() {
  SpinLockHolder cl(&lock_);
  if (!collector_.enabled()) return;
  // Unregister prof_handler to stop receiving SIGPROF interrupts before
  // flushing the profile data.
  DisableHandler();
  // DisableHandler waits for the currently running callback to complete and
  // guarantees no future invocations. It is safe to flush the profile data.
  collector_.FlushTable();
  EnableHandler();
}

bool CpuProfiler::Enabled() {
  SpinLockHolder cl(&lock_);
  return collector_.enabled();
}

void CpuProfiler::GetCurrentState(ProfilerState* state) {
  ProfileData::State collector_state;
  {
    SpinLockHolder cl(&lock_);
    collector_.GetCurrentState(&collector_state);
  }
  state->enabled = collector_state.enabled;
  state->start_time = static_cast<time_t>(collector_state.start_time);
  state->samples_gathered = collector_state.samples_gathered;
  int buf_size = sizeof(state->profile_name);
  strncpy(state->profile_name, collector_state.profile_name, buf_size);
  state->profile_name[buf_size-1] = '\0';
}

void CpuProfiler::EnableHandler() {
  CHECK(prof_handler_token_ == NULL) << "SIGPROF handler already registered";
  prof_handler_token_ = ProfileHandlerRegisterCallback(prof_handler, this);
  CHECK(prof_handler_token_ != NULL) << "Failed to set up SIGPROF handler";
}

void CpuProfiler::DisableHandler() {
  CHECK(prof_handler_token_ != NULL) << "SIGPROF handler is not registered";
  ProfileHandlerUnregisterCallback(prof_handler_token_);
  prof_handler_token_ = NULL;
}

// Signal handler that records the pc in the profile-data structure. We do no
// synchronization here.  profile-handler.cc guarantees that at most one
// instance of prof_handler() will run at a time. All other routines that
// access the data touched by prof_handler() disable this signal handler before
// accessing the data and therefore cannot execute concurrently with
// prof_handler().
void CpuProfiler::prof_handler(int sig, siginfo_t *, void* signal_ucontext,
                               void * cpu_profiler) {
  CpuProfiler* instance = static_cast<CpuProfiler*>(cpu_profiler);

  if (instance->filter_ == NULL ||
      (*instance->filter_)(instance->filter_arg_)) {
    void * stack[ProfileData::kMaxStackDepth];
    // Under frame-pointer-based unwinding at least on x86, the
    // top-most active routine doesn't show up as a normal frame, but
    // as the "pc" value in the signal handler context.
    stack[0] = GetPC(*reinterpret_cast<ucontext_t*>(signal_ucontext));
    // We skip the top three stack trace entries (this function,
    // SignalHandler::SignalHandler and one signal handler frame)
    // since they are artifacts of profiling and should not be
    // measured.  Other profiling related frames may be removed by
    // "pprof" at analysis time.  Instead of skipping the top frames,
    // we could skip nothing, but that would increase the profile size
    // unnecessarily.
    int depth = GetStackTraceWithContext(stack + 1, arraysize(stack) - 1,
                                         3, signal_ucontext);
    void ** used_stack = NULL;
    if (stack[1] == stack[0]) {
      // in case of non-frame-pointer-based unwinding we will get
      // duplicate of PC in stack[1], which we don't want
      used_stack = stack + 1;
    } else {
      used_stack = stack;
      // To account for pc value in stack[0];
      depth++;
    }
    instance->collector_.Add(depth, used_stack);

    if (FLAGS_cpu_profiler_debug) {
      char symbol[1024] = { 0 };
      std::string debug_str;
      for (int i = 0; i < depth; i++) {
        if (google::Symbolize(static_cast<char *>(used_stack[i]),
                              symbol, sizeof(symbol))) {
          uint64_t add = reinterpret_cast<uint64_t>(used_stack[i]);
          debug_str = "[" + IntToString(i) + "]:" + Uint64ToString(add) +
                      "/" + symbol + "\n" + debug_str;
        }
      }
      LOG(INFO) << "\n" << debug_str;
    }
  }
}

void ProfilerRegisterThread() {
  ProfileHandlerRegisterThread();
}

void ProfilerFlush() {
  CpuProfiler::instance_.FlushTable();
}

int ProfilingIsEnabledForAllThreads() {
  return CpuProfiler::instance_.Enabled();
}

int ProfilerStart(const char* fname, const char * sname) {
  return CpuProfiler::instance_.Start(fname, sname, NULL);
}

int ProfilerStartWithOptions(const char *fname, const char * sname,
                             const ProfilerOptions *options) {
  return CpuProfiler::instance_.Start(fname, sname, options);
}

void ProfilerStop() {
  CpuProfiler::instance_.Stop();
}

void ProfilerGetCurrentState(
    ProfilerState* state) {
  CpuProfiler::instance_.GetCurrentState(state);
}
