// Copyright 2011 Yunrang Inc. All Rights Reserved
// Author : bingbai@yunrang.com (Bing Bai)

#include "third_part/file_monitor/public/monitor_handler.h"
#include <algorithm>
#include <queue>
#include "base/public/string_util.h"
#include "base/public/shared_ptr.h"
#include "file/public/file_base.h"
#include "file/public/file.h"
#include "third_part/file_monitor/public/file_watcher.h"
#include "third_part/file_monitor/public/hash_batch_calculator.h"
#include "third_part/file_monitor/public/callback_executer.h"
#include "third_part/file_monitor/public/event_pipeline.h"

namespace util {

FileMonitor* FileMonitor::GetInstance() {
  return Singleton<FileMonitor>::get();
}

FileMonitor::FileMonitor() {
  mutex_for_fm_.reset(new base::Mutex());
  watchworker_.reset(new FileWatcher(this));
  calcworker_.reset(new HashBatchCalculator(this));
  callbackworker_.reset(new CallbackExecuter());
  eventrworker_.reset(new InotifyEventPipeline(this));
  callbackworker_->Start();
  calcworker_->Start();
  eventrworker_->Start();
  watchworker_->Start();
}

FileMonitor::~FileMonitor() {
  watchworker_->StopRunning();
  eventrworker_->StopRunning();
  calcworker_->StopRunning();
  callbackworker_->StopRunning();
  base::MutexLock lock(mutex_for_fm_.get());
  std::map<int, struct RegisterNode>::iterator iter = watchmap_.begin();
  while (iter != watchmap_.end()) {
    watchworker_->RemoveWatching(iter->first);
    iter++;
  }
  watchmap_.clear();
  delegatemap_.clear();
  filemap_.clear();
}

std::string FileMonitor::AddSlashForDir(const std::string& dir) {
  std::string dirpath = dir;
  if (dir.find_last_of("/") != dir.length()-1) {
    dirpath.append("/");
  }
  return dirpath;
}

void FileMonitor::RegisterRecursively(const std::string& parent, int wd) {
  std::vector<std::string> dirs;
  GetAllRecursiveDir(parent, &dirs);
  for (int i = 0; i < dirs.size(); i++) {
    int swd = watchworker_->AddMonitor(dirs[i]);
    if (swd > 0) {
      WDMap::iterator witer = watchmap_.find(swd);
      if (witer == watchmap_.end()) {
        RegisterNode node;
        node.path = dirs[i];
        node.type = KRecursiveDir;
        node.callbackset = watchmap_[wd].callbackset;
        watchmap_.insert(std::make_pair(swd, node));
        RegisterTopFiles(dirs[i], swd);
      } else {
        watchmap_[swd].type |= KRecursiveDir;
        std::copy(watchmap_[wd].callbackset.begin(),
                  watchmap_[wd].callbackset.end(),
                  std::back_inserter(watchmap_[swd].callbackset));
      }
    }
  }
}

void FileMonitor::RegisterTopFiles(const std::string& dirpath, int wd) {
  FileMap::iterator fiter = filemap_.find(dirpath);
  if (fiter != filemap_.end()) {
    return;
  }
  ContentMap content_map;
  std::vector<std::string> file_in_dir;
  if (!file::FileBase::GetFilesInDir(dirpath, &file_in_dir).ok()) {
    LOG(ERROR) << "can't get topfiles in " << dirpath;
    return;
  }
  for (int i = 0; i < file_in_dir.size(); i++) {
    std::string file = file_in_dir[i];
    FileInfoValue value;
    if (file::File::FileSize(file, &(value.filesize))) {
      calcworker_->AddFile(file, "", wd);
      content_map.insert(std::make_pair(file, value));
    }
  }
  filemap_.insert(std::make_pair(dirpath, content_map));
}

void FileMonitor::GetAllRecursiveDir(const std::string& parent,
                                     std::vector<std::string>* dir) {
  dir->clear();
  if (!file::FileBase::IsDir(parent)) {
    LOG(ERROR) << parent << " is not a valid dir!";
  }
  std::queue<std::string> dir_queue;
  dir_queue.push(parent);
  std::vector<std::string> subdir;
  while (!dir_queue.empty()) {
    if (!file::FileBase::GetDirsInDir(dir_queue.front(), &subdir).ok()) {
      LOG(ERROR) << "can't get sub dir!";
      break;
    }
    dir_queue.pop();
    if (subdir.empty()) {
      continue;
    }
    for (int i = 0; i < subdir.size(); i++) {
      subdir[i] = AddSlashForDir(subdir[i]);
      VLOG(6) << "subdir: " << subdir[i];
      dir_queue.push(subdir[i]);
    }
    std::copy(subdir.begin(), subdir.end(), std::back_inserter(*dir));
    subdir.clear();
  }
}

void FileMonitor::HandleEvent(const EventParam& param) {
  base::MutexLock lock(mutex_for_fm_.get());
  switch (param.type) {
    case MODIFYEVENT:
      VLOG(6) << "MODIFY";
      HandleModifyEvent(param);
      break;
    case CREATEEVENT:
      VLOG(6) << "CREATE";
      HandleCreateEvent(param);
      break;
    case DELETEEVENT:
      VLOG(6) << "DELETE";
      HandleDeleteEvent(param);
      break;
    case SINGLEDELETEEVENT:
      VLOG(6) << "SINGLEDELETE";
      HandleSingleDeleteEvent(param);
      break;
    default:
      LOG(ERROR) << "valid event";
      break;
  }
}

void FileMonitor::HandleCreateDir(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleCreateDir in";
  WatchType watch_type;
  CallbackSet callback_set;
  WDMap::iterator witer = watchmap_.find(param.wd);
  if (!FindInWatchMap(param.wd, &witer)) {
    return;
  }
  watch_type = witer->second.type;
  callback_set = witer->second.callbackset;
  std::string fullpath = AddSlashForDir(param.fullpath);
  if ((watch_type & KDelegateDir) == KDelegateDir) {
    // check it in delegatemap
    DelegateMap::iterator diter = delegatemap_.find(param.wd);
    if (diter == delegatemap_.end()) {
      LOG(ERROR) << "not registered in delegatemap";
      return;
    }
    DecedentMap::iterator dece_iter = diter->second.begin();
    bool transferred(false);
    while (dece_iter != diter->second.end()) {
      if (dece_iter->first.find(fullpath) != std::string::npos) {
        // revive in steps
        VLOG(7) << "fullpath: " << fullpath
                << ", dece.first: " << dece_iter->first;
        transferred = true;
        CallbackSet set = dece_iter->second;
        std::string path = dece_iter->first;
        VLOG(7) << "revived path: " << path;
        for (int i = 0; FLAGS_v >= 7 && i < set.size(); i++) {
          VLOG(7) << "revived set: " << set[i].get();
        }
        if (dece_iter->first == fullpath) {
          HandleReviveDir(param.fullpath, set);
        } else {
          TransferDelegate(param.fullpath,
                           param.wd,
                           path,
                           set);
        }
        diter->second.erase(dece_iter++);
        continue;
      }
      dece_iter++;
    }
    // clear watchmap & delegatemap
    if (transferred && diter->second.size() == 0) {
      WDMap::iterator witer;
      if (!FindInWatchMap(diter->first, &witer)) {
        delegatemap_.erase(diter);
        return;
      }
      delegatemap_.erase(diter);
      witer->second.type &= ~KDelegateDir;
      VLOG(6) << "remove delegate and clean up, wd: " << witer->first;
      if (witer->second.type == 0) {
        watchworker_->RemoveWatching(witer->first);
        watchmap_.erase(witer);
      }
    }
  } else {
    // register the recursive dir
    int new_wd = watchworker_->AddMonitor(param.fullpath);
    WDMap::iterator switer = watchmap_.find(new_wd);
    if (switer == watchmap_.end()) {
      VLOG(6) << "not yet";
      RegisterNode node;
      node.path = param.fullpath;
      node.type = KRecursiveDir;
      node.callbackset = callback_set;
      watchmap_.insert(std::make_pair(new_wd, node));
      RegisterTopFiles(param.fullpath, new_wd);
      RegisterRecursively(param.fullpath, new_wd);
    } else {
      VLOG(6) << "append call back";
      watchmap_[new_wd].type |= KRecursiveDir;
      std::copy(watchmap_[param.wd].callbackset.begin(),
                watchmap_[param.wd].callbackset.end(),
                std::back_inserter(watchmap_[new_wd].callbackset));
    }
    VLOG(6) << "new wd: " << param.wd;
    for (int i = 0;
         FLAGS_v >=7 && i < watchmap_[param.wd].callbackset.size();
         i++) {
      VLOG(7) << "FileMonitor::HandleCreateDir callback: "
              << watchmap_[param.wd].callbackset[i].get();
    }
  }
  VLOG(6) << "FileMonitor::HandleCreateDir out";
}

bool FileMonitor::FindInRegularMap(int wd,
                                   WDMap::iterator* witer,
                                   FileMap::iterator* fiter) {
  if (FindInWatchMap(wd, witer)
      && FindInFileMap(((*witer)->second.path), fiter)) {
    return true;
  }
  return false;
}

bool FileMonitor::FindInWatchMap(int wd, WDMap::iterator* witer) {
  *witer = watchmap_.find(wd);
  if (*witer == watchmap_.end()) {
    LOG(ERROR) << "Not registerred in watch map, wd: " << wd;
    return false;
  }
  return true;
}

bool FileMonitor::FindInFileMap(const std::string& path,
                                FileMap::iterator* fiter) {
  *fiter = filemap_.find(path);
  if (*fiter == filemap_.end()) {
    LOG(WARNING) << "Not registerred in file map, path: " << path;
    return false;
  }
  return true;
}

bool FileMonitor::FindInDelegateMap(int wd, DelegateMap::iterator* diter) {
  *diter = delegatemap_.find(wd);
  WDMap::iterator witer = watchmap_.find(wd);
  if (*diter == delegatemap_.end()
      || witer == watchmap_.end()
      || (witer->second.type & KDelegateDir) != KDelegateDir) {
    LOG(ERROR) << "not register in delegatemap, wd: " << wd;
    return false;
  }
  return true;
}

void FileMonitor::HandleCreateFile(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleCreateFile in";
  DelegateMap::iterator diter = delegatemap_.find(param.wd);
  if (diter != delegatemap_.end()) {
    DecedentMap::iterator dece_iter = diter->second.find(param.fullpath);
    if (dece_iter != diter->second.end()) {
      HandleReviveFile(param.fullpath, dece_iter->second);
      diter->second.erase(dece_iter++);
      if (UpdateDelegate(param.wd)) {
        delegatemap_.erase(diter);
      }
    }
  }
  FileInfoValue filenode;
  if (!file::File::FileSize(param.fullpath, &filenode.filesize)) {
    LOG(ERROR) << "can't get filesize now";
  }

  WDMap::iterator witer;
  FileMap::iterator fiter;
  CallbackSet set;
  std::string md5("");
  bool callback_atonce(true);
  if (!FindInRegularMap(param.wd, &witer, &fiter)) {
    LOG(WARNING) << "FileMonitor::HandleCreateFile can't handle as normal";
    return;
  }
  set = witer->second.callbackset;
  ContentMap::iterator miter = fiter->second.find(param.fullpath);
  if (miter == fiter->second.end()) {
    fiter->second[param.fullpath] = filenode;
    md5 = fiter->second[param.fullpath].md5value;
  } else {
    size_t size(0);
    if (file::File::FileSize(param.fullpath, &size)
        && size != miter->second.filesize) {
      md5 = "";
      miter->second.filesize = size;
    } else {
      md5 = miter->second.md5value;
      callback_atonce = false;
      VLOG(6) << "won't call back at once";
    }
  }
  VLOG(6) << "FileMonitor::HandleCreateFile handle create event, path: "
          << param.fullpath
          << ", callbackset size: " << witer->second.callbackset.size();
  calcworker_->AddFile(param.fullpath,
                       md5,
                       param.wd);
  if (callback_atonce) {
    callbackworker_->AddCallback(param.wd,
                                 param.fullpath,
                                 set,
                                 CREATEEVENT);
  }
  VLOG(6) << "FileMonitor::HandleCreateFile out";
}

void FileMonitor::HandleCreateEvent(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleCreateEvent in";
  if (!file::FileBase::Exists(param.fullpath)) {
    LOG(ERROR) << "Dir/File " << param.fullpath
               << " doesn't exist!";
    return;
  }
  if (file::FileBase::IsDir(param.fullpath)) {
    HandleCreateDir(param);
  } else {
    HandleCreateFile(param);
  }
  VLOG(6) << "FileMonitor::HandleCreateEvent out";
}

void FileMonitor::HandleModifyEvent(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleModifyEvent in";
  size_t size(0);
  CallbackSet set;
  std::string md5;
  // filemap_ : map<string = dirpath/filepath, map<string = filepath,
  //            FileInfoValue = {size md5}> >
  FileMap::iterator fiter = filemap_.end();
  // watchmap_: map<int, map<string =dirpath/filepath, Callbackset> >
  WDMap::iterator witer = watchmap_.end();
  if (!FindInRegularMap(param.wd, &witer, &fiter)) {
    return;
  }
  set = witer->second.callbackset;
  ContentMap::iterator citer = fiter->second.find(param.fullpath);
  if (citer == fiter->second.end()) {
    VLOG(ERROR) << "not registered in filemap. but has a modify event"
                << ", path: " << param.fullpath;
    return;
  }
  if (file::File::FileSize(param.fullpath, &size)
      && size != fiter->second[param.fullpath].filesize) {
      // direct call CallBack here
      callbackworker_->AddCallback(param.wd,
                                   param.fullpath,
                                   set,
                                   MODIFYEVENT);
      citer->second.filesize = size;
      citer->second.md5value = "";
  }
  md5 = citer->second.md5value;
  calcworker_->AddFile(param.fullpath,
                       md5,
                       param.wd);
  VLOG(6) << "FileMonitor::HandleModifyEvent out";
}

void FileMonitor::HandleDeleteEvent(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleDeleteEvent in";
  WDMap::iterator witer;
  FileMap::iterator fiter;
  CallbackSet set;
  if (!FindInRegularMap(param.wd, &witer, &fiter)) {
    return;
  }
  set = witer->second.callbackset;
  callbackworker_->AddCallback(param.wd,
                               param.fullpath,
                               set,
                               DELETEEVENT);
  fiter->second.erase(param.fullpath);
  // fullpath is the deleted file in some dir.
  VLOG(6) << "FileMonitor::HandleDeleteEvent out";
}

// for delegate dir use
// when a sub dir of a delegating dir is created, and the sub dir
// can delegate a decedant watching target, transfer the delegation
// to the sub dir
void FileMonitor::TransferDelegate(const std::string& path,
                                  int wd,
                                  const std::string& decepath,
                                  const CallbackSet& callback) {
  VLOG(6) << "FileMonitor::TransferDelegate in"
          << ", path: " << path
          << ", decepath: " << decepath;
  for (int i = 0; FLAGS_v >= 6 && i < callback.size(); i++) {
    VLOG(6) << "callback: " << callback[i].get();
  }
  DelegateMap::iterator diter = delegatemap_.find(wd);
  if (diter == delegatemap_.end()) {
    LOG(ERROR) << "can't find it";
    return;
  }
  int new_wd = watchworker_->AddMonitor(path);
  if (new_wd < 0) {
    LOG(ERROR) << "fail to register inotify: " << path;
    return;
  } else {
    VLOG(6) << "new delegate for " << decepath
            << ", wd: " << new_wd;
    if (file::FileBase::Exists(decepath)) {
      VLOG(7) << "some event was missing, revive mannully";
      EventParam param;
      param.wd = new_wd;
      param.fullpath = decepath;
      param.type = CREATEEVENT;
      eventrworker_->Push(param);
    }
  }
  DelegateMap::iterator dele_iter = delegatemap_.find(new_wd);
  CallbackSet set = callback;
  if (dele_iter == delegatemap_.end()) {
    DecedentMap decedent_map;
    decedent_map.insert(std::make_pair(decepath, set));
    delegatemap_[new_wd] = decedent_map;
  } else {
    dele_iter->second.insert(std::make_pair(decepath, set));
  }
  RegisterNode node;
  node.path = AddSlashForDir(path);
  node.type = KDelegateDir;
  bool has_other_charactor(false);
  WDMap::iterator witer;
  if (!FindInWatchMap(diter->first, &witer)) {
    return;
  }
  if ((witer->second.type & (KNormalDir|KRecursiveDir)) != 0) {
    VLOG(6) << "path: " << path << " has other charactors than delegate";
    node.type |= KRecursiveDir;
    node.callbackset = witer->second.callbackset;
    has_other_charactor = true;
  }
  WDMap::iterator dwiter;
  if (!FindInWatchMap(new_wd, &dwiter)) {
    watchmap_[new_wd] = node;
  } else {
    dwiter->second.type |= KDelegateDir;
  }
  if (has_other_charactor) {
    RegisterTopFiles(path, new_wd);
    RegisterRecursively(path, new_wd);
  }
  VLOG(6) << "FileMonitor::TransferDelegate out";
}

void FileMonitor::HandleReviveEvent(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleReviveEvent in";
  base::MutexLock lock(mutex_for_fm_.get());
  DelegateMap::iterator diter;
  if (!FindInDelegateMap(param.wd, &diter)) {
    return;
  }
  DecedentMap& decedent_map = diter->second;
  DecedentMap::iterator dece_iter = decedent_map.begin();
  while (dece_iter != decedent_map.end()) {
    if (dece_iter->first == param.fullpath
        &&file::FileBase::Exists(param.fullpath)) {
      file::FileBase::IsDir(param.fullpath) ?
        HandleReviveDir(param.fullpath, dece_iter->second)
        : HandleReviveFile(param.fullpath, dece_iter->second);
      VLOG(6) << "erase:" << dece_iter->first;
      decedent_map.erase(dece_iter++);
      if (UpdateDelegate(param.wd)) {
        VLOG(6) << "delegate is canceled: " << param.fullpath;
        delegatemap_.erase(diter);
        break;
      }
      continue;
    }
    dece_iter++;
  }
  VLOG(6) << "FileMonitor::HandleReviveEvent out";
}

// when a decedent watching target revives, clean up related delegation
// Note: not delete from DelegateMap
bool FileMonitor::UpdateDelegate(int wd) {
  VLOG(6) << "FileMonitor::UpdateDelegate in";
  DelegateMap::iterator diter;
  if (!FindInDelegateMap(wd, &diter)) {
    return false;
  }
  if (diter->second.empty()) {
    WDMap::iterator witer;
    if (!FindInWatchMap(wd, &witer)) {
      return false;
    }
    witer->second.type &= ~KDelegateDir;
    VLOG(6) << "delegate charactor of " << witer->second.path
            << " is removed";
    if (witer->second.type == 0) {
      // only plays as delagate before
      watchworker_->RemoveWatching(wd);
      watchmap_.erase(witer);
    }
    return true;
  }
  VLOG(6) << "FileMonitor::UpdateDelegate out";
  return false;
}

void FileMonitor::HandleReviveDir(const std::string& path,
                                  CallbackSet callback) {
  VLOG(6) << "FileMonitor::HandleReviveDir in";
  int wd = watchworker_->AddMonitor(RemoveEndingSlash(path));
  if (wd < 0) {
    LOG(ERROR) << "can't register in inotify, path: " << path;
    return;
  } else {
    VLOG(6) << "new wd for revived path: " << path
            << ", wd: " << wd;
  }
  RegisterNode node;
  node.path = path;
  node.type = KNormalDir;
  node.callbackset = callback;
  for (int i = 0; i < node.callbackset.size(); i++) {
    VLOG(6) << "node.callbackset:" << node.callbackset[i].get();
  }
  watchmap_[wd] = node;
  RegisterTopFiles(path, wd);
  RegisterRecursively(path, wd);
  VLOG(7) << "Path: " << path << ", wd : " << wd;
  for (int i = 0; FLAGS_v >= 7 && i < watchmap_[wd].callbackset.size(); i++) {
    VLOG(7) << "callback is " << watchmap_[wd].callbackset[i].get();
  }
  VLOG(6) << "FileMonitor::HandleReviveDir out";
}

void FileMonitor::HandleReviveFile(const std::string& path,
                                   CallbackSet callback) {
  VLOG(6) << "FileMonitor::HandleReviveFile in, path: "
          << path;
  int wd = watchworker_->AddMonitor(path);
  if (wd < 0) {
    LOG(ERROR) << "fail to add inotify, path:" << path;
    return;
  }
  RegisterNode node;
  node.path = path;
  node.type = KNormalFile;
  node.callbackset = callback;
  watchmap_[wd] = node;
  FileMap::iterator fiter = filemap_.find(path);
  if (fiter != filemap_.end()) {
    // check size/md5
    VLOG(6) << "check size/md5";
    size_t size(0);
    ContentMap::iterator citer = fiter->second.find(path);
    if (citer != fiter->second.end()
        &&file::File::FileSize(path, &size)
        && size != citer->second.filesize) {
      VLOG(6) << "revive and callback as modify";
      callbackworker_->AddCallback(wd, path, callback, MODIFYEVENT);
      citer->second.md5value = "";
      citer->second.filesize = size;
    }
  } else {
    VLOG(6) << "update filemap";
    FileInfoValue filenode;
    ContentMap content_map;
    if (!file::File::FileSize(path, &filenode.filesize)) {
      LOG(ERROR) << "can't get filesize now";
    }
    content_map[path] = filenode;
    filemap_[path] = content_map;
  }
  calcworker_->AddFile(path, filemap_[path][path].md5value, wd);
  VLOG(6) << "FileMonitor::HandleReviveFile out";
}

void FileMonitor::HandleSingleDeleteEvent(const EventParam& param) {
  VLOG(6) << "FileMonitor::HandleSingleDeleteEvent in";
  WDMap::iterator witer;
  CallbackSet callback;
  if (!FindInWatchMap(param.wd, &witer)) {
    return;
  }
  // only registered file will be kept in filemap
  // registered dir will be removed
  callback = witer->second.callbackset;
  if ((witer->second.type & KNormalFile) != KNormalFile) {
    VLOG(6) << "filemap erased for key: " << witer->second.path;
    filemap_.erase(witer->second.path);
  }
  watchmap_.erase(witer);
  if (file::FileBase::Exists(param.fullpath)) {
    // there must be a create event reviving this file
    // no need to callback for singledelete event
    return;
  }
  callbackworker_->AddCallback(param.wd,
                               param.fullpath,
                               callback,
                               SINGLEDELETEEVENT);
  VLOG(6) << "FileMonitor::HandleSingleDeleteEvent out";
}

bool FileMonitor::GetRegisteredPath(int wd, std::string* path) {
  path->clear();
  WDMap::iterator witer;
  if (FindInWatchMap(wd, &witer)) {
    *path = witer->second.path;
    return true;
  }
  return false;
}

// called by file_watcher thread
void FileMonitor::HandleInotifyEvent(int wd,
                                     WatchEventType reason,
                                     const std::string& name) {
  std::string reg_path;
  std::string fullpath;
  {
    base::MutexLock lock(mutex_for_fm_.get());
    if (!GetRegisteredPath(wd, &reg_path)) {
      LOG(ERROR) << "can't get registered path, wd: " << wd;
      return;
    }
    VLOG(5) << "FileMonitor::HandleInotifyEvent in:"
            << " wd: " << wd <<", registered path: " << reg_path
            << " reason: " << reason  << " name: " << name;
    if (!GetFullPath(wd, name, &fullpath)) {
      LOG(ERROR) << "can't get fullpath, wd:" << wd;
      return;
    }
    if (reason == SINGLEDELETEEVENT) {
      PreprocessSingleDeleteEvent(wd, name);
    }
  }

  EventParam param;
  param.wd = wd;
  param.fullpath = fullpath;
  param.type = reason;
  eventrworker_->Push(param);
  VLOG(6) << "FileMonitor::HandleInotifyEvent out";
}

std::string FileMonitor::RemoveEndingSlash(const std::string& path) {
  if (path.find_last_of('/') == path.size()-1) {
    return path.substr(0, path.size()-1);
  }
  return path;
}

bool FileMonitor::ReviveManually(int wd, const std::string& fullpath) {
  if (!file::FileBase::Exists(fullpath)
      || file::FileBase::IsDir(fullpath)) {
    return false;
  }
  VLOG(6) << "file: " << fullpath << " exists!";
  int new_wd = watchworker_->AddMonitor(fullpath);
  if (new_wd < 0) {
    LOG(ERROR) << "can't register file " << fullpath;
    return false;
  }
  WDMap::iterator witer = watchmap_.find(wd);
  if (witer == watchmap_.end()) {
    LOG(ERROR) << "wd " << wd
               << " was not registerred in watchmap";
    watchworker_->RemoveWatching(new_wd);
    return false;
  }
  watchmap_[new_wd] = witer->second;
  EventParam param;
  param.wd = new_wd;
  param.type = MODIFYEVENT;
  param.fullpath = fullpath;
  eventrworker_->Push(param);
  return true;
}

void FileMonitor::PreprocessSingleDeleteEvent(int wd,
                                              const std::string& name) {
  VLOG(6) << "FileMonitor::PreprocessSingleDeleteEvent in";
  std::string fullpath;
  if (!GetFullPath(wd, name, &fullpath)) {
    LOG(ERROR) << "can't get fullpath";
    return;
  }
  // watch its valid parent
  std::string parent = GetValidParent(fullpath);
  VLOG(6) << "parent is " << RemoveEndingSlash(parent);
  // child : wd , parent: pwd
  int pwd = watchworker_->AddMonitor(RemoveEndingSlash(parent));
  if (pwd < 0) {
    LOG(ERROR) << "can't register parent: " << parent
               << " of singledeleted path: " << fullpath;
    return;
  }
  WDMap::iterator witer = watchmap_.find(pwd);
  if (ReviveManually(wd, fullpath)) {
    if (witer == watchmap_.end()) {
      watchworker_->RemoveWatching(pwd);
    }
    return;
  }

  // update watch map
  CallbackSet set;
  if (witer == watchmap_.end()) {
    RegisterNode node;
    node.path = parent;
    node.type = KDelegateDir;
    watchmap_.insert(std::make_pair(pwd, node));
  } else {
    witer->second.type |= KDelegateDir;
  }
  witer = watchmap_.find(wd);
  set = witer->second.callbackset;
  // update delegatemap
  DelegateMap::iterator diter = delegatemap_.find(pwd);  // pwd
  if (diter == delegatemap_.end()) {
    DecedentMap dmap;
    dmap.insert(std::make_pair(fullpath, set));
    delegatemap_.insert(std::make_pair(pwd, dmap));
  } else {
    diter->second.insert(std::make_pair(fullpath, set));
  }

  // delegatemap : map<int, map<string, Callbackset> >
  DelegateMap::iterator dit = delegatemap_.find(wd);  // wd
  if (dit != delegatemap_.end()) {
    // continue delegate
    DecedentMap::iterator dece_it = dit->second.begin();
    while (dece_it != dit->second.end()) {
      DelegateMap::iterator pditor = delegatemap_.find(pwd);
      if (pditor == delegatemap_.end()) {
        DecedentMap decedent_map;
        decedent_map[dece_it->first] = dece_it->second;
        delegatemap_.insert(std::make_pair(pwd, decedent_map));
      } else {
        pditor->second.insert(std::make_pair(dece_it->first,
                                             dece_it->second));
      }
      VLOG(6) << "decedent handling for: " << dece_it->first;
      dece_it++;
    }
    delegatemap_.erase(dit);
  }
  VLOG(6) << "FileMonitor::PreprocessSingleDeleteEvent out";
}

bool FileMonitor::GetFullPath(int wd,
                              const std::string& name,
                              std::string* fullpath) {
  fullpath->clear();
  std::string dir("");
  WDMap::iterator witer = watchmap_.find(wd);
  if (witer == watchmap_.end()) {
    return false;
  }
  dir = witer->second.path;
  StringAppendF(fullpath, "%s%s", dir.c_str(), name.c_str());
  if (file::FileBase::Exists(*fullpath)
      &&file::FileBase::IsDir(*fullpath)) {
    *fullpath = AddSlashForDir(*fullpath);
  }
  return true;
}

std::string FileMonitor::GetValidParent(const std::string& path) {
  std::string parent = GetParentDirName(path);
  while (!parent.empty()) {
    if (file::FileBase::Exists(parent)) {
      break;
    }
    parent = GetParentDirName(parent);
  }
  return parent;
}

std::string FileMonitor::GetParentDirName(const std::string& filepath) {
  std::string path(filepath);
  if (filepath.find_last_of("/") == filepath.length()-1) {
    path = filepath.substr(0, filepath.size()-1);
  }
  return path.substr(0, path.find_last_of("/") + 1);
}


bool FileMonitor::Register(const std::string& path,
                           base::Closure* callback) {
  base::MutexLock lock(mutex_for_fm_.get());
  if (!callback) {
    return false;
  }
  // only monitor file specified by 'path', dir specified by 'path'
  // and the sub-dir contained by dir specified by 'path'
  int wd = watchworker_->AddMonitor(path);
  if (wd < 0) {
    delete callback;
    return false;
  }
  std::string pathstr = path;
  bool is_dir(false);
  if (file::FileBase::IsDir(path)) {
    pathstr = AddSlashForDir(path);
    is_dir = true;
  }
  // map<int, RegisterNode>
  WDMap::iterator iter = watchmap_.find(wd);
  // update watchmap
  base::shared_ptr<base::Closure> callback_ptr(callback);
  if (iter!= watchmap_.end()) {
    // update callback
    iter->second.callbackset.push_back(callback_ptr);
  } else {
    RegisterNode register_node;
    register_node.path = pathstr;
    register_node.type = is_dir? KNormalDir : KNormalFile;
    register_node.callbackset.push_back(callback_ptr);
    watchmap_.insert(std::make_pair(wd, register_node));
  }
  // update filemap
  // map<string, map<string, FileInfoValue> >
  FileMap::iterator fileiter = filemap_.find(pathstr);
  if (fileiter == filemap_.end()) {
    if (is_dir) {
      RegisterTopFiles(pathstr, wd);
      RegisterRecursively(pathstr, wd);
    } else {
      FileInfoValue file_info;
      ContentMap md5map;
      if (!file::File::FileSize(pathstr, &(file_info.filesize))) {
        LOG(ERROR) << "Fail to check " << pathstr << " size!";
      }
      md5map.insert(std::make_pair(pathstr, file_info));
      filemap_.insert(std::make_pair(pathstr, md5map));
      calcworker_->AddFile(pathstr, "", wd);
    }
  }
  return true;
}

bool FileMonitor::ContentChanged(const std::string& filepath,
                                 const std::string& value,
                                 int wd) {
  CallbackSet set;
  {
    base::MutexLock lock(mutex_for_fm_.get());
    if (!UpdateHashValueNonBlock(filepath, value, wd)) {
      LOG(ERROR) << "MD5 value update fails!";
      return false;
    }
    set = watchmap_[wd].callbackset;
  }
  VLOG(7) << "file: " << filepath
          << " new md5: " << value;
  callbackworker_->AddCallback(wd, filepath, set, MODIFYEVENT);
  return true;
}

bool FileMonitor::UpdateHashValueNonBlock(const std::string& filepath,
                                          const std::string& md5value,
                                          int wd) {
  std::string reg_path("");
  WDMap::iterator iter = watchmap_.end();
  if (!FindInWatchMap(wd, &iter)) {
    LOG(ERROR) << "INVALID WD";
    return false;
  }
  reg_path = iter->second.path;
  FileMap::iterator fileiter = filemap_.find(reg_path);
  if (fileiter == filemap_.end()) {
    LOG(ERROR) << "INVALID REGISTERRED PATH";
    return false;
  }
  ContentMap::iterator md5iter = fileiter->second.find(filepath);
  if (md5iter ==  fileiter->second.end()) {
    LOG(ERROR) << "INVALID FILEPATH";
    return false;
  }
  VLOG(6) << "file:" << filepath
          << " current md5: " << md5value;
  md5iter->second.md5value = md5value;
  size_t size(0);
  if (file::File::FileSize(filepath, &size)) {
    md5iter->second.filesize = size;
  }
  return true;
}

bool FileMonitor::UpdateHashValue(const std::string& filepath,
                                  const std::string& md5value,
                                  int wd) {
  base::MutexLock lock(mutex_for_fm_.get());
  return UpdateHashValueNonBlock(filepath, md5value, wd);
}
}
