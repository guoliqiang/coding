// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 19:29:07
// File  : code.cc
// Brief :

/*
 * 12 / 12 test cases passed.
 * Status: Accepted
 * Runtime: 256 ms
 * Submitted: 0 minutes ago
 *
 * */

#include "base/public/common_head.h"

namespace algorithm {

struct UndirectedGraphNode {
  int label;
  std::vector<UndirectedGraphNode *> neighbors;
  UndirectedGraphNode(int x) : label(x) {};
};

void Clone(UndirectedGraphNode * node, std::vector<UndirectedGraphNode *> & neighbors,
           std::map<int, UndirectedGraphNode *> & seen) {
  UndirectedGraphNode * ptr = new UndirectedGraphNode(node->label);
  neighbors.push_back(ptr);
  seen[ptr->label] = ptr;
  std::vector<UndirectedGraphNode *> & nei = node->neighbors;
  for (int i = 0; i < nei.size(); i++) {
    if (seen.count(nei[i]->label)) {
      ptr->neighbors.push_back(seen[nei[i]->label]);
    } else {
      Clone(nei[i], ptr->neighbors, seen);
    }
  }
}

UndirectedGraphNode *  Clone(UndirectedGraphNode * node) {
  if (node == NULL) return node;
  std::vector<UndirectedGraphNode *> foo;
  std::map<int, UndirectedGraphNode *> seen;
  Clone(node, foo, seen);
  return foo.front();
}

}  // namespace algorithm

using namespace algorithm;


int main(int argc, char** argv) {

  return 0;
}
