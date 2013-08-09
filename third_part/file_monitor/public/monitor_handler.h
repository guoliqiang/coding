// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#ifndef UTIL_FILE_MONITOR_MONITOR_HANDLER_H_
#define UTIL_FILE_MONITOR_MONITOR_HANDLER_H_

#include <map>
#include <vector>
#include <string>
#include "base/public/singleton.h"
#include "base/public/callback.h"
#include "base/public/scoped_ptr.h"
#include "base/public/mutex.h"
#include "third_part/file_monitor/public/file_monitor_type.h"
#include "third_part/file_monitor/public/monitor_observer.h"
#include "third_part/file_monitor/public/content_observer.h"

namespace util {
class FileWatcher;
class HashBatchCalculator;
class CallbackExecuter;
class InotifyEventPipeline;

class FileMonitor : public MonitorObserver, public ContentObserver {
 private:
  FileMonitor();
  ~FileMonitor();

  friend struct DefaultSingletonTraits<FileMonitor>;

 public:
  // interfaces below are exploded to other threads or unpredictable user
  // use internal mutex to keep thread safe

  // from MonitorObserver
  void HandleInotifyEvent(int wd,
                          WatchEventType reason,
                          const std::string& name);
  // from Md5Observer
  bool ContentChanged(const std::string& filepath,
                      const std::string& value,
                      int wd);
  bool UpdateHashValue(const std::string& filepath,
                       const std::string& md5value,
                       int wd);
  // for pipeline
  void HandleReviveEvent(const EventParam& param);
  void HandleEvent(const EventParam& param);

  // the commmon interface to register file monitor
  // Usage:
  // {
  //   util::FileMonitor* monitor = util::FileMonitor::GetInstance();
  //   monitor->Register(_your_target, _your_callback_);
  //   ...
  // }

  bool Register(const std::string& path, base::Closure* callback);

 private:
  std::string AddSlashForDir(const std::string& dir);
  std::string RemoveEndingSlash(const std::string& path);
  // non-block update md5 value
  bool UpdateHashValueNonBlock(const std::string& filepath,
                               const std::string& md5value,
                               int wd);
  bool GetRegisteredPath(int wd, std::string* path);
  bool FindInWatchMap(int wd, WDMap::iterator* witer);
  bool FindInFileMap(const std::string& path, FileMap::iterator* fiter);
  bool FindInRegularMap(int wd,
                        WDMap::iterator* witer,
                        FileMap::iterator* fiter);
  bool FindInDelegateMap(int wd, DelegateMap::iterator* diter);
  bool GetFullPath(int wd,
                   const std::string& name,
                   std::string* fullpath);
  std::string GetValidParent(const std::string& path);
  std::string GetParentDirName(const std::string& path);
  void GetAllRecursiveDir(const std::string& parent,
                          std::vector<std::string>* dir);
  // register the top level file in a dir, no mention recursive cases
  void RegisterTopFiles(const std::string& dirpath, int wd);
  void RegisterRecursively(const std::string& parent, int wd);
  void HandleCreateEvent(const EventParam& param);
  void HandleCreateDir(const EventParam& param);
  void HandleCreateFile(const EventParam& param);
  void HandleDeleteEvent(const EventParam& param);
  void HandleModifyEvent(const EventParam& param);
  void HandleSingleDeleteEvent(const EventParam& param);
  void HandleReviveDir(const std::string& path,
                       CallbackSet callback);
  void HandleReviveFile(const std::string& path,
                        CallbackSet callback);

  // add a delegate for singledeleted target at once
  void PreprocessSingleDeleteEvent(int wd, const std::string& name);

  // when a decedent watching target revives, clean up related delegation
  bool UpdateDelegate(int wd);

  // for delegate dir use
  // when a sub dir of a delegating dir is created, and the sub dir
  // can delegate a decedant watching target, transfer the delegation
  // to the sub dir
  void TransferDelegate(const std::string& path,
                        int wd,
                        const std::string& decepath,
                        const CallbackSet& callback);
  bool ReviveManually(int wd, const std::string& fullpath);
 public:
  static FileMonitor* GetInstance();
 
 private:
  scoped_ptr<FileWatcher>            watchworker_;
  scoped_ptr<HashBatchCalculator>    calcworker_;
  scoped_ptr<CallbackExecuter>       callbackworker_;
  scoped_ptr<InotifyEventPipeline>   eventrworker_;
  scoped_ptr<base::Mutex>            mutex_for_fm_;
  // map<int, RegisterNode = {path,WatchType,Callbackset}>
  WDMap                              watchmap_;
  // map<string = dirpath/filepath, map<string = filepath, FileInfoValue = {size md5}> >
  FileMap                            filemap_;
  // map<int, map<string =dirpath/filepath, Callbackset> >
  DelegateMap                        delegatemap_;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileMonitor);
};
}

#endif  // UTIL_FILE_MONITOR_MONITOR_HANDLER_H_
