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

#include "third_part/tb_common_utils/tbsys/public/tbsys.h"

namespace tbsys {

CDefaultRunnable::CDefaultRunnable(int threadCount) {
  _stop = false;
  _threadCount = threadCount;
  _thread = NULL;
}

CDefaultRunnable::~CDefaultRunnable() {
  if (_thread) {
    delete[] _thread;
    _thread = NULL;
  }
}

void CDefaultRunnable::setThreadCount(int threadCount) {
  if (_thread != NULL) {
    TBSYS_LOG(ERROR, "已经在运行了不能设置线程数");
    return;
  }
  _threadCount = threadCount;
}

void CDefaultRunnable::start() {
  if (_thread != NULL || _threadCount < 1) {
    TBSYS_LOG(ERROR, "start failure, _thread: %p, threadCount: %d",
              _thread, _threadCount);
    return;
  }
  _thread = new CThread[_threadCount];
  for (int i=0; i<_threadCount; i++) {
    _thread[i].start(this, (void*)((long)i));
  }
}

void CDefaultRunnable::stop() {
  _stop = true;
}

void CDefaultRunnable::wait() {
  if (_thread != NULL) {
    for (int i=0; i<_threadCount; i++) {
      _thread[i].join();
    }
  }
}

}  // namespace tbsys
