// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Peng YongYi)

#include "../internal/trie_tree-inl_union.h"

#include "../../../base/public/logging.h"
using namespace trie_tree;

int main(int argc, char** argv) {
  TrieTreeUnion<string, string> tt;
  std::vector<string> input;
  string value = "ok";
  input.push_back("中国");
  input.push_back("人民");

  tt.Put(input, value);
  LOG(INFO) << tt.Find(input);
  return 0;
}

