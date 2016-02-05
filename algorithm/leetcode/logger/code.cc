// Copyright 2016 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2016-02-02 17:48:17
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct LoggerNode {
  int id;
  int start_time;
  int end_time;
};

class Cmp {
 public:
  bool operator() (const LoggerNode * x, const LoggerNode * y) {
    return x->start_time > y->start_time;
  }
};

class Logger {
 public:
  void StartRequest(int id) {
    dict[id] = new LoggerNode();
    dict[id]->start_time = time(NULL);
    dict[id]->end_time = 0;
    dict[id]->id = id;
  }
  void EndRequest(int id) {
    dict[id]->end_time = time(NULL);
    while (queue.size() && queue.top()->end_time != 0) {
      LoggerNode * cur = queue.top();
      queue.pop();
      LOG(INFO) << cur->id << " " << cur->start_time << " " << cur->end_time;
      delete dict[cur->id];
      dict.erase(cur->id);
    }
  }
 private:
  std::priority_queue<LoggerNode*, std::vector<LoggerNode *>, Cmp> queue;
  std::map<int, LoggerNode *> dict;
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
