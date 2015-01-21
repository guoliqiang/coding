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

class AnonymousPipeMonitor : public base::Thread {
 public:
  AnonymousPipeMonitor(bool joinable = false) : base::Thread(joinable) {
    fd_[0] = -1;
    fd_[1] = -1;
    Init();
  }

  ~AnonymousPipeMonitor() {
    if (fd_[0] != -1) close(fd_[0]);
    if (fd_[1] != -1) close(fd_[1]);
  }

  void Init() {
    if (fd_[0] == -1) close(fd_[0]);
    if (fd_[1] == -1) close(fd_[1]);
    CHECK(pipe(fd_) != -1) << "can not make pipe for "  << strerror(errno);
    fcntl(fd_[0], F_SETFL, O_NONBLOCK);
  }

  void AddCallback(char ch, base::Closure * callback) {
    base::MutexLock lock(&mutex_);
    callback_.insert(std::make_pair(ch,
        base::shared_ptr<base::Closure>(callback)));
  }

  void Execute(char ch) {
    base::MutexLock lock(&mutex_);
    if (callback_.count(ch)) {
      callback_[ch]->Run();
    } else {
      LOG(WARNING) << "not find callback for " << ch;
    }
  }

  void Write(char ch) {
    write(fd_[1], &ch, 1);
  }
 
 protected:
  virtual void Run() {
    while (true) {
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(fd_[0], &read_fds);

      timeval timeout;
      timeout.tv_sec = 10;
      timeout.tv_usec = 0;
      int ret = select(fd_[0] + 1, &read_fds, 0, 0, &timeout);
      if (ret == -1 && errno != EINTR) {
        LOG(WARNING) << "select error :" << strerror(errno);
      } else {
        if (!FD_ISSET(fd_[0], &read_fds)) continue;
        char ch;
        int read_ret = read(fd_[0], &ch, 1);
        if (read_ret == 0) { // closed, reopen
          LOG(WARNING) << "read EOF";
          Init();
          continue;
        } else if (read_ret == -1) {
          if (errno != EWOULDBLOCK) {
            LOG(WARNING) << "read error";
            Init();
          }
          continue;
        } else if (isspace(ch)) {
          LOG(WARNING) << "read space charactter";
          continue;
        }
        Execute(ch);
      }
    }
  }

 private:
  int fd_[2];
  std::map<char, base::shared_ptr<base::Closure> > callback_;
  base::Mutex mutex_;
};

class PipeMonitor :  public base::Thread {
 public:
  PipeMonitor(const std::string & pipe_path, bool joinable = false)
      : base::Thread(joinable) {
    pipe_path_ = pipe_path;
    fd_ = -1;
    wfd_ = -1;
    Init();
  }

  ~PipeMonitor() {
    if (fd_ != -1) close(fd_);
    if (wfd_ != -1) close(wfd_);
  }

  void Write(char ch) {
    write(wfd_, &ch, 1);
  }
 
  void Init() {
    if (fd_ != -1) {
      close(fd_);
      fd_ = -1;
    }
    if (wfd_ != -1) {
      close(wfd_);
      wfd_ = -1;
    }

    file::File::DeleteRecursively(pipe_path_);
    unlink(pipe_path_.c_str());
    int mk_ret = mkfifo(pipe_path_.c_str(), 0610);
    if (mk_ret) {
      CHECK(false) << "can not make fifo for " << pipe_path_ << " error is"
                   << strerror(errno);
    }
    fd_ = open(pipe_path_.c_str(), O_RDONLY | O_NONBLOCK);
    wfd_ = open(pipe_path_.c_str(), O_WRONLY | O_NONBLOCK);
    CHECK(fd_ != -1) << "can not open " << pipe_path_;
    CHECK(wfd_ != -1) << "can not open " << pipe_path_;
  }

  void AddCallback(char ch, base::Closure * callback) {
    base::MutexLock lock(&mutex_);
    callback_.insert(std::make_pair(ch,
        base::shared_ptr<base::Closure>(callback)));
  }

  void Execute(char ch) {
    base::MutexLock lock(&mutex_);
    if (callback_.count(ch)) {
      callback_[ch]->Run();
    } else {
      LOG(WARNING) << "not find callback for " << ch;
    }
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
        Execute(ch);
      }
    }
  }

 private:
  std::string pipe_path_;
  int fd_;
  int wfd_;
  std::map<char, base::shared_ptr<base::Closure> > callback_;
  base::Mutex mutex_;
};

}  // namespace pipe_monitor

#endif  //__PIPE_MONITOR_H_
