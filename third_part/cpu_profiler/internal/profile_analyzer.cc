// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-03-26 02:04:54
// File  : profile_analyzer.cc
// Brief :

#include "third_part/cpu_profiler/public/profile_analyzer.h"
#include <fcntl.h>

DEFINE_string(cpu_profiler_path, "cpu_profiler.prof", "");
DEFINE_string(cpu_profiler_symbol_path, "cpu_profiler.symbol", "");

void ProfileAnalyzer::Build() {
  int fd = open(FLAGS_cpu_profiler_symbol_path.c_str(), O_RDONLY);
  CHECK(fd >= 0) << "open " << FLAGS_cpu_profiler_symbol_path << " error";
  uint64_t address = 0;
  int len = 0;
  std::string buffer;
  int read_size = 0;
  while ((read_size = read(fd, &address, sizeof(address)))) {
    CHECK(read_size == sizeof(address));

    read_size = read(fd, &len, sizeof(len));
    CHECK(read_size = sizeof(len));
    if (len > buffer.size()) buffer.resize(len);

    read_size = read(fd, const_cast<char *>(buffer.data()), len);
    CHECK(read_size == len);

    symbol_.insert(std::make_pair(address, buffer.substr(0, len)));
    hit_num_[buffer.substr(0, len)] = 0;
  }
  close(fd);

  fd = open(FLAGS_cpu_profiler_path.c_str(), O_RDONLY);
  CHECK(fd >= 0) << "open " << FLAGS_cpu_profiler_path << " error";
  
  std::vector<uint64_t> stack;
  uint64_t head[2] = { 0 };
  bool first = true;
  int cnt = 0;

  while ((read_size = read(fd, head, sizeof(head)))) {
    CHECK_EQ(read_size, sizeof(head));
    if (head[1] > stack.size()) stack.resize(head[1]);
    read_size = read(fd, &stack[0], head[1] * sizeof(uint64_t));
    CHECK_EQ(read_size, head[1] * sizeof(uint64_t));

    if (first) {
      CHECK_EQ(head[1], 3);
      first = false;
    } else if (head[0] == 0 && head[1] == 1 && stack[0] == 0) {
      break;
    } else {
      cnt++;
      for (int i = 0; i < head[1]; i++) {
        if (i + 1 != head[1]) {
          if (!grap_[symbol_[stack[i]]].count(symbol_[stack[i + 1]])) {
            grap_[symbol_[stack[i]]][symbol_[stack[i + 1]]] = 0;
          }
          grap_[symbol_[stack[i]]][symbol_[stack[i + 1]]] += head[0];
          VLOG(5) << "add "
                  << "grap_[" << symbol_[stack[i]] << "]["
                  << symbol_[stack[i + 1]] << "]:"
                  << head[0];
        }

        if (i - 1 >= 0) {
          if (!reverse_grap_[symbol_[stack[i]]].count(symbol_[stack[i - 1]])) {
            reverse_grap_[symbol_[stack[i]]][symbol_[stack[i - 1]]] = 0;
          }
          reverse_grap_[symbol_[stack[i]]][symbol_[stack[i - 1]]] += head[0];
        }

        if (i == 0) {
          hit_num_[symbol_[stack[head[0]]]]++;
          if (!reverse_grap_[symbol_[stack[i]]].count(symbol_[stack[i]])) {
            reverse_grap_[symbol_[stack[i]]][symbol_[stack[i]]] = 0;
          }
          reverse_grap_[symbol_[stack[i]]][symbol_[stack[i]]] += head[0];
          if (!grap_[symbol_[stack[i]]].count(symbol_[stack[i]])) {
            grap_[symbol_[stack[i]]][symbol_[stack[i]]] = 0;
          }
          grap_[symbol_[stack[i]]][symbol_[stack[i]]] += head[0];
          VLOG(5) << "add "
                  << "grap_[" << symbol_[stack[i]] << "]["
                  << symbol_[stack[i]] << "]:"
                  << head[0] << " in " << i << " of " << head[1];
        }
      }  // for
    }
  }
  LOG(INFO) << "catch " << cnt << " stack call";
}


void ProfileAnalyzer::SimpleList(std::map<std::string, int> * rs) {
  rs->clear();
  for (base::hash_map<std::string, int>::iterator i = hit_num_.begin();
       i != hit_num_.end(); i++) {
    const std::string & key = i->first;
    rs->insert(std::make_pair(key, i->second));
  }
}
