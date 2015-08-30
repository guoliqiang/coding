// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 02:43:54
// File  : Serializer.h
// Brief :

#ifndef  THIRD_PART_RAW_MERGE_PUBLIC_SERIALIZER_H_
#define  THIRD_PART_RAW_MERGE_PUBLIC_SERIALIZER_H_

#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/shared_ptr.h"
#include "base/public/string_util.h"
#include "third_part/fast_file_rw/public/env.h"
#include "third_part/data_logger/public/log_writer.h"

namespace raw_merge {

class Serializer {
 public:
  Serializer(const std::string & dir, int size = 200 * 1000 * 1000,
             int last_merge_rawid = -1);
  virtual ~Serializer();
  void Serialize(const std::string & str);
  bool RotateFile();

 private:
  bool RotateFileInternal(bool force = false);

 private:
  std::string dir_;
  int size_;
  int write_size_;
  int seq_num_;
  int last_merge_rawid_;
  std::vector<std::string> tmp_files_;
  base::Mutex mutex_;
  base::shared_ptr<fast_file_rw::WritableFile> fd_;
  base::shared_ptr<data_logger::Writer> log_writer_;
};

}  // namespace raw_merge

#endif  // THIRD_PART_RAW_MERGE_PUBLIC_SERIALIZER_H_
