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
//
// Produce stack trace.
//
// There are three different ways we can try to get the stack trace:
//
// 1) Our hand-coded stack-unwinder.  This depends on a certain stack
//    layout, which is used by gcc (and those systems using a
//    gcc-compatible ABI) on x86 systems, at least since gcc 2.95.
//    It uses the frame pointer to do its work.
//
// 2) The libunwind library.  This is still in development, and as a
//    separate library adds a new dependency, abut doesn't need a frame
//    pointer.  It also doesn't call malloc.
//
// 3) The gdb unwinder -- also the one used by the c++ exception code.
//    It's obviously well-tested, but has a fatal flaw: it can call
//    malloc() from the unwinder.  This is a problem because we're
//    trying to use the unwinder to instrument malloc().
//
// Note: if you add a new implementation here, make sure it works
// correctly when GetStackTrace() is called with max_depth == 0.
// Some code may do that.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "third_part/cpu_profiler/public/stacktrace.h"


// we're using plain struct and not class to avoid any possible issues
// during initialization. Struct of pointers is easy to init at
// link-time.
struct GetStackImplementation {
  int (*GetStackFramesPtr)(void** result,
                           int* sizes,
                           int max_depth,
                           int skip_count);

  int (*GetStackFramesWithContextPtr)(void** result, int* sizes, int max_depth,
                                      int skip_count, const void *uc);

  int (*GetStackTracePtr)(void** result, int max_depth,
                          int skip_count);

  int (*GetStackTraceWithContextPtr)(void** result, int max_depth,
                                  int skip_count, const void *uc);

  const char *name;
};

#define STACKTRACE_INL_HEADER "third_part/cpu_profiler/public/stacktrace_libunwind-inl.h"
#define GST_SUFFIX libunwind
#include "third_part/cpu_profiler/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER

static GetStackImplementation *get_stack_impl = &impl__libunwind;

static int frame_forcer(int rv) {
  return rv;
}

int GetStackFrames(void** result, int* sizes, int max_depth, int skip_count) {
  return frame_forcer(get_stack_impl->GetStackFramesPtr(result, sizes,
      max_depth, skip_count));
}

int GetStackFramesWithContext(void** result, int* sizes, int max_depth,
                              int skip_count, const void *uc) {
  return frame_forcer(get_stack_impl->GetStackFramesWithContextPtr(
                      result, sizes, max_depth,
                      skip_count, uc));
}

int GetStackTrace(void** result, int max_depth,
                  int skip_count) {
  return frame_forcer(get_stack_impl->GetStackTracePtr(result, max_depth,
                                                       skip_count));
}

int GetStackTraceWithContext(void** result, int max_depth,
                             int skip_count, const void *uc) {
  return frame_forcer(get_stack_impl->GetStackTraceWithContextPtr(
                        result, max_depth, skip_count, uc));
}
