// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-03 02:08:34
// File  : id_worker.h
// Brief : http://itindex.net/detail/53406-twitter-id-%E7%AE%97%E6%B3%95
// multi server need to use NTP to sync timestamp.
// http://www.ntp.org/ntpfaq/NTP-s-def.htm

#ifndef  THIRD_PART_SNOWFLAKE_PUBLIC_ID_WORKER_H_
#define  THIRD_PART_SNOWFLAKE_PUBLIC_ID_WORKER_H_

#include "base/public/basictypes.h"
#include "base/public/logging.h"
#include "base/public/time.h"
#include "base/public/mutex.h"

namespace snowflake {

class IDWorker {
 public:
  IDWorker(int64_t machine_id);
  int64_t NextID();

 private:
  int64_t machine_id_;
  int64_t sequence_;
  int64_t last_timestamp_;
  base::Mutex mutex_;

 private:
  void WaitNextMs();

  static const int32_t sequence_bit_ = 12;
  static const int32_t machine_id_bit_ = 10;
  static const int64_t machine_id_mask_ = (1L << machine_id_bit_) - 1;
  static const int64_t sequence_mask_ = (1L << sequence_bit_) - 1;
  static const int32_t timestamp_shift_ = machine_id_bit_ + sequence_bit_;
  static const int32_t machine_id_shift_ = sequence_bit_;
};

}  // namespace snowflake

#endif  // THIRD_PART_SNOWFLAKE_PUBLIC_ID_WORKER_H_
