// Copyright 2013 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2013-12-19 21:55:39
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"
// wrong
namespace algorithm {
typedef struct {
  char *buffer;
  int length;
  int start;
  int end;
} RingBuffer;

RingBuffer *RingBuffer_create(int length);
void RingBuffer_destroy(RingBuffer *buffer);
int RingBuffer_read(RingBuffer *buffer, char *target, int amount);
int RingBuffer_write(RingBuffer *buffer, char *data, int length);
int RingBuffer_empty(RingBuffer *buffer);
int RingBuffer_full(RingBuffer *buffer);
int RingBuffer_available_data(RingBuffer *buffer);
int RingBuffer_available_space(RingBuffer *buffer);

RingBuffer *RingBuffer_create(int length) {
  RingBuffer *buffer = static_cast<RingBuffer*>(calloc(1, sizeof(RingBuffer)));
  buffer->length  = length + 1;
  buffer->start = 0;
  buffer->end   = 0;
  buffer->buffer = static_cast<char *>(calloc(buffer->length, 1));
  return buffer;
}

void RingBuffer_destroy(RingBuffer *buffer) {
  if (buffer) {
    free(buffer->buffer);
    free(buffer);
  }
}

int RingBuffer_full(RingBuffer *buffer) {
  return (buffer->end + 1) % buffer->length == buffer->start;
}

int RingBuffer_empty(RingBuffer *buffer) {
  return buffer->end == buffer->start;
}

int RingBuffer_available_space(RingBuffer *buffer) {
  return buffer->length - buffer->end - 1;
}

int RingBuffer_available_data(RingBuffer *buffer) {
  return buffer->length - buffer->start - 1;
}

int RingBuffer_write(RingBuffer *buffer, char *data, int length) {
  if (length > RingBuffer_available_space(buffer)) {
    return -1;
  }

  void *result = memcpy(buffer->buffer + buffer->end, data, length);
  if (result == NULL) {
    LOG(ERROR) << "Failed to write data into buffer.";
    return -1;
  }
  buffer->end = (buffer->end + length) % buffer->length;
  if (buffer->end == buffer->start) {
    buffer->start = (buffer->start + length) % buffer->length;
  }
  return length;
}

int RingBuffer_read(RingBuffer *buffer, char *target, int amount) {
  int t = RingBuffer_available_data(buffer);
  LOG(INFO) << "available space is :" << t;
  if (amount > RingBuffer_available_data(buffer)) {
    LOG(ERROR) << "Available is:" << RingBuffer_available_space(buffer)
               << " amount:" << amount;
    return -1;
  }
  void *result = memcpy(target, buffer->buffer + buffer->start, amount);
  if (result == NULL) {
    LOG(ERROR) << "Failed to write buffer into data.";
    return -1;
  }
  buffer->start = (buffer->start + amount) % buffer->length;
  return amount;
}
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  char read[100];
  // char write[100];
  RingBuffer * bf = RingBuffer_create(3);
  LOG(INFO) << RingBuffer_read(bf, read, 2);
  return 0;
}
