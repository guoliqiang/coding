// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-01-12 22:03:41
// File  : code.cc
// Brief :

#include "base/public/common_ojhead.h"

namespace algorithm {

struct Node {
  unsigned int hv;
  std::string host;
  Node(unsigned int hvi, const std::string & hosti) : hv(hvi), host(hosti) {}
  bool operator < (const Node & x) const {
    return hv < x.hv;
  }
};

class CHash {
 public:
  unsigned int Hash(const char * str) {
    unsigned int rs = 0;
    while (*str != '\0') {
      rs = rs * 31 + *str;
      str++;
    }
    return rs;
  }

  void Set(std::vector<std::string> hosts) {
    for (int i = 0; i < hosts.size(); i++) {
      unsigned int hv = Hash(hosts[i].c_str());
      hv_to_host.push_back(Node(hv, hosts[i]));
    }
    std::sort(hv_to_host.begin(), hv_to_host.end());
  }

  // 二分查找大于等于key的hash值的server的ip
  std::string Get(const std::string & key) {
    unsigned int hv = Hash(key.c_str());
    int b = 0;
    int e = hv_to_host.size() - 1;
    while (b <= e) {
      int mid = b + (e - b) / 2;
      if (hv_to_host[mid].hv >= hv && (mid == 0 || hv_to_host[mid - 1].hv < hv)) return hv_to_host[mid].host;
      else if (hv_to_host[b].hv >= hv) e = mid - 1;
      else b = mid + 1;
    }
    return hv_to_host[0].host;
  }

 private:
  std::vector<Node> hv_to_host;  // hash value and host
};
}  // namespace algorithm

using namespace algorithm;

int main(int argc, char** argv) {
  return 0;
}
