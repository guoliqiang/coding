// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-01-19 04:37:50
// File  : pipe_monitor.h
// Brief :

#ifndef  __PIPE_MONITOR_H_
#define  __PIPE_MONITOR_H_

#include <string>
#include <map>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "base/public/thread.h"
#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/callback.h"
#include "base/public/shared_ptr.h"
#include "file/public/file.h"

namespace pipe_monitor {

class PipeMonitor :  public base::Thread {
 public:
  PipeMonitor(const std::string & pipe_path, bool joinable = false)
      : base::Thread(joinable) {
    pipe_path_ = pipe_path;
    fd_ = -1;
    Init();
  }

  ~PipeMonitor() {
    if (fd_ != -1) close(fd_);
  }

  void Init() {
    if (fd_ != -1) {
      close(fd_);
      fd_ = -1;
    }
    file::File::DeleteRecursively(pipe_path_);
    unlink(pipe_path_.c_str());
    int mk_ret = mkfifo(pipe_path_.c_str(), 0610);
    if (mk_ret) {
      CHECK(false) << "can not make fifo for " << pipe_path_ << " error is"
                   << strerror(errno);
    }
    fd_ = open(pipe_path_.c_str(), O_RDONLY | O_NONBLOCK);
    CHECK(fd_ != -1) << "can not open " << pipe_path_;
  }

  void AddCallback(char ch, base::Closure * callback) {
    base::MutexLock lock(&mutex_);
    callback_.insert(std::make_pair(ch,
        base::shared_ptr<base::Closure>(callback)));
  }

 protected:
  virtual void Run() {
    while (true) {
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(fd_, &read_fds);

      timeval timeout;
      timeout.tv_sec = 10;
      timeout.tv_usec = 0;
      int ret = select(fd_ + 1, &read_fds, 0, 0, &timeout);
      if (ret == -1 && errno != EINTR) {
        LOG(WARNING) << "select error :" << strerror(errno);
      } else {
        if (!FD_ISSET(fd_, &read_fds)) continue;
        char ch;
        int read_ret = read(fd_, &ch, 1);
        if (read_ret == 0) { // closed, reopen
          LOG(WARNING) << "read EOF from " << pipe_path_;
          Init();
          continue;
        } else if (read_ret == -1) {
          if (errno != EWOULDBLOCK) {
            LOG(WARNING) << "read error from " << pipe_path_;
            Init();
          }
          continue;
        } else if (isspace(ch)) {
          LOG(WARNING) << "read space charactter";
          continue;
        }
        base::MutexLock lock(&mutex_);
        if (callback_.count(ch)) {
          callback_[ch]->Run();
        } else {
          LOG(WARNING) << "not find callback for " << ch;
        }
      }
    }
  }

 private:
  std::string pipe_path_;
  int fd_;
  std::map<char, base::shared_ptr<base::Closure> > callback_;
  base::Mutex mutex_;
};

}  // namespace pipe_monitor

#endif  //__PIPE_MONITOR_H_
