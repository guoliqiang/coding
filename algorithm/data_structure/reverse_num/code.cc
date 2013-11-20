// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-11-03 01:05:05
// File  : code.cc
// Brief :

// poj2299

#include "base/public/common_head.h"

namespace algorithm {
const int MAX = 1000;
int N = 0;
int data[MAX] = {0};
int temp[MAX] = {0};
int num = 0;

// n * log(n)
void ThroughMergeSort(int b, int e) {
  if (b == e) return;
  else {
    int mid = b + (e - b) / 2;
    ThroughMergeSort(b, mid);
    ThroughMergeSort(mid + 1, e);
    int k = 0;
    int i = b;
    int j = mid + 1;
    while(i <= mid && j <= e) {
      if (data[j] < data[i]) {
        num += mid - i + 1;
        temp[k++] = data[j];
        j++;
      } else {
        temp[k++] = data[i];
        i++;
      }
    }
    while (i <= mid) {
      temp[k++] = data[i++];
    }
    while (j <= e) {
      temp[k++] = data[j++];
    }
    for (int i = b; i <= e; i++) {
      data[i] = temp[i - b];
    }
  }
}

int ReverseNum() {
  num = 0;
  memset(temp, 0, sizeof(temp));
  ThroughMergeSort(0, N - 1);
  return num;
}
void Read(std::vector<int> & v) {
  N = v.size();
  memset(data, 0, sizeof(data));
  for (int i = 0; i < N; i++) {
    data[i] = v[i];
  }
}
}  // namespace algoroithm

using namespace algorithm;


int main(int argc, char** argv) {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(5);
  v.push_back(3);
  Read(v);
  LOG(INFO) << ReverseNum();
  return 0;
}
