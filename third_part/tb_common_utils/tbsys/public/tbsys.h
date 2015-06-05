// (C) 2007-2010 Taobao Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// Version: $Id$
//
// Authors:
//   duolong <duolong@taobao.com>

#ifndef TBSYS_H
#define TBSYS_H

#include <assert.h>
#include <errno.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <vector>
#include <string>

#ifndef UNUSED
#define UNUSED(v) ((void)(v))
#endif

namespace tbsys {
class CConfig;
class CFileUtil;
class CStringUtil;
class CNetUtil;
class CTimeUtil;
class CProcess;
class CLogger;
class CThread;
class CThreadMutex;
class CThreadCond; 
class Runnable;
class CDefaultRunnable;
class CFileQueue;
class IQueueHandler;
class CQueueThread;
class CFileQueueThread;
};//end namespace tbsys

namespace tbutil {

class noncopyable {
 protected:
  noncopyable() { }
  ~noncopyable() { }
 private:
   noncopyable(const noncopyable&);
   const noncopyable& operator=(const noncopyable&);
};

#if defined(__BCPLUSPLUS__) || defined(_MSC_VER)
  typedef __int64 Int64;
  #define T_INT64(n) n##i64
#elif defined(TNET_64)
  typedef long Int64;
  #define T_INT64(n) n##L
#else
  typedef long long Int64;
  #define T_INT64(n) n##LL
#endif
 
typedef unsigned char Byte;
typedef short Short;
typedef int Int;
typedef Int64 Long;
typedef float Float;
typedef double Double;
typedef ::std::vector<bool> BoolSeq;
typedef ::std::vector< Byte> ByteSeq;
typedef ::std::vector< Short> ShortSeq;
typedef ::std::vector< Int> IntSeq;
typedef ::std::vector< Long> LongSeq;
typedef ::std::vector< Float> FloatSeq;
typedef ::std::vector< Double> DoubleSeq;
typedef ::std::vector< ::std::string> StringSeq;
inline int getSystemErrno() {
  return errno;
}

}  // namespace tbutil

#include "third_part/tb_common_utils/tbsys/public/atomic.h"
#include "third_part/tb_common_utils/tbsys/public/config.h"
#include "third_part/tb_common_utils/tbsys/public/file_util.h"
#include "third_part/tb_common_utils/tbsys/public/string_util.h"
#include "third_part/tb_common_utils/tbsys/public/tbnet_util.h"
#include "third_part/tb_common_utils/tbsys/public/tbtime_util.h"
#include "third_part/tb_common_utils/tbsys/public/process.h"
#include "third_part/tb_common_utils/tbsys/public/tblog.h"
#include "third_part/tb_common_utils/tbsys/public/tbrwlock.h"

#include "third_part/tb_common_utils/tbsys/public/runnable.h"
#include "third_part/tb_common_utils/tbsys/public/iqueue_handler.h"
#include "third_part/tb_common_utils/tbsys/public/defaul_trunnable.h"
#include "third_part/tb_common_utils/tbsys/public/thread.h"
#include "third_part/tb_common_utils/tbsys/public/thread_mutex.h"
#include "third_part/tb_common_utils/tbsys/public/thread_cond.h"

#include "third_part/tb_common_utils/tbsys/public/queue_thread.h"
#include "third_part/tb_common_utils/tbsys/public/file_queue.h"
#include "third_part/tb_common_utils/tbsys/public/file_queue_thread.h"
#include "third_part/tb_common_utils/tbsys/public/profiler.h"
#include "third_part/tb_common_utils/tbsys/public/byte_buffer.h"

#endif
