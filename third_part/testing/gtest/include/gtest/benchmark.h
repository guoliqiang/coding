// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// Ported by: yinghuang@yunrang.com (Ying Huang)
#ifndef TESTING_GTEST_INCLUDE_GTEST_BENCHMARK_H_
#define TESTING_GTEST_INCLUDE_GTEST_BENCHMARK_H_

#include "../../../../../base/public/basictypes.h"

// This is a hack to disable the gcc 4.4 warning about hash_map and hash_set
// being deprecated.  We can get rid of this when we upgrade to VS2008 and we
// can use <tr1/unordered_map> and <tr1/unordered_set>.
#ifdef __DEPRECATED
#define YUNRANG_OLD__DEPRECATED __DEPRECATED
#undef __DEPRECATED
#endif
#include <ext/hash_set>
#include <ext/hash_map>
#ifdef YUNRANG_OLD__DEPRECATED
#define __DEPRECATED CHROME_OLD__DEPRECATED
#undef YUNRANG_OLD__DEPRECATED
#endif

// C
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>         // For size_t
#include <assert.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>

// C++
#include <vector>
#include <string>
#include <algorithm>
#include <iosfwd>
#include <map>
#include <stack>
#include <utility>
#include <set>

// Use std names.
using std::set;
using std::pair;
using std::vector;
using std::string;
using std::min;
using std::max;
using std::ostream;
using std::map;
using std::stack;
using std::sort;
using std::swap;
using std::make_pair;
using __gnu_cxx::hash_set;
using __gnu_cxx::hash;

namespace testing {

struct Benchmark {
  const char* name;
  void (*fn)(int);
  void (*fnr)(int, int);
  int lo;
  int hi;
  int threadlo;
  int threadhi;

  void Register();
  Benchmark(const char* name, void (*f)(int)) { Clear(name); fn = f; Register(); }
  Benchmark(const char* name, void (*f)(int, int), int l, int h) { Clear(name); fnr = f; lo = l; hi = h; Register(); }
  void Clear(const char* n) { name = n; fn = 0; fnr = 0; lo = 0; hi = 0; threadlo = 0; threadhi = 0; }
  Benchmark* ThreadRange(int lo, int hi) { threadlo = lo; threadhi = hi; return this; }
};

class TimeRecorder {
 public:
  TimeRecorder() : time_used_(0) {}
  void Resume() { last_ = nsec(); }
  void Suspend() { time_used_ += nsec() - last_; }
  int64 GetTimeUsed() const { return time_used_; }
 protected:
  static int64 nsec() {
    struct timeval tv;
    if(gettimeofday(&tv, 0) < 0)
      return -1;
    return (int64)tv.tv_sec*1000*1000*1000 + tv.tv_usec*1000;
  }
  int64 time_used_;
  int64 last_;
};

int NumCPUs();
void SetBenchmarkBytesProcessed(long long);
void StartBenchmarkTiming();
void StopBenchmarkTiming();
void BenchmarkMemoryUsage();
void SetBenchmarkItemsProcessed(int);
int round(int n);
void RUN_ALL_BENCHMARKS();

}  // namespace testing

#define BENCHMARK(f) \
  ::testing::Benchmark* _benchmark_##f = (new ::testing::Benchmark(#f, f))

#define BENCHMARK_RANGE(f, lo, hi) \
  ::testing::Benchmark* _benchmark_##f = \
  (new ::testing::Benchmark(#f, f, lo, hi))

#endif  // TESTING_GTEST_INCLUDE_GTEST_BENCHMARK_H_

