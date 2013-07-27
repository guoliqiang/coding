// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Cui Yanliang)

#include "infrastructure/hadoop/hdfs/hdfs.h"
#include <algorithm>
#include "base/yr.h"
#include "base/logging.h"
#include "infrastructure/hadoop/hdfs/gen-cpp/ThriftHadoopFileSystem.h"
#include "util/yrns/yrns_manager.h"

DEFINE_string(hdfs_file_owner, "", "owner for new file or dir");
DEFINE_string(hdfs_file_group, "", "group for new file or dir");
DEFINE_int32(hdfs_file_mode_bits, 0, "mode bits for new file or dir");
DEFINE_string(hdfs_thrift_yrns_path, "",
        "chose the thrift-server from servers which under this yrns_path. "
        "Avaliable Values: "
        "hadoop/online_hadoop/hadoop_thrift_server,"
        "hadoop/experiment_hadoop/hadoop_thrift_server,"
        "hadoop/log_hadoop/hadoop_thrift_server.");
DEFINE_bool(auto_choose_hdfs_thrift, false,
        "true means the object will use yrns to decide "
        "which hadoop thrift server will be used."
        "User should set hdfs_thrift_yrns_path when set this value to true");
DEFINE_int32(hdfs_thrift_max_connect_times, 3, "max connect time when failed");

namespace hadoopfs {

Hdfs::Hdfs(
    const string &server_ip,
    uint32_t server_port,
    const string &hdfs_host,
    int32_t timeout)
    : server_ip_(server_ip),
      hdfs_host_(hdfs_host),
      server_port_(server_port),
      timeout_(timeout) {
  Reset();
}

Hdfs::Hdfs(const std::string &hdfs_uri,
          int32_t timeout)
          : hdfs_host_(hdfs_uri),
            timeout_(timeout) {
  // when use this construction,
  // the object will use zookeeper to get the thrift server info,
  // so this value will set to true automatically.
  FLAGS_auto_choose_hdfs_thrift = true;
  CHECK(!FLAGS_hdfs_thrift_yrns_path.empty())
    << " u must specified hdfs_thrift_yrns_path";
  Reset();
}

void Hdfs::GetHadoopThriftServerAddr() {
  util::YRNSManager yrns_manager;
  std::vector<util::YRNSReplicaInfo> results;
  yrns_manager.GetAllReplicas(FLAGS_hdfs_thrift_yrns_path,
                        util::YRNSManager::SERVICE_RPC,
                        &results);
  CHECK_GT(results.size(), 0) << "No HdfsThriftServer Avaliable, "
    << "check " << FLAGS_hdfs_thrift_yrns_path;
  unsigned seed_time = base::GetTimeInMs();
  unsigned int host_idx =
      static_cast<unsigned int> (rand_r(&seed_time) % results.size());
  server_ip_ = results[host_idx].hostname;
  server_port_ = results[host_idx].port;
  LOG(INFO) << "Using HadoopThriftServer: " << server_ip_
            << ":" << server_port_;
}

bool Hdfs::ConnectHadoopThriftServer() {
  socket_.reset(new TSocket(server_ip_, server_port_));
  transport_.reset(new TBufferedTransport(socket_));
  protocol_.reset(new TBinaryProtocol(transport_));
  client_.reset(new ThriftHadoopFileSystemClient(protocol_));
  SetTimeout(timeout_);
  try {
    transport_->open();
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC open, "
               << ex.what();
    return false;
  }
  return true;
}

void Hdfs::Reset() {
  if (FLAGS_auto_choose_hdfs_thrift) {
    CHECK(!FLAGS_hdfs_thrift_yrns_path.empty())
        << "when set auto_choose_hdfs_thrift, "
        << "hdfs_thrift_yrns_path must be specified";
    GetHadoopThriftServerAddr();
  }
  int retry_cnter = 0;
  while (retry_cnter++ < FLAGS_hdfs_thrift_max_connect_times) {
    if (!ConnectHadoopThriftServer()
        && FLAGS_auto_choose_hdfs_thrift) {
       GetHadoopThriftServerAddr();  // chose another thrift server
    } else {
      break;
    }
  }
}

void Hdfs::SetTimeout(int32_t timeout) {
  socket_->setRecvTimeout(timeout);
  socket_->setSendTimeout(timeout);
}

void Hdfs::SetInactivityTimeoutPeriod(int64_t periodInSeconds) {
  try {
    client_->setInactivityTimeoutPeriod(periodInSeconds);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC setInactivityTimeoutPeriod, "
               << ex.what();
  }
}

void Hdfs::Shutdown(int32_t status) {
  try {
    client_->shutdown(status);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC shutdown, " << ex.what();
  }
}

bool Hdfs::Create(const string& path, int64_t *fd) {
  bool succ = false;
  Pathname hdfs_path;
  ThriftHandle handle;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->create(handle, hdfs_path);
    *fd = handle.id;
    succ = true;
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC create, " << ex.what();
  }
  ChangeAttrIfPossible(path);
  return succ;
}

bool Hdfs::CreateFile(const string& path,
                      int16_t mode,
                      bool overwrite,
                      int32_t buffer_size,
                      int16_t block_replication,
                      int64_t blocksize,
                      int64_t *fd) {
  bool succ = false;
  Pathname hdfs_path;
  ThriftHandle handle;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->createFile(handle, hdfs_path, mode, overwrite, buffer_size,
                      block_replication, blocksize);
    *fd = handle.id;
    succ = true;
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC createFile, " << ex.what();
  }
  ChangeAttrIfPossible(path);
  return succ;
}

bool Hdfs::Open(const string& path, int64_t *fd) {
  bool succ = false;
  Pathname hdfs_path;
  ThriftHandle handle;
  if (path.find("hdfs:/") == 0) {
    hdfs_path.pathname = path;
  } else {
    hdfs_path.pathname = hdfs_host_ + path;
  }
      VLOG(4) << "call RPC open: " << hdfs_path.pathname;
  int try_i = 0;
  while (succ == false && try_i++ < kMaxTryCount) {
    try {
      client_->open(handle, hdfs_path);
      *fd = handle.id;
      succ = true;
    } catch(TException &ex) {
      LOG(ERROR) << "failed to call RPC open, " << ex.what();
      sleep(2);
      Reset();
    }
  }
  return succ;
}

bool Hdfs::Append(const string& path, int64_t *fd) {
  bool succ = false;
  Pathname hdfs_path;
  ThriftHandle handle;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->append(handle, hdfs_path);
    *fd = handle.id;
    succ = true;
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC append, " << ex.what();
  }
  ChangeAttrIfPossible(path);
  return succ;
}

bool Hdfs::Write(int64_t fd, const std::string& data) {
  bool succ = false;
  ThriftHandle handle;
  int try_i = 0;
  while (succ == false && try_i++ < kMaxTryCount) {
    try {
      handle.id = fd;
      std::vector<signed char> value;
      value.reserve(data.length()+1);
      std::copy(data.begin(), data.end(), std::back_inserter(value));
      succ = client_->write(handle, value);
    } catch(TException &ex) {
      LOG(ERROR) << "failed to call RPC write,  " << ex.what()
        << ". Try count: " << try_i;
      succ = false;
      sleep(2);
      Reset();
    }
  }
  return succ;
}

bool Hdfs::Read(
    int64_t fd, int64_t offset, int32_t size, std::string *data) {
  VLOG(5) << "read size: " << size;
  bool succ = false;
  ThriftHandle handle;
  std::vector<signed char> value;
  int try_i = 0;
  while (succ == false && try_i++ < kMaxTryCount) {
    try {
      handle.id = fd;
      client_->read(value, handle, offset, size);
      data->assign(value.begin(), value.end());
      succ = true;
    } catch(TException &ex) {
      LOG(ERROR) << "failed to call RPC read, " << ex.what()
        << ". Try count: " << try_i;
      succ = false;
      sleep(2);
      Reset();
    }
  }
  return succ;
}

bool Hdfs::Close(int64_t fd) {
  bool succ = false;
  ThriftHandle handle;
  try {
    handle.id = fd;
    succ = client_->close(handle);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC close, " << ex.what();
    succ = false;
  }
  return succ;
}

bool Hdfs::Rm(const string& path, bool recursive) {
  bool succ = false;
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    succ = client_->rm(hdfs_path, recursive);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC rm, " << ex.what();
    succ = false;
  }
  return succ;
}

bool Hdfs::Rename(const string& path, const string& dest) {
  bool succ = false;
  Pathname hdfs_path;
  Pathname hdfs_dest;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    if (dest.find("hdfs:/") == 0) {
      hdfs_dest.pathname = dest;
    } else {
      hdfs_dest.pathname = hdfs_host_ + dest;
    }
    succ = client_->rename(hdfs_path, hdfs_dest);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC rename, " << ex.what();
    succ = false;
  }
  ChangeAttrIfPossible(path);
  return succ;
}

bool Hdfs::Mkdir(const string& path) {
  bool succ = false;
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    succ = client_->mkdirs(hdfs_path);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC mkdirs, " << ex.what();
    succ = false;
  }
  ChangeAttrIfPossible(path);
  return succ;
}

bool Hdfs::Exists(const string& path) {
  bool succ = false;
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    VLOG(4) << "call RPC exists: " << hdfs_path.pathname;
    succ = client_->exists(hdfs_path);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC exists, " << ex.what();
    succ = false;
  }
  return succ;
}

void Hdfs::Stat(const string &path, FileStatus *st) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->stat(*st, hdfs_path);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC stat, " << ex.what();
  }
}

void Hdfs::ListStatus(const string& path, vector<FileStatus> *stats) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->listStatus(*stats, hdfs_path);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC listStatus, " << ex.what()
               << " on path: " << path;
  }
}

void Hdfs::Chmod(const string& path, int16_t mode) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->chmod(hdfs_path, mode);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC chmod, " << ex.what();
  }
}

void Hdfs::Chown(const string& path,
                 const string& owner,
                 const string& group) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->chown(hdfs_path, owner, group);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC chown, " << ex.what();
  }
}

void Hdfs::Chown(const std::string& path, const std::string& owner) {
  FileStatus st;
  Stat(path, &st);
  Chown(path, owner, st.group);
}

void Hdfs::Chgrp(const std::string& path, const std::string& group) {
  FileStatus st;
  Stat(path, &st);
  Chown(path, st.owner, group);
}

void Hdfs::SetReplication(const string& path, int16_t replication) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->setReplication(hdfs_path, replication);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC setReplication, " << ex.what();
  }
}

void Hdfs::GetFileBlockLocations(const string& path,
                                 int64_t start,
                                 int64_t length,
                                 vector<BlockLocation> *location) {
  Pathname hdfs_path;
  try {
    if (path.find("hdfs:/") == 0) {
      hdfs_path.pathname = path;
    } else {
      hdfs_path.pathname = hdfs_host_ + path;
    }
    client_->getFileBlockLocations(*location, hdfs_path, start, length);
  } catch(TException &ex) {
    LOG(ERROR) << "failed to call RPC getFileBlockLocations, " << ex.what();
  }
}

void Hdfs::ChangeAttrIfPossible(const std::string& path) {
  if (FLAGS_hdfs_file_mode_bits != 0) {
    Chmod(path, FLAGS_hdfs_file_mode_bits);
  }
  if (!FLAGS_hdfs_file_owner.empty() && !FLAGS_hdfs_file_group.empty()) {
    Chown(path, FLAGS_hdfs_file_owner, FLAGS_hdfs_file_group);
  } else if (!FLAGS_hdfs_file_owner.empty()) {
    Chown(path, FLAGS_hdfs_file_owner);
  } else if (!FLAGS_hdfs_file_group.empty()) {
    Chgrp(path, FLAGS_hdfs_file_group);
  }
}

}  // namespace hadoopfs

