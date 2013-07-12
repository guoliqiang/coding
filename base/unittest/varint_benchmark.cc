// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: quj@yunrang.com (Jing Qu)

#include <stdio.h>

#include "base/yr.h"
#include "base/varint.h"
#include "base/time.h"

int main(int argc, char **argv) {
  int kBufSize = 1 << 30;
  uint8* data = new uint8[kBufSize];
  uint8* ptr = data;
  uint64 data_size = 0;
  while (ptr < data + kBufSize - 20) {
    ptr = WriteVarint64(data_size * data_size * 13, ptr);
    data_size++;
  }
  printf("data_size: %ld\n", data_size);
  uint64 value;
  int64 start = GetTimeInMs();
  const uint8* ptr_read = data;
  data_size = 0;
  while (ptr_read < data + kBufSize - 20) {
    data_size++;
    ptr_read = ReadVarint64(ptr_read, &value);
  }
  printf("time used: %ld ms, decode %ld\n", GetTimeInMs() - start, data_size);
  delete[] data;
}
