// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 03:39:03
// File  : serializer.cc
// Brief :

#include "third_part/raw_merge/public/serializer.h"

#include <vector>
#include <string>
#include <algorithm>
#include "file/public/file.h"
#include "base/public/logging.h"

namespace raw_merge {

Serializer::Serializer(const std::string & dir, int size,
                       int last_merge_rawid) {
  dir_ = dir;
  size_ = size;

  write_size_ = 0;
  seq_num_ = -1;
  last_merge_rawid_ = last_merge_rawid;

  if (last_merge_rawid_ == -1) {  // write raw file
    std::vector<std::string> files;
    file::File::GetFilesInDir(dir_, &files);
    for (int i = 0; i < files.size(); i++) {
      std::string name = file::File::BaseName(files[i]);
      if (StartsWithASCII(name, "raw.", true)) {
        int num = StringToInt(name.substr(name.find('.') + 1));
        seq_num_ = std::max(seq_num_, num);
      }
    }
  }
  RotateFile();
}

Serializer::~Serializer() {
  for (std::vector<std::string>::reverse_iterator i = tmp_files_.rbegin();
       i != tmp_files_.rend(); i++) {
    std::string old_path = dir_ + "/" + *i;
    std::string new_path = dir_ + "/" + i->substr(i->find('.') + 1);
    file::File::MoveFile(old_path, new_path);
  }
}

void Serializer::Serialize(const std::string & str) {
  base::MutexLock lock(&mutex_);
  log_writer_->AddRecord(str);
  write_size_ += str.size();
  RotateFileInternal();
}

bool Serializer::RotateFile() {
  base::MutexLock lock(&mutex_);
  return RotateFileInternal(true);
}

bool Serializer::RotateFileInternal(bool force) {
  if (log_writer_.get() == NULL || force || write_size_ > size_) {
    seq_num_++;
    fast_file_rw::Env * env = fast_file_rw::Env::Default();
    fast_file_rw::WritableFile * write = NULL;

    std::string path;
    if (last_merge_rawid_ != -1) {
      path = dir_ + "/tmp.merge." + StringPrintf("%08d", last_merge_rawid_) +
             "." + StringPrintf("%06d", seq_num_);
      tmp_files_.push_back(path.substr(path.rfind('/') + 1));
    } else {
      path = dir_ + "/raw." + StringPrintf("%08d", seq_num_);
    }
    CHECK(env->NewWritableFile(path, &write).ok());
    log_writer_.reset(new data_logger::Writer(write));
    fd_.reset(write);
    write_size_ = 0;
    return true;
  }
  return false;
}

}  // namespace raw_merge
