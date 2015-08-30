// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 05:15:12
// File  : deserializer.h
// Brief :

#ifndef  THIRD_PART_RAW_MERGE_PUBLIC_DESERIALIZER_H_
#define  THIRD_PART_RAW_MERGE_PUBLIC_DESERIALIZER_H_

#include "base/public/logging.h"
#include "base/public/mutex.h"
#include "base/public/shared_ptr.h"
#include "base/public/string_util.h"
#include "third_part/fast_file_rw/public/env.h"
#include "third_part/data_logger/public/log_reader.h"

namespace raw_merge {

class Deserializer {
 public:
  Deserializer(const std::string & dir, bool read_last_raw = false);
  bool Deserialize(std::string * record, std::string * scrach);
  virtual ~Deserializer() {}
  int GetMergeid() { return mergeid_; }
  int GetRawid() { return rawid_; }
  bool HasMergeFiles() { return has_merge_; }
  bool HasRawFiles() { return has_raw_; }

 private:
  bool IsMergeFile(const std::string & name) {
    return StartsWithASCII(name, "merge.", true);
  }
  bool IsRawFile(const std::string & name) {
    return StartsWithASCII(name, "raw.", true);
  }
  int GetRawid(const std::string & name) {
    return StringToInt(name.substr(name.find('.') + 1));
  }
  int GetMergeid(const std::string & name, int * subid = NULL) {
    std::string tmp = name.substr(name.find('.') + 1);
    int rs = StringToInt(tmp.substr(0, tmp.find('.')));
    if (subid != NULL) {
      *subid = StringToInt(tmp.substr(tmp.find('.') + 1));
    }
    return rs;
  }

 private:
  std::string dir_;
  int mergeid_;
  int rawid_;
  bool has_raw_;
  bool has_merge_;
  int index_;
  std::vector<std::string> files_;
  base::shared_ptr<fast_file_rw::SequentialFile> fd_;
  base::shared_ptr<data_logger::Reader> log_reader_;
  base::Mutex mutex_;
};

}  // namespace raw_merge

#endif  // THIRD_PART_RAW_MERGE_PUBLIC_DESERIALIZER_H_
