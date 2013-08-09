// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#include "third_part/file_monitor/public/file_watcher.h"
#include <sys/inotify.h>
#include <sys/select.h>
#include <string>
#include "third_part/file_monitor/public/monitor_observer.h"

namespace util {
const unsigned int BufferLength = 1024;
FileWatcher::FileWatcher(MonitorObserver* observer) : observer_(observer),
                                                      status_(IDLE) {
  fd_ = inotify_init();
}

FileWatcher::~FileWatcher() {}

void FileWatcher::Run() {
  status_ = RUNNING;
  while (status_ == RUNNING) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd_, &fds);

    timeval timer = {0, WatchTimeoutMicroSec};
    if (select(fd_+1, &fds, NULL, NULL, &timer) > 0) {
      int len(0);
      int index(0);
      unsigned char buf[BufferLength] = {0};
      if ((len = read(fd_, &buf, sizeof(buf))) > 0) {
        while (index < len) {
          struct inotify_event *event = NULL;
          event = (struct inotify_event *)(buf + index);
          index += sizeof(struct inotify_event) + event->len;
          std::string pname(event->name);
          VLOG(7) << "len: " << event->len;
          if (event->len == 0) {
            pname.clear();
          }
          VLOG(6) << "pname is "<< event->name;
          // don't handle . &&.. event
          if ((pname == ".") || (pname == "..")) {
             continue;
          }
          WatchEventType event_type = GetWatchEventType(event->mask);
          if (event_type >= 0 && status_ == RUNNING) {
            observer_->HandleInotifyEvent(event->wd, event_type, pname);
          }
        }
      }
    }
  }
  status_ = QUIT;
  LOG(INFO)<< "FileWatcher thread stops";
}

int FileWatcher::AddMonitor(const std::string& path) {
  // the same path will be return the same wd as before.
  int wd(-1);
  if (!path.empty()) {
    wd = inotify_add_watch(fd_,
                           path.c_str(),
                           IN_CLOSE_WRITE | IN_DELETE | IN_CREATE);
  }
  return wd;
}

// see https://www.ibm.com/developerworks/cn/linux/l-inotify/
// for the explanation for IN_CLOSE_WRITE / IN_CREATE / IN_DELETE / IN_IGNORED
WatchEventType FileWatcher::GetWatchEventType(unsigned int event) {
  if (event & IN_CLOSE_WRITE) {
    // an opende doc file or dir file is closed.
    return MODIFYEVENT;
  }
  if (event & IN_CREATE) {
    // a doc or sub-dir is created in an monitored dir.
    return CREATEEVENT;
  }
  if (event & IN_DELETE) {
    // a doc or sub-dir is deleted in an monitored dir.
    return DELETEEVENT;
  }
  if (event & IN_IGNORED) {
    // the monitored doc or dir is deleted.
    return SINGLEDELETEEVENT;
  }
  return INVALIDEVENT;
}

void FileWatcher::StopRunning() {
  status_= QUITING;
  while (status_ != QUIT) {
    sleep(ThreadQuittingTimeoutSec);
  }
  return;
}

void FileWatcher::RemoveWatching(int wd) {
  inotify_rm_watch(fd_, wd);
  VLOG(7)<< "watch " << wd << " is removed";
}
}
