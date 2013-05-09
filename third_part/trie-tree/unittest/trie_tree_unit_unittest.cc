#include "../public/trie_tree-inl_unit.h"

#include "../../../base/public/logging.h"
using namespace trie_tree;

int main(int argc, char** argv) {
  TrieTreeUnit<string, string> tt;
  std::vector<string> input;
  string value = "ok";
  input.push_back("中国");
  input.push_back("人民");

  tt.Put(input, value);
  LOG(INFO) << tt.Find(input);
  return 0;
}

