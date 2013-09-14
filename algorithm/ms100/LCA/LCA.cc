// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-14 08:25:49
// File  : LCA.cc
// Brief :
#include "LCA.h"
using namespace algorithm;

int main(int argc, char** argv) {
  std::vector<int> foo;
  for (int i = 0; i < 10; i++) {
    foo.push_back(i);
  }
  Node<int> * root = BuildBST(foo);
  Node<int> * n1 = RandomNode(root);
  Node<int> * n2 = RandomNode(root);
  LOG(INFO) << n1->value << " " << n2->value;
  CHECK(LCA(root, n1, n2) != NULL);;
  CHECK(LCA2(root, n1, n2) != NULL);;
  LOG(INFO) << "Tarjan:" << Tarjan(root, n1, n2)->value;
  LOG(INFO) << "LCA:" << LCA(root, n1, n2)->value;
  LOG(INFO) << "LCA2:" << LCA2(root, n1, n2)->value;
  n1 = Find(root, 4);
  n2 = Find(root, 0);
  CHECK(n1 != NULL);
  CHECK(n2 != NULL);
  CHECK(LCA(root, n1, n2) != NULL);;
  CHECK(LCA2(root, n1, n2) != NULL);;
  LOG(INFO) << n1->value << " " << n2->value;
  LOG(INFO) << "Tarjan:" << Tarjan(root, n1, n2)->value;
  LOG(INFO) << "LCA:" << LCA(root, n1, n2)->value;
  LOG(INFO) << "LCA2:" << LCA2(root, n1, n2)->value;


  n1 = Find(root, 0);
  n2 = Find(root, 3);
  CHECK(n1 != NULL);
  CHECK(n2 != NULL);
  CHECK(LCA(root, n1, n2) != NULL);;
  CHECK(LCA2(root, n1, n2) != NULL);;
  LOG(INFO) << n1->value << " " << n2->value;
  LOG(INFO) << "Tarjan:" << Tarjan(root, n1, n2)->value;
  LOG(INFO) << "LCA:" << LCA(root, n1, n2)->value;
  LOG(INFO) << "LCA2:" << LCA2(root, n1, n2)->value;
  
  n1 = Find(root, 7);
  n2 = Find(root, 4);
  CHECK(n1 != NULL);
  CHECK(n2 != NULL);
  CHECK(LCA(root, n1, n2) != NULL);;
  CHECK(LCA2(root, n1, n2) != NULL);;
  LOG(INFO) << n1->value << " " << n2->value;
  LOG(INFO) << "Tarjan:" << Tarjan(root, n1, n2)->value;
  LOG(INFO) << "LCA:" << LCA(root, n1, n2)->value;
  LOG(INFO) << "LCA2:" << LCA2(root, n1, n2)->value;
  return 0;
}
