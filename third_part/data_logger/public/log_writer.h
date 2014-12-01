// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef _LOG_WRITER_H_
#define _LOG_WRITER_H_

#include <stdint.h>
#include "third_part/data_logger/public/log_format.h"
#include "third_part/fast_file_rw/public/status.h"
#include "third_part/fast_file_rw/public/env.h"

namespace data_logger {

using fast_file_rw::Status;
using fast_file_rw::WritableFile;

class Writer {
 public:
  // Create a writer that will append data to "*dest".
  // "*dest" must be initially empty.
  // "*dest" must remain live while this Writer is in use.
  explicit Writer(WritableFile* dest);
  ~Writer();

  Status AddRecord(const std::string& slice);

 private:
  WritableFile* dest_;
  int block_offset_;       // Current offset in block

  // crc32c values for all supported record types.  These are
  // pre-computed to reduce the overhead of computing the crc of the
  // record type stored in the header.
  uint32_t type_crc_[kMaxRecordType + 1];

  fast_file_rw::Status EmitPhysicalRecord(RecordType type, const char* ptr, size_t length);

  // No copying allowed
  Writer(const Writer&);
  void operator=(const Writer&);
};

}  // namespace data_logger

#endif  // _LOG_WRITER_H_
