// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 05:19:17
// File  : deserializer.cc
// Brief :

#include "third_part/raw_merge/public/deserializer.h"

#include <string>
#include <algorithm>
#include <vector>
#include "file/public/file.h"
#include "base/public/logging.h"

namespace raw_merge {

Deserializer::Deserializer(const std::string & dir, bool read_last_raw) {
  dir_ = dir;
  has_raw_ = false;
  has_merge_ = false;
  mergeid_ = -1;
  rawid_ = -1;
  index_ = -1;

  std::vector<std::string> tmp;
  file::File::GetFilesInDir(dir, &tmp);
  std::sort(tmp.begin(), tmp.end());  // merge.* before raw.*
  int max_rawid = -1;
  int max_mergeid = -1;

  for (int i = 0; i < tmp.size(); i++) {
    std::string name = file::File::BaseName(tmp[i]);
    if (IsRawFile(name)) {
      max_rawid = std::max(max_rawid, GetRawid(name));
    } else if (IsMergeFile(name)) {
      max_mergeid = std::max(max_mergeid, GetMergeid(name));
    }
  }
  if (!read_last_raw) max_rawid -= 1;

  for (int i = 0; i < tmp.size(); i++) {
    std::string name = file::File::BaseName(tmp[i]);
    if (IsRawFile(name)) {
      int rawid = GetRawid(name);
      if (rawid > max_mergeid && rawid <= max_rawid)  {
        has_raw_ = true;
        files_.push_back(tmp[i]);
      }
    } else if (IsMergeFile(name)) {
      int mergeid = GetMergeid(name);
      if (mergeid == max_mergeid) {
        has_merge_ = true;
        files_.push_back(tmp[i]);
      }
    }
  }
  mergeid_ = max_mergeid < 0 ? -1 : max_mergeid;
  rawid_ = max_rawid < 0 ? -1 : max_rawid;
}

bool Deserializer::Deserialize(std::string * record, std::string * scrach) {
  base::MutexLock lock(&mutex_);
  while (log_reader_.get() == NULL ||
         !log_reader_->ReadRecord(record, scrach)) {
    index_++;
    if (index_ == files_.size()) return false;

    fast_file_rw::Env * env = fast_file_rw::Env::Default();
    fast_file_rw::SequentialFile * read = NULL;
    CHECK(env->NewSequentialFile(files_[index_], &read).ok());
    log_reader_.reset(new data_logger::Reader(read, NULL, true, 0));
    fd_.reset(read);
  }
  return true;
}

}  // namespace raw_merge
