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
// Author: Paul Menage <opensource@google.com>
//
// Some wrappers for pthread functions so that we can be LD_PRELOADed
// against non-pthreads apps.
//
// This module will behave very strangely if some pthreads functions
// exist and others don't.

#include "third_part/cpu_profiler/public/maybe_threads.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include "base/public/basictypes.h"

// NOTE: it's similar to bitcast defined in basic_types.h with
// exception of ignoring sizes mismatch
template <typename T1, typename T2>
static T2 memcpy_cast(const T1 &input) {
  T2 output;
  size_t s = sizeof(input);
  if (sizeof(output) < s) {
    s = sizeof(output);
  }
  memcpy(&output, &input, s);
  return output;
}

int perftools_pthread_key_create(pthread_key_t *key,
                                 void (*destr_function) (void *)) {  // NOLINT
  return pthread_key_create(key, destr_function);
}

int perftools_pthread_key_delete(pthread_key_t key) {
  return pthread_key_delete(key);
}

void *perftools_pthread_getspecific(pthread_key_t key) {
  return pthread_getspecific(key);
}

int perftools_pthread_setspecific(pthread_key_t key, void *val) {
  return pthread_setspecific(key, val);
}

int perftools_pthread_once(pthread_once_t *ctl,
                           void  (*init_routine) (void)) {  // NOLINT
  return pthread_once(ctl, init_routine);
}
