// (C) 2007-2010 Taobao Inc.
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// Version: $Id$
//
// Authors: duolong <duolong@taobao.com>

#include <string>
#include <algorithm>
#include "third_part/tb_common_utils/tbsys/public/byte_buffer.h"

using namespace tbutil;  // NOLINT

ByteBuffer::out_of_range::out_of_range(uint32_t p, uint32_t l, uint32_t s) {
  snprintf(errmsg_, MAX_ERROR_MSG_LEN, "current postion: %u, field length: %u,"
          "out of range size %u", p, l, s);
}

const char* ByteBuffer::out_of_range::what() const throw() {
  return errmsg_;
}

ByteBuffer::ByteBuffer(uint32_t size)
    : data_(NULL), size_(size), position_(0), own_(true) {
  if (size_ > 0)  {
    data_ = allocate(size_);
  }
}

ByteBuffer::ByteBuffer()
    : data_(NULL), size_(0), position_(0), own_(true) { }

ByteBuffer::ByteBuffer(const char* data, uint32_t offset, uint32_t size)
    : data_(NULL), size_(size), position_(0), own_(true) {
  copy(data, offset, size);
}

ByteBuffer::ByteBuffer(const ByteBuffer& rhs)
    : data_(NULL), size_(0), position_(0), own_(true) {
  reset();
  copy(rhs.data_, 0, rhs.size_);
}

ByteBuffer::~ByteBuffer() {
  free();
}

ByteBuffer & ByteBuffer::assign(const char* data,
                                uint32_t offset,
                                uint32_t size) {
  reset();
  copy(data, offset, size);
  return *this;
}

ByteBuffer & ByteBuffer::wrap(char* data, uint32_t offset, uint32_t size) {
  reset();
  own_ = false;
  data_ = data + offset;
  size_ = size;
  return *this;
}


ByteBuffer & ByteBuffer::operator=(const ByteBuffer& rhs) {
  if (this == &rhs) {
    return *this;
  } else {
    reset();
    copy(rhs.data_, 0, rhs.size_);
  }
  return *this;
}

ByteBuffer & ByteBuffer::copy(const char* data,
                              uint32_t offset,
                              uint32_t size) {
  reset();
  size_ = size;
  if (size_ > 0)  {
    data_ = allocate(size_);
    memcpy(data_, data + offset, size_);
  }
  return *this;
}

char * ByteBuffer::allocate(uint32_t size) const {
  char * data = (char*)malloc(size);  // NOLINT
  assert(data);
  return data;
}

void ByteBuffer::free() {
  if (own_ && data_) ::free(data_);
  data_ = NULL;
}

void ByteBuffer::reset() {
  free();
  position_ = 0;
  size_ = 0;
  own_ = true;
}

void ByteBuffer::reset(uint32_t size) {
  free();
  size_ = size;
  if (size_ > 0)  {
    data_ = allocate(size_);
  }
}

uint32_t ByteBuffer::position(uint32_t p) throw(ByteBuffer::out_of_range) {
    if (p > size_) throw out_of_range(position_, p, size_);
    uint32_t oldp = position_;
    position_ = p;
    return oldp;
}

ByteBuffer & ByteBuffer::put(const char* src,
                             uint32_t offset,
                             uint32_t size) throw(ByteBuffer::out_of_range) {
  if (position_ + size  > size_) throw out_of_range(position_, size, size_);
  memcpy(data_ + position_, src + offset, size);
  position_ += size;
  return *this;
}

ByteBuffer & ByteBuffer::putString(const std::string & v)
    throw(ByteBuffer::out_of_range) {
  put(v.size() + 1);
  put(v.c_str(), 0, v.size() + 1);
  return *this;
}

ByteBuffer & ByteBuffer::getString(std::string & v)
    throw(ByteBuffer::out_of_range) {
  uint32_t size = 0;
  get(size);
  if (size > 0) {
    char * data = new char[size];
    get(data, 0, size);
    v.assign(data);
    delete []data;
  }
  return *this;
}

ByteBuffer & ByteBuffer::get(char* dst,
                             uint32_t offset,
                             uint32_t size) throw(ByteBuffer::out_of_range) {
  if (position_ + size > size_) throw out_of_range(position_, size, size_);
  memcpy(dst + offset, data_ + position_, size);
  position_ += size;
  return *this;
}

ByteBuffer & ByteBuffer::get(int index, char* dst,
                             uint32_t offset,
                             uint32_t size) throw(ByteBuffer::out_of_range) {
  position_ = index;
  return get(dst, offset, size);
}


// you need use this method very carefully.
// it does not copy data_ to dst, only make dst point to right position
ByteBuffer & ByteBuffer::getRef(int index, const char* &dst,
                                uint32_t size) throw(out_of_range) {
  rawData(index, dst, size);
  if (index < 0) position_ += size;
  else
    position_ = index + size;
  return *this;
}

const ByteBuffer & ByteBuffer::rawData(int index, const char* &dst,
    uint32_t size) const throw(out_of_range) {
  if (index < 0) index = position_;
  if (index + size > size_) throw out_of_range(index, size, size_);
  dst = data_ + index;
  return *this;
}
