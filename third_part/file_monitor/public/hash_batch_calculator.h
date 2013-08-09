// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_HASH_BATCH_CALCULATOR_H_
#define UTIL_FILE_MONITOR_HASH_BATCH_CALCULATOR_H_

#include <map>
#include <string>
#include <vector>
#include "base/public/thread.h"
#include "base/public/scoped_ptr.h"
#include "third_part/file_monitor/public/content_observer.h"
#include "third_part/file_monitor/public/file_monitor_type.h"

namespace base {
class Mutex;
}  // namespace base

namespace util {

class HashBatchCalculator : public base::Thread {
 public:
  explicit HashBatchCalculator(ContentObserver* observer);
  ~HashBatchCalculator();

 protected:
  virtual void Run();
 
 public:
  bool AddFile(const std::string& filepath,
               const std::string& currentmd5,
               int wd);
  void StopRunning();
 
 private:
  std::string CalFileMd5(const std::string& filepath);
 
 private:
  ContentObserver*        observer_;
  ThreadStatus            status_;
  // map<string = filepath, map<int = fd, string = md5> >
  // a filepath may respect to multi fd.
  Md5CalculatingMap       filereqmap_;
  scoped_ptr<base::Mutex> mutex_;
  DISALLOW_COPY_AND_ASSIGN(HashBatchCalculator);
};

}  // namespace util
#endif  // UTIL_FILE_MONITOR_HASH_BATCH_CALCULATOR_H_
