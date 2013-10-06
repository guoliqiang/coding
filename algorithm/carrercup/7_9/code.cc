// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-04 23:00:01
// File  : code.cc
// Brief : http://www.nobius.org/~dbg/practical-file-system-design.pdf
//

#include "base/public/common_head.h"

namespace algorithm {
const static int DATA_BLOCK_SIZE = 4 * 1024; // 4k
const static int NUM_DATA_BLOCKS = 512;

struct DataBlock {
  char data[DATA_BLOCK_SIZE];
};

struct INode {
  std::vector<int> datablocks;
};

struct MetaData {
  int size;
  int last_modifed;
  int created;
  char extra_attributes;
};


struct FSBase {};
struct File : public FSBase {
  INode nodes;
  MetaData metaData;
};

struct Directory : public FSBase {
  std::vector<FSBase* > content;
};


struct FileSystem {
  bool CreateFile(const char* name, int * fd) {
    return true;
  }
  bool OpenFile(const char * name, int * fd) {
    return true;
  }
  bool CloseFile(int * fd) {
    return true;
  }
  bool RemoveFile(const char * name) {
    return true;
  }
  bool CreateDirectory(const char* name, int * fd) {
    return true;
  }
  bool WriteToFile(int file, void * data, int num, bool append) {
    return true;
  }
  // 返回目前读取到的位置
  int ReadFromFile(int file, void* res, int numbutes,
                   int position) {
    return 0;
  }

  DataBlock dataBlocks[NUM_DATA_BLOCKS];
  std::vector<bool> dataBlockUsed;
  std::map<std::string, FSBase *> mapFromName;
  
  std::map<int, std::string> fdFromName;
  std::map<std::string, int> nameFromFd;
};

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}

