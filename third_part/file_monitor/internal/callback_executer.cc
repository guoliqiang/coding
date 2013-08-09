// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#include "third_part/file_monitor/public/callback_executer.h"
#include <queue>
#include "base/public/callback.h"
#include "base/public/mutex.h"

namespace util {
const int MicrosecondsInSecond = 1000000;

CallbackExecuter::CallbackExecuter() : status_(IDLE) {
  mutex_.reset(new base::Mutex);
}

CallbackExecuter::~CallbackExecuter() {
  base::MutexLock lock(mutex_.get());
  callbackmap_.clear();
}
void CallbackExecuter::AddCallback(int wd,
                                   const std::string& path,
                                   const CallbackSet& callback,
                                   WatchEventType eventtype) {
  VLOG(6) << "CallbackExecuter::AddCallback in"
          << ", path: " << path
          << ", type: " << eventtype
          << ", wd: " << wd;
  base::MutexLock lock(mutex_.get());
  CallbackMap::iterator iter = callbackmap_.find(wd);
  if (iter != callbackmap_.end()) {
    iter->second.path = path;
    iter->second.callbackset = callback;
    iter->second.tick = base::TimeTicks::Now();
  } else {
    // VLOG(1) << "3";
    struct MCallbackValueNode node;
    node.path = path;
    node.callbackset = callback;
    node.eventtype = eventtype;
    node.tick = base::TimeTicks::Now();
    callbackmap_.insert(std::make_pair(wd, node));
  }
  VLOG(6) << "CallbackExecuter::AddCallback out";
}

void CallbackExecuter::Run() {
  status_ = RUNNING;
  // copy call back map into a local queue
  std::queue<MCallbackNode>  localqueue;
  while (status_ == RUNNING) {
    mutex_->Lock();
    base::TimeTicks now = base::TimeTicks::Now();
    if (callbackmap_.size()) {
      CallbackMap::iterator iter = callbackmap_.begin();
      while (iter != callbackmap_.end()) {
        if (IsTimespanEnough(iter->second.tick, now)) {
          MCallbackNode innode;
          innode.wd = iter->first;
          innode.value.path = iter->second.path;
          innode.value.callbackset = iter->second.callbackset;
          innode.value.tick = iter->second.tick;
          innode.value.eventtype = iter->second.eventtype;
          localqueue.push(innode);
          callbackmap_.erase(iter++);
        } else {
          iter++;
        }
      }
    }
    mutex_->Unlock();
    while (!localqueue.empty()) {
      for (int i = 0; i < localqueue.front().value.callbackset.size(); i++) {
         VLOG(6) << "file " << localqueue.front().value.path
                 << " gets a call back:"
                 << localqueue.front().value.callbackset[i].get();
         localqueue.front().value.callbackset[i]->Run();
         VLOG(6) << "callback done";
      }
      localqueue.pop();
    }
    if (status_ == RUNNING) {
      sleep(CallbackTimeoutSec);
    }
  }
  callbackmap_.clear();
  status_ = QUIT;
  LOG(INFO)<< "CallbackExecuter thread stops";
}

bool CallbackExecuter::IsTimespanEnough(const base::TimeTicks& val1,
                                        const base::TimeTicks& val2) {
  base::TimeDelta delta = val2 - val1;
  return delta.InMicroseconds() >  WatchTimeoutMicroSec;
}

void CallbackExecuter::StopRunning() {
  status_ = QUITING;
  while (status_ != QUIT) {
    sleep(ThreadQuittingTimeoutSec);
  }
  return;
}
}
