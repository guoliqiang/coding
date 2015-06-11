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

#include "third_part/cpu_profiler2/public/config.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "third_part/cpu_profiler2/public/stacktrace.h"
#include "base/public/logging.h"

namespace cpu_profiler2 {

// we're using plain struct and not class to avoid any possible issues
// during initialization. Struct of pointers is easy to init at
// link-time.
struct GetStackImplementation {
  int (*GetStackFramesPtr)(void** result, int* sizes, int max_depth,
                           int skip_count);

  int (*GetStackFramesWithContextPtr)(void** result, int* sizes, int max_depth,
                                      int skip_count, const void *uc);

  int (*GetStackTracePtr)(void** result, int max_depth,
                          int skip_count);

  int (*GetStackTraceWithContextPtr)(void** result, int max_depth,
                                  int skip_count, const void *uc);

  const char *name;
};

}  // namespace cpu_profiler2

#if HAVE_DECL_BACKTRACE
#define STACKTRACE_INL_HEADER "stacktrace_generic-inl.h"
#define GST_SUFFIX generic
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_generic
#endif

// libunwind uses __thread so we check for both libunwind.h and
// __thread support
#if defined(HAVE_LIBUNWIND_H) && defined(HAVE_TLS)
#define STACKTRACE_INL_HEADER "stacktrace_libunwind-inl.h"
#define GST_SUFFIX libunwind
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_libunwind
#endif // HAVE_LIBUNWIND_H

#if defined(__i386__) || defined(__x86_64__)
#define STACKTRACE_INL_HEADER "stacktrace_x86-inl.h"
#define GST_SUFFIX x86
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_x86
#endif // i386 || x86_64

#if defined(__ppc__) || defined(__PPC__)
#if defined(__linux__)
#define STACKTRACE_INL_HEADER "stacktrace_powerpc-linux-inl.h"
#else
#define STACKTRACE_INL_HEADER "stacktrace_powerpc-darwin-inl.h"
#endif
#define GST_SUFFIX ppc
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_ppc
#endif

#if defined(__arm__)
#define STACKTRACE_INL_HEADER "stacktrace_arm-inl.h"
#define GST_SUFFIX arm
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_arm
#endif

#ifdef TCMALLOC_ENABLE_INSTRUMENT_STACKTRACE
#define STACKTRACE_INL_HEADER "stacktrace_instrument-inl.h"
#define GST_SUFFIX instrument
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_instrument
#endif

// The Windows case -- probably cygwin and mingw will use one of the
// x86-includes above, but if not, we can fall back to windows intrinsics.
#if defined(_WIN32) || defined(__CYGWIN__) || defined(__CYGWIN32__) \
    || defined(__MINGW32__)
#define STACKTRACE_INL_HEADER "stacktrace_win32-inl.h"
#define GST_SUFFIX win32
#include "third_part/cpu_profiler2/public/stacktrace_impl_setup-inl.h"
#undef GST_SUFFIX
#undef STACKTRACE_INL_HEADER
#define HAVE_GST_win32
#endif

namespace cpu_profiler2 {

static GetStackImplementation *all_impls[] = {
#ifdef HAVE_GST_generic
  &impl__generic,
#endif
#ifdef HAVE_GST_libunwind
  &impl__libunwind,
#endif
#ifdef HAVE_GST_x86
  &impl__x86,
#endif
#ifdef HAVE_GST_arm
  &impl__arm,
#endif
#ifdef HAVE_GST_ppc
  &impl__ppc,
#endif
#ifdef HAVE_GST_instrument
  &impl__instrument,
#endif
#ifdef HAVE_GST_win32
  &impl__win32,
#endif
  NULL
};

}  // namespace cpu_profiler2

// ppc and i386 implementations prefer arch-specific asm implementations.
// arm's asm implementation is broken
#if defined(__i386__) || defined(__x86_64__) || defined(__ppc__) \
    || defined(__PPC__)
#if !defined(NO_FRAME_POINTER)
#define TCMALLOC_DONT_PREFER_LIBUNWIND
#endif
#endif

#if defined(HAVE_GST_instrument)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__instrument;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_win32)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__win32;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_x86) && defined(TCMALLOC_DONT_PREFER_LIBUNWIND)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__x86;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_ppc) && defined(TCMALLOC_DONT_PREFER_LIBUNWIND)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__ppc;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_libunwind)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__libunwind;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_arm)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__arm;
}  // namespace cpu_profiler2

#elif defined(HAVE_GST_generic)
namespace cpu_profiler2 {
static GetStackImplementation *get_stack_impl = &impl__generic;
}  // namespace cpu_profiler2

#elif 0
// This is for the benefit of code analysis tools that may have
// trouble with the computed #include above.
#include "third_part/cpu_profiler2/public/stacktrace_x86-inl.h"
#include "third_part/cpu_profiler2/public/stacktrace_libunwind-inl.h"
#else
#error Cannot calculate stack trace: will need to write for your environment
#endif

namespace cpu_profiler2 {

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

int GetStackTrace(void** result, int max_depth, int skip_count) {
  return frame_forcer(get_stack_impl->GetStackTracePtr(result, max_depth,
                                                       skip_count));
}

int GetStackTraceWithContext(void** result, int max_depth,
                             int skip_count, const void *uc) {
  return frame_forcer(get_stack_impl->GetStackTraceWithContextPtr(
      result, max_depth, skip_count, uc));
}

static void init_default_stack_impl_inner(void) {
  char *val = getenv("TCMALLOC_STACKTRACE_METHOD");
  if (!val || !*val) {
    return;
  }
  for (GetStackImplementation **p = all_impls; *p; p++) {
    GetStackImplementation *c = *p;
    if (strcmp(c->name, val) == 0) {
      get_stack_impl = c;
      return;
    }
  }
  LOG(ERROR) << "Unknown or unsupported stacktrace method requested."
                "Ignoring it" << val;
}

static void init_default_stack_impl(void) {
  init_default_stack_impl_inner();
  if (EnvToBool("TCMALLOC_STACKTRACE_METHOD_VERBOSE", false)) {
    LOG(ERROR) << "Chosen stacktrace method is " << get_stack_impl->name
               << "Supported methods:";
    for (GetStackImplementation **p = all_impls; *p; p++) {
      GetStackImplementation *c = *p;
      LOG(ERROR) << c->name;
    }
  }
}

REGISTER_MODULE_INITIALIZER(stacktrace_init_default_stack_impl,
                            init_default_stack_impl());

}  // namespace cpu_profiler2
