// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_FILE_MONITOR_COMMON_H_
#define UTIL_FILE_MONITOR_FILE_MONITOR_COMMON_H_
namespace util {
typedef uint32 WatchType;
enum ThreadStatus { IDLE,
                     RUNNING,
                     QUITING,
                     QUIT,
};

enum WatchEventType { INVALIDEVENT = -1,
                      MODIFYEVENT = 1,
                      CREATEEVENT,
                      DELETEEVENT,
                      SINGLEDELETEEVENT,
};

// Register watch type
const WatchType KNormalDir = 0b0010;
const WatchType KRecursiveDir = 0b0100;
const WatchType KDelegateDir = 0b1000;
const WatchType KNormalFile = 0b0001;

// Timeout
const int WatchTimeoutMicroSec = 1000*1000;
const int CallbackTimeoutSec = 1;
const int CallbackEventWaitingSec = 1;
const int HashCalSleepingSec = 1;
const int EventPipeLineSleepingSec = 1;
const int ThreadQuittingTimeoutSec = 1;
const int64 CoolingTimeoutSec = 1;
}
#endif  // UTIL_FILE_MONITOR_FILE_MONITOR_COMMON_H_
