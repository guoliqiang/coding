// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#include "third_part/file_monitor/public/hash_batch_calculator.h"
#include <queue>
#include <algorithm>
#include "base/public/mutex.h"
#include "file/public/file_posix.h"
#include "third_part/crypto/public/md5.h"

namespace util {
const int KFileBufferLength = 1024;

HashBatchCalculator::HashBatchCalculator(ContentObserver* observer)
  : observer_(observer), status_(IDLE) {
  mutex_.reset(new base::Mutex());
}

HashBatchCalculator::~HashBatchCalculator() {
  base::MutexLock lock(mutex_.get());
  filereqmap_.clear();
}

std::string HashBatchCalculator::CalFileMd5(const std::string& filepath) {
  std::string str;
  if (!file::FileBase::Exists(filepath) ||
      file::FileBase::IsDir(filepath)) {
    return str;
  }

  crypto::MD5Context context;
  crypto::MD5Digest digest;
  crypto::MD5Init(&context);
  std::string buffer;
  file::FileBase* file_handler = file::FilePosix::Open(filepath,
                                                       file::FileBase::kRead);
  while (file_handler->Read(KFileBufferLength, &buffer).ok() &&
         !buffer.empty() &&
         status_ == RUNNING) {
    crypto::MD5Update(&context, buffer.c_str(), buffer.size());
  }
  delete file_handler;
  if (status_ != RUNNING) {
    LOG(WARNING) << "MD5-calculating thread is forced to quit now!";
    return str;
  }
  crypto::MD5Final(&digest, &context);
  int dig_length = sizeof(digest.a);
  for (int i = 0; i < dig_length; i++) {
    str.append(StringPrintf("%02x", digest.a[i]));
  }
  return str;
}


void HashBatchCalculator::Run() {
  status_ = RUNNING;
  std::queue<std::pair<std::string, Md5MappingMap> > queue;
  while (status_ == RUNNING) {
    VLOG(7) << "HashBatchCalculator::Run";
    mutex_->Lock();
    // copy entry in map to a temp queue
    Md5CalculatingMap::iterator iter = filereqmap_.begin();
    while (iter != filereqmap_.end()) {
      std::string file = iter->first;
      Md5MappingMap md5map;
      md5map.swap(iter->second);
      queue.push(std::make_pair(file, md5map));
      filereqmap_.erase(iter++);
    }
    filereqmap_.clear();
    mutex_->Unlock();
    while (!queue.empty()) {
      std::string file = queue.front().first;
      VLOG(7) << "file under md5 calculating: " << file;
      if (!file::FileBase::Exists(file)
          ||file::FileBase::IsDir(file)) {
        LOG(WARNING) << "File " << file << " is unavailable";
        queue.pop();
        continue;
      }
      std::string latestmd5 = CalFileMd5(queue.front().first);
      if (status_ == QUITING) {
        status_ = QUIT;
        return;
      }
      Md5MappingMap::iterator miter = queue.front().second.begin();
      while (miter != queue.front().second.end()) {
        if (miter->second == "") {
          VLOG(6) << "update md5 value: " << queue.front().first;
          observer_->UpdateHashValue(queue.front().first,
                                     latestmd5,
                                     miter->first);
        } else if (miter->second != latestmd5) {
          VLOG(6) << "md5 changed, callback later "<< queue.front().first;
          observer_->ContentChanged(queue.front().first,
                                    latestmd5,
                                    miter->first);
        }
        miter++;
      }
      queue.pop();
    }
    if (status_ == RUNNING) {
      sleep(HashCalSleepingSec);
    }
  }
  status_ = QUIT;
  LOG(INFO) << "HashBatchCalculator thread stops";
}

bool HashBatchCalculator::AddFile(const std::string& filepath,
                                  const std::string& md5value,
                                  int wd) {
  VLOG(5) << "HashBatchCalculator::AddFile in, WD is " << wd
          << ", path is " << filepath
          << ", md5 is " << md5value;
  using std::string;
  base::MutexLock lock(mutex_.get());
  Md5CalculatingMap::iterator iter = filereqmap_.find(filepath);
  bool ret(false);
  if (iter == filereqmap_.end()) {
    VLOG(6) << "file " << filepath
            << " was added into Hash calculator map";
    Md5MappingMap md5map;
    md5map.insert(std::make_pair(wd, md5value));
    filereqmap_.insert(std::make_pair(filepath, md5map));
    ret = true;
  } else {
    Md5MappingMap::iterator siter = iter->second.find(wd);
    if (siter == iter->second.end()) {
      iter->second.insert(std::make_pair(wd, md5value));
    }
  }
  iter = filereqmap_.find(filepath);
  CHECK(iter != filereqmap_.end());
  VLOG(7) << "HashBatchCalculator::AddFile out";
  return ret;
}

void HashBatchCalculator::StopRunning() {
  status_ = QUITING;
  while (status_ != QUIT) {
    sleep(ThreadQuittingTimeoutSec);
  }
  return;
}
}
