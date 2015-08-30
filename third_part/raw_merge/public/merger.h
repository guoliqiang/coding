// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 20:40:37
// File  : merger.h
// Brief :

#ifndef  THIRD_PART_RAW_MERGE_PUBLIC_MERGER_H_
#define  THIRD_PART_RAW_MERGE_PUBLIC_MERGER_H_

// usage:
// {
//   Merger merge(dir);  // local variable
//   if (merge.Init()) {
//     while (merge.Deserialize(...)) {
//       ....
//     }
//     ....
//     mege.Serialize(...);
//   }
// }

#include <string>
#include "third_part/raw_merge/public/serializer.h"
#include "third_part/raw_merge/public/deserializer.h"

namespace raw_merge {

class Merger {
 public:
  Merger(const std::string & dir) : dir_(dir) {}
  virtual ~Merger() {}
  bool Init(int merge_file_size = 200 * 1000 * 1000,
            bool read_last_raw = false);
  void Serialize(const std::string & str);
  bool Deserialize(std::string * record, std::string * scrach);

 private:
  std::string dir_;
  base::shared_ptr<Serializer> serializer_;
  base::shared_ptr<Deserializer> deserializer_;
};

}  // namespace raw_merge

#endif  // THIRD_PART_RAW_MERGE_PUBLIC_MERGER_H_
