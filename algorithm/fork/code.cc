// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Zhongwu Zhai(zhaizhongwu@jike.com)
// Date  : 2014-04-15 01:41:11
// File  : code.cc
// Brief :

/*
fork will also copy the data of heap!
http://www.cnblogs.com/Anker/archive/2013/01/16/2862800.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "base/public/common_ojhead.h"

struct Shared {
  sem_t mutex;
  int count;
} shared;

void Foo() {
  int nloop = 10;
  int fd = open("mytest", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  write(fd,&shared,sizeof(struct Shared));
  struct  Shared *ptr = (struct  Shared *)mmap(NULL, sizeof(struct Shared), PROT_READ | PROT_WRITE,
                                               MAP_SHARED, fd, 0);
  close(fd);
  if(ptr == MAP_FAILED) {
    LOG(INFO) << "mmap error!";
  } else {
    sem_init(&ptr->mutex,1,1);
    setbuf(stdout, NULL);
    pid_t pid = fork();
    if (pid == 0) {
      for (int i = 0; i < nloop; ++i) {
        sem_wait(&ptr->mutex);
        LOG(INFO) << "child:" << ptr->count++;
        sem_post(&ptr->mutex);
        sleep(1);
      }
    } else {
      for (int i = 0;i < nloop; ++i) {
        sem_wait(&ptr->mutex);
        LOG(INFO) << "parent:" << ptr->count++;
        sem_post(&ptr->mutex);
        sleep(2);
      }
      int status = 0;
      waitpid(pid, &status, 0);
    }
  }
}

int main(int argc, char **argv) {
  Foo();
}

