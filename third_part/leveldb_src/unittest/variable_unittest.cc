// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-11-21 02:20:24
// File  : valiate_unittest.cc
// Brief : http://www.sjsjw.com/kf_code/article/033227ABA000171.asp

#include "base/public/logging.h"
#include "base/public/thread.h"

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

int flag=10; //这里是否由volatile, 结果不同
void * wait(void* param) {
  int count = 1;
  while ( flag != count ) {
    count = ~count;
  }
  printf("wait\n");
  return NULL;
}
void* wake(void* param) {
  flag = 1;
  printf("wake\n");
  return NULL;
}

void foo() {
  pthread_t t[2];
  pthread_create(&t[0], NULL, wait, NULL);
  sleep(1);
  pthread_create(&t[1], NULL, wake, NULL);
  while(1);
}

int A, B;
void foo2() {
  A = B + 1;
  B = 5;
}

int main(int argc, char** argv) {
  foo2();
  printf("%d\n", A);
  return 0;
}
