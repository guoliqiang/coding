// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#include "third_part/file_monitor/public/event_pipeline.h"
#include <vector>
#include <string>
#include "base/public/mutex.h"
#include "base/public/time.h"
#include "third_part/file_monitor/public/file_monitor_common.h"
#include "third_part/file_monitor/public/monitor_handler.h"

namespace util {
InotifyEventPipeline::InotifyEventPipeline(FileMonitor* monitor)
         : base::Thread(false), monitor_(monitor), status_(IDLE) {
  mutex_for_queue_.reset(new base::Mutex());
}

InotifyEventPipeline::~InotifyEventPipeline() {
}

void InotifyEventPipeline::Push(const EventParam& event) {
  VLOG(6) << "InotifyEventPipeline::Push in"
          << ", path: " << event.fullpath
          << ", wd: " << event.wd
          << ", type: " << event.type;
  base::MutexLock lock(mutex_for_queue_.get());
  base::TimeTicks current_tick = base::TimeTicks::Now();
  event_list_.push_back(std::make_pair(current_tick, event));
}

bool InotifyEventPipeline::IsCoolingLongEnough(int64 second,
                                     const EventEntry& entry) {
  base::TimeTicks now = base::TimeTicks::Now();
  base::TimeDelta delta = now - entry.first;
  return delta.InSeconds() >= second;
}

bool InotifyEventPipeline::GetFrontOfEventList(EventEntry* entry) {
  base::MutexLock lock(mutex_for_queue_.get());
  if (event_list_.empty()) {
    return false;
  }
  *entry = event_list_.front();
  return true;
}

void InotifyEventPipeline::Pop() {
  base::MutexLock lock(mutex_for_queue_.get());
  event_list_.pop_front();
}

bool InotifyEventPipeline::IsEventListEmpty() {
  base::MutexLock lock(mutex_for_queue_.get());
  return event_list_.empty();
}

void InotifyEventPipeline::Run() {
  status_ = RUNNING;
  while (status_ == RUNNING) {
    std::vector<EventParam> result;
    while (!IsEventListEmpty()) {
      VLOG(7) << "In progress";
      EventEntry entry;
      if (!GetFrontOfEventList(&entry)
          ||!IsCoolingLongEnough(CoolingTimeoutSec, entry)) {
        break;
      }
      Pop();
      MergedEventList candidate;
      Extract(entry.second, &candidate);
      Reduce(&candidate);
      while (!candidate.empty()) {
        EventParam param = candidate.front();
        result.push_back(param);
        candidate.pop_front();
      }
    }
    // should push event into FileMonitor
    VLOG(7) << "result.size(): " << result.size();
    for (int i = 0; i < result.size(); i++) {
      monitor_->HandleEvent(result[i]);
      VLOG(6) << "InotifyEventPipeline run done!";
    }
    sleep(EventPipeLineSleepingSec);
  }
  status_ = QUIT;
  LOG(INFO) << "pipeline thread stops";
}

void InotifyEventPipeline::Extract(const EventParam& event,
                                    MergedEventList* eventset) {
  VLOG(6) << "InotifyEventPipeline::Extract in";
  // add event itself into eventset first
  eventset->push_back(event);
  // merge event with same wd or fullpath;
  int wd = event.wd;
  std::string fullpath = event.fullpath;
  base::MutexLock lock(mutex_for_queue_.get());
  EventList::iterator iter = event_list_.begin();
  while (iter != event_list_.end()) {
    if (iter->second.wd == wd || iter->second.fullpath == fullpath) {
      eventset->push_back(iter->second);
      event_list_.erase(iter++);
      continue;
    }
    iter++;
  }
  VLOG(7) << "eventset .size : " << eventset->size();
  VLOG(6) << "InotifyEventPipeline::Extract out";
}

void InotifyEventPipeline::StopRunning() {
  status_ = QUITING;
  while (status_ != QUIT) {
    sleep(ThreadQuittingTimeoutSec);
  }
}

void InotifyEventPipeline::Reduce(MergedEventList* singlelist) {
  VLOG(6) << "InotifyEventPipeline::Reduce in"
          << ", size of singlelist is " << singlelist->size();
  MergedEventList::iterator iter = singlelist->begin();
  MergedEventList::iterator cursor = singlelist->end();

  // strategy 1
  // Merge event with same filepath and
  // event singledelete & create/modify into a 'revive event'
  while (iter != singlelist->end()) {
    cursor = iter;
    cursor++;
    bool erased(false);
    while (cursor != singlelist->end()) {
      if (iter->type == SINGLEDELETEEVENT
          && (cursor->type == CREATEEVENT
            ||cursor->type == MODIFYEVENT)
          && cursor->fullpath == iter->fullpath) {
        monitor_->HandleReviveEvent(*cursor);
        singlelist->erase(cursor++);
        singlelist->erase(iter++);
        erased = true;
        continue;
      }
      cursor++;
    }
    if (!erased) {
      iter++;
    }
  }
  VLOG(6) << "strategy 1 done"
          << ", singlelist.size: " << singlelist->size();

  // strategy 2
  // merge events same path type delete & create event into a modify event
  iter = singlelist->begin();
  while (iter != singlelist->end()) {
    cursor = iter;
    cursor++;
    if (iter->type == DELETEEVENT) {
      while (cursor != singlelist->end()) {
        if (cursor->wd == iter->wd
            &&cursor->fullpath == iter->fullpath
            &&cursor->type == MODIFYEVENT) {
          iter->type = CREATEEVENT;
          singlelist->erase(cursor++);
          break;
        }
        cursor++;
      }
    }
    iter++;
  }
  VLOG(6) << "strategy 2 done"
          << ", singlelist.size: " << singlelist->size();

  // strategy 3
  // merge events same path&wd from create/modify into create
  iter = singlelist->begin();
  while (iter != singlelist->end()) {
    cursor = iter;
    cursor++;
    if (iter->type == CREATEEVENT) {
      while (cursor != singlelist->end()) {
        if (cursor->wd == iter->wd
            &&cursor->fullpath == iter->fullpath
            &&cursor->type == MODIFYEVENT) {
          singlelist->erase(cursor++);
          break;
        }
        cursor++;
      }
    }
    iter++;
  }
  VLOG(6) << "strategy 3 done"
          << ", singlelist.size: " << singlelist->size();
  // strategy 4
  // merge events with same wd&filepath into the latest event
  iter = singlelist->begin();
  while (iter != singlelist->end()) {
    cursor = iter;
    cursor++;
    bool erased(false);
    while (cursor != singlelist->end()) {
      if (cursor->wd == iter->wd
          &&cursor->fullpath == iter->fullpath
          &&iter->type != CREATEEVENT) {
        singlelist->erase(iter++);
        erased = true;
        break;
      }
      cursor++;
    }
    if (!erased) {
      iter++;
    }
  }
  VLOG(6) << "strategy 4 done"
          << ", singlelist.size: " << singlelist->size();
  VLOG(6) << "InotifyEventPipeline::Reduce out";
}
}
