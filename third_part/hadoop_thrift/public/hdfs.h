// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yanliangcui@yunrang.com (Cui Yanliang)

#ifndef INFRASTRUCTURE_HADOOP_HDFS_HDFS_H_
#define INFRASTRUCTURE_HADOOP_HDFS_HDFS_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/thrift.h"
#include "infrastructure/hadoop/hdfs/gen-cpp/ThriftHadoopFileSystem.h"

namespace hadoopfs {

class Hdfs {
 public:
  // params introduction:
  // timeout:   ms, default=0 means every operation will always be blocking
  // until completing operation or occuring errors
  // sample:
  // Hdfs hdfs("192.168.0.44", 7372, "hdfs://hadoop03:9000/", 10000);

  Hdfs(const std::string &server_ip, uint32_t server_port,
       const std::string &hdfs_uri, int32_t timeout = 0);
  // sample:
  // Hdfs hdfs("hdfs://GS-DPO-SEV0001:9000/", 10000);
  Hdfs(const std::string &hdfs_uri, int32_t timeout = 0);
  void Reset();

  void SetTimeout(int32_t timeout);  // ms

  void SetInactivityTimeoutPeriod(int64_t periodInSeconds);
  void Shutdown(int32_t status);
  bool Create(const std::string& path, int64_t *fd);

  bool CreateFile(const std::string& path,
                  int16_t mode,
                  bool overwrite,
                  int32_t bufferSize,
                  int16_t block_replication,
                  int64_t blocksize,
                  int64_t *fd);

  bool Open(const std::string& path, int64_t *fd);
  bool Append(const std::string& path, int64_t *fd);
  bool Write(int64_t fd, const std::string& data);
  bool Read(int64_t fd,
            int64_t offset,
            int32_t size,
            std::string* data);
  bool Close(int64_t fd);
  bool Rm(const std::string& path, bool recursive);
  bool Rename(const std::string& path, const std::string& dest);
  bool Mkdir(const std::string& path);
  bool Exists(const std::string& path);

  // member data in class FileStatus
  /*
  class hadoopfs::FileStatus {
    public:
      std::std::string path;
      int64_t length;   //important
      bool isdir;
      int16_t block_replication;
      int64_t blocksize;
      int64_t modification_time;
      std::std::string permission;
      std::std::string owner;
      std::std::string group;
  };
  */
  void Stat(const std::string& path, FileStatus *st);

  void ListStatus(const std::string& path, std::vector<FileStatus> *stats);
  void Chmod(const std::string& path, int16_t mode);
  void Chown(const std::string& path,
             const std::string& owner,
             const std::string& group);
  void Chown(const std::string& path, const std::string& owner);
  void Chgrp(const std::string& path, const std::string& group);
  void SetReplication(const std::string& path, int16_t replication);

  // member data in class BlockLocation
  /*
  class hadoopfs::BlockLocation {
    public:
      std::vector<std::std::string>  hosts;
      std::vector<std::std::string>  names;
      int64_t offset;
      int64_t length;
  }
  */
  void GetFileBlockLocations(const std::string& path,
      int64_t start,
      int64_t length,
      std::vector<BlockLocation> *location);

 private:
  void ChangeAttrIfPossible(const std::string& path);
  void GetHadoopThriftServerAddr();
  bool ConnectHadoopThriftServer();
  static const int kMaxTryCount = 10;
  std::string server_ip_;
  std::string hdfs_host_;
  uint32_t server_port_;

  uint32_t timeout_;

  boost::shared_ptr<TSocket> socket_;
  boost::shared_ptr<TTransport> transport_;
  boost::shared_ptr<TProtocol> protocol_;
  boost::shared_ptr<ThriftHadoopFileSystemClient> client_;
};
}  // namespace hadoopfs
#endif  // INFRASTRUCTURE_HADOOP_HDFS_HDFS_H_
