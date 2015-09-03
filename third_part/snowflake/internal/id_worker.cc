// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-03 02:22:28
// File  : id_worker.cc
// Brief :

#include "third_part/snowflake/public/id_worker.h"

namespace snowflake {

IDWorker::IDWorker(int64_t machine_id) {
  machine_id_ = machine_id & machine_id_mask_;
  sequence_ = 0;
  last_timestamp_ = 0;
};

void IDWorker::WaitNextMs() {
  int64_t cur = 0;
  do {
    cur = base::GetTimeInMs();
  } while (cur <= last_timestamp_);
  last_timestamp_ = cur;
}

int64_t IDWorker::NextID() {
  base::MutexLock lock(&mutex_);
  int64_t cur = base::GetTimeInMs();
  if (last_timestamp_ == cur) {
    sequence_ = (sequence_ + 1) & sequence_mask_;
    if (sequence_ == 0) WaitNextMs();
  } else {
    sequence_ = 0;
  }
  last_timestamp_ = cur;
  int64_t id = 0;
  id = (last_timestamp_ << timestamp_shift_) |
       (machine_id_  << machine_id_shift_) | sequence_;
  return id;
}


}  // namespace snowflake
