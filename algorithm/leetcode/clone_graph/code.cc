// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-10-02 19:29:07
// File  : code.cc
// Brief :

/*
Clone an undirected graph. Each node in the graph contains a label and a list of its neighbors.


OJ's undirected graph serialization:
Nodes are labeled uniquely.

We use # as a separator for each node, and , as a separator for node label and each neighbor of the node.
As an example, consider the serialized graph {0,1,2#1,2#2,2}.

The graph has a total of three nodes, and therefore contains three parts as separated by #.

First node is labeled as 0. Connect node 0 to both nodes 1 and 2.
Second node is labeled as 1. Connect node 1 to node 2.
Third node is labeled as 2. Connect node 2 to node 2 (itself), thus forming a self-cycle.
Visually, the graph looks like the following:

       1
      / \
     /   \
    0 --- 2
         / \
         \_/

*/

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
