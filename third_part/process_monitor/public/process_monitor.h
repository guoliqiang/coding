// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-20 22:43:19
// File  : process_monitor.h
// Brief : https://github.com/russells/process-monitor

#ifndef  __PROCESS_MONITOR_H_
#define  __PROCESS_MONITOR_H_

#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pty.h>
#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/thread.h"
#include "third_part/signal_monitor/public/signal_monitor.h"

namespace process_monitor {

class ProcessMonitor : public base::Thread {
 public:
  ProcessMonitor(char * (* const argv), bool daemon = true,
                 bool joinable = true)
      : base::Thread(joinable) , argv_(argv), daemon_(daemon) {
    pty_fd_ = -1;
    child_pid_ = -1;
    CHECK(argv_ != NULL && argv_[0] != NULL) << "valid input";
  }

  ~ProcessMonitor() {
    if (pty_fd_ != -1) close(pty_fd_);
  }

  void GoDaemon() {
    int ret = fork();
    CHECK(ret != -1) << "can not fork!";
    if (ret != 0) { // parent
      exit(0);
    } else { // child
      close(0);
      close(1);
      close(2);
      open("/dev/null", O_RDONLY);
      open("/dev/null", O_RDONLY);
      open("/dev/null", O_RDONLY);
      int sid = setsid();
      CHECK(sid != -1) << "can not set sid";
    }
  }

  void StartChild() {
    if (pty_fd_ != -1) {
      close(pty_fd_);
      pty_fd_ = -1;
    }
    child_pid_ = -1;
    int pid = forkpty(&pty_fd_, NULL, NULL, NULL);
    CHECK(pid != -1) << "can not fork";
    if (pid != 0) { // parent
      child_pid_ =  pid;
      fcntl(pty_fd_, F_SETFL, O_NONBLOCK);
    } else {
      execv(argv_[0], argv_);
    }
  }

  void HandleChildSignal() {
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
    if (pid == -1 || pid != child_pid_) {
      LOG(WARNING) << "pid=" << pid << " child_pid=" << child_pid_;
      return;
    }
    if (WIFSIGNALED(status)) {
      LOG(INFO) << argv_[0] << "/" << child_pid_
                << " exit due to signal "  << WTERMSIG(status)
                << " status " << WEXITSTATUS(status);
    } else {
      LOG(INFO) << argv_[0] << "/" << child_pid_
                << " exit with status " << WEXITSTATUS(status);
    }
    StartChild();
  }

  void StartMonitor() {
    if (daemon_) GoDaemon();
    base::Closure * callback = base::NewPermanentCallback(
        this, &ProcessMonitor::HandleChildSignal);
    signal_monitor::SignalMonitor::GetInstance()->AddCallback(SIGCHLD,
        callback);
    StartChild();
  }

 void Read() {
   if (pty_fd_ == -1) return;
   while (true) {
     char buf[1024] = {0};
     int ret = read(pty_fd_, buf, 1024);
     if (ret == 0) {
       LOG(WARNING) << "pty closed";
       pty_fd_ = -1;
       return;
     } else if (ret == -1) {
       if (errno != EWOULDBLOCK) {
         if (errno != EIO) {
           LOG(WARNING) << "can not read from pty";
         }
         close(pty_fd_);
         pty_fd_ = -1;
       }
       return;
     }
     LOG(INFO) << buf;
   }
 }

 protected:
  virtual void Run() {
    StartMonitor();
    while (true) {
      if (pty_fd_ == -1) {
        sleep(1);
        continue;
      }
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(pty_fd_, &read_fds);
      struct timeval timeout;
      timeout.tv_sec = 10;
      timeout.tv_usec = 0;
      int ret = select(pty_fd_ + 1, &read_fds, 0, 0, &timeout);
      if (ret == -1 && errno != EINTR) {
        LOG(WARNING)  << "select error";
        continue;
      } else if (!FD_ISSET(pty_fd_, &read_fds)) {
        continue;
      }
      Read();
    }
  }

 private:
  char * (* const argv_);
  bool daemon_;
  int pty_fd_;
  int child_pid_;
};

}  // namespace process_monitor

#endif  //__PROCESS_MONITOR_H_
