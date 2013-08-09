// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Simplified version of Google's logging.

#ifndef HADOOP_LOGGING_H__
#define HADOOP_LOGGING_H__

#include <unistd.h>  /* for write */
#include <sstream>
#include <string>
#include <stdlib.h>
#include "base/public/basictypes.h"
#include "base/public/flags.h"

DECLARE_int32(HP_v);

// Debug-only checking.
// HP == Hadoop

#define HP_DCHECK(condition) assert(condition)
#define HP_DCHECK_EQ(val1, val2) assert((val1) == (val2))
#define HP_DCHECK_NE(val1, val2) assert((val1) != (val2))
#define HP_LE(val1, val2) assert((val1) <= (val2))
#define HP_DCHECK_LT(val1, val2) assert((val1) < (val2))
#define HP_DCHECK_GE(val1, val2) assert((val1) >= (val2))
#define HP_DCHECK_GT(val1, val2) assert((val1) > (val2))

// Always-on checking
#define HP_CHECK(x)	if(x) { } else HPLogMessageFatal(__FILE__, __LINE__).stream() << "Check failed: " #x
#define HP_CHECK_LT(x, y)	HP_CHECK((x) < (y))
#define HP_CHECK_GT(x, y)	HP_CHECK((x) > (y))
#define HP_CHECK_LE(x, y)	HP_CHECK((x) <= (y))
#define HP_CHECK_GE(x, y)	HP_CHECK((x) >= (y))
#define HP_CHECK_EQ(x, y)	HP_CHECK((x) == (y))
#define HP_CHECK_NE(x, y)	HP_CHECK((x) != (y))

#define HP_LOG_INFO HPLogMessage(__FILE__, __LINE__)
#define HP_LOG_ERROR HPLOGMessage(__FILE__, __LINE__, 2)
#define HP_LOG_WARNING HP_LOG_INFO
#define HP_LOG_FATAL HPLogMessageFatal(__FILE__, __LINE__, 2)
#define HP_LOG_QFATAL HP_LOG_FATAL

#define HP_VLOG(x) if((x)>FLAGS_HP_v){} else HP_LOG_INFO.stream()

#ifdef NDEBUG
// #define DEBUG_MODE 0
#define HP_LOG_DFATAL HP_LOG_ERROR
#else
// #define DEBUG_MODE 1
#define HP_LOG_DFATAL HP_LOG_FATAL
#endif

#define HP_LOG(severity) HP_LOG_ ## severity.stream()

class HPLogMessage {
 public:
  HPLogMessage(const char* file, int line, int fd = 1) : flushed_(false), fd_(fd) {
    stream() << file << ":" << line << ": ";
  }
  void Flush() {
    stream() << "\n";
    std::string s = str_.str();
    int n = (int)s.size(); // shut up msvc
    if(write(fd_, s.data(), n) < 0) {}  // shut up gcc
    flushed_ = true;
  }
  ~HPLogMessage() {
    if (!flushed_) {
      Flush();
    }
  }
  std::ostream & stream() { return str_; }
 
 private:
  bool flushed_;
  int fd_;  // 0 =stdin 1 = stdout 2 =stderr
  std::ostringstream str_;
  DISALLOW_COPY_AND_ASSIGN(HPLogMessage);
};

class HPLogMessageFatal : public HPLogMessage {
 public:
  HPLogMessageFatal(const char* file, int line, int fd = 1)
    : HPLogMessage(file, line, fd) { }
  ~HPLogMessageFatal() {
    Flush();
    abort();
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(HPLogMessageFatal);
};

#endif  // HADOOP_LOGGING_H__
