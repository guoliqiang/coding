// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-15 23:39:58
// File  : eight_queue.h
// Brief :

/*
 * Write an algorithm to print all ways of arranging eight queens on a chess board so that none of them share the same row, column or diagonal.
 *
 * */

#ifndef  __EIGHT_QUEUE_H_
#define  __EIGHT_QUEUE_H_

#include "base/public/logging.h"
#include <vector>
#include <math.h>
#include <iostream>

namespace algorithm {

bool Check(std::vector<int> & queue, int n) {
  if (n == 0) return true;
  for (int i = 0; i <= n - 1; i++) {
    if (queue[i] == queue[n] ||
        fabs(queue[i] - queue[n]) == fabs(i - n))
      return false;
  }
  return true;
}

void TraceBack(std::vector<int> & queue,
               int n, int * rs) {
  for (int j = 0; j < 8; j++) {
    queue[n] = j;
    if (Check(queue, n)) {
      if (n == 7) {
        for (int i = 0; i < 8; i++) {
          std::cout << queue[i] << " ";
        }
        std::cout << std::endl;
        (*rs) += 1;
      } else {
       TraceBack(queue, n + 1, rs);
      }
    }
  }
}

int Queue() {
  int rs = 0;
  std::vector<int> queue;
  queue.assign(8, 0);
  TraceBack(queue, 0, &rs);
  return rs;
}

void TraceBack2(std::vector<int> & queue,
                int n, int * rs) {
  if (n == 8) {
    (*rs) += 1;
    return;
  }
  for (int i = n; i < 8; i++) {
    std::swap(queue[i], queue[n]);
    if (Check(queue, n)) {
       TraceBack2(queue, n + 1, rs);
    }
    std::swap(queue[i], queue[n]);
  }
}

int Queue2() {
  int rs = 0;
  std::vector<int> queue;
  for (int i = 0; i < 8; i++) {
    queue.push_back(i);
  }
  TraceBack2(queue, 0, &rs);
  return rs;
}

};

#endif  //  __EIGHT_QUEUE_H_
