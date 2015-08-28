// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 20:46:44
// File  : merger.cc
// Brief :

#include "third_part/raw_merge/public/merger.h"

namespace raw_merge {

bool Merger::Init(int merge_file_size /*= 200 * 1000 * 1000*/,
                  bool read_last_raw /*= false*/) {
  deserializer_.reset(new Deserializer(dir_, read_last_raw));
  if (!deserializer_->HasRawFiles()) return false;
  serializer_.reset(new Serializer(dir_, merge_file_size,
                    deserializer_->GetRawid()));
  return true;
}

void Merger::Serialize(const std::string & str) {
  serializer_->Serialize(str);
}

bool Merger::Deserialize(std::string * record, std::string * scrach) {
  return deserializer_->Deserialize(record, scrach);
}

}  // namespace raw_merge
