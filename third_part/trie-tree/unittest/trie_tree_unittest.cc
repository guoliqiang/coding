// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Peng YongYi)

#include "../internal/trie_tree-inl.h"

#include "../../../base/public/logging.h"
#include "../../testing/gtest/include/gtest/gtest.h"

namespace trie_tree {

class TrieTreeUnittest: public testing::Test {
 public:
  TrieTree<string>* vf_;
 protected:
  virtual void SetUp() {
    vf_ = new TrieTree<string>();
  }
  virtual void TearDown() {
    delete vf_;
  }
};

TEST_F(TrieTreeUnittest, TestPutGet) {
  string key1("www.test");
  string key2("test");
  string val1("www.test001.com");
  string val2("www.test002.com");

  vector<string> result;
  set<string> r_set;
  vf_->Get(key1, &result);
  EXPECT_EQ(0, result.size());
  EXPECT_EQ(false, vf_->Find(key1));

  vf_->Put(key1, val1);
  vf_->Get(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(1, result.size());
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_FALSE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_EQ(8, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  vf_->Put(key1, val2);
  vf_->Get(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(8, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  vf_->Put(key2, val2);
  vf_->Get(key2, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(1, result.size());
  EXPECT_EQ(true, vf_->Find(key2));
  EXPECT_FALSE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(4, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  string key("www.test00");
  vf_->Get(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
}

TEST_F(TrieTreeUnittest, TestPutGet2) {
  string key1("www.");
  string key2("www.test");
  string val1("www.test001.com");
  string val2("www.test002.com");

  vf_->Put(key1, val1);
  vf_->Put(key1, val2);
  vf_->Put(key2, val1);
  vf_->Put(key2, val2);

  string key("www.te");
  vector<string> result;
  set<string> r_set;
  vf_->Get(key, &result);
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key));

  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(4, vf_->GetMinTrie());
}

TEST_F(TrieTreeUnittest, TestGetEmpty) {
  string key("");
  vector<string> r;

  vf_->Get(key, 10, &r);
  string key1("www.");
  string key2("www.test");
  string val1("www.test001.com");
  string val2("www.test002.com");

  vf_->Put(key1, val1);
  vf_->Put(key1, val2);
  vf_->Put(key2, val1);
  vf_->Put(key2, val2);

  vf_->Get(key, 10, &r);
  EXPECT_EQ(true, vf_->Find(key2));
}

TEST_F(TrieTreeUnittest, TestRemove) {
  string key1("www.test");
  string key2("test");
  string val1("www.test001.com");
  string val2("www.test002.com");

  vf_->Put(key1, val1);
  vf_->Put(key1, val2);
  vf_->Put(key2, val1);
  vf_->Put(key2, val2);

  vector<string> result;
  set<string> r_set;
  vf_->Get(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(4, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  string key("www.test00");
  vf_->Remove(key1, val1);
  vf_->Get(key, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(1, result.size());
  EXPECT_EQ(true, vf_->Find(key));
  EXPECT_FALSE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));

  vf_->Remove(key2, val1);
  vf_->Remove(key2, val2);
  EXPECT_EQ(8, vf_->GetMinTrie());
}

TEST_F(TrieTreeUnittest, TestFullMatchGet) {
  string key1("www.test");
  string key2("test");
  string val1("www.test001.com");
  string val2("www.test002.com");

  vector<string> result;
  set<string> r_set;
  vf_->FullMatchGet(key1, &result);
  EXPECT_EQ(0, result.size());
  EXPECT_EQ(false, vf_->Find(key1));

  vf_->Put(key1, val1);
  vf_->FullMatchGet(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(1, result.size());
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_FALSE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_EQ(false, vf_->Find("www"));
  EXPECT_EQ(8, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  vf_->Put(key1, val2);
  vf_->FullMatchGet(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_EQ(false, vf_->Find("www"));
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(8, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  vf_->Put(key2, val2);
  vf_->FullMatchGet(key2, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(1, result.size());
  EXPECT_EQ(true, vf_->Find(key2));
  EXPECT_EQ(false, vf_->Find("tes"));
  EXPECT_FALSE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
  EXPECT_EQ(4, vf_->GetMinTrie());

  result.clear();
  r_set.clear();
  string key("www.test00");
  vf_->FullMatchGet(key1, &result);
  for(vector<string>::iterator it = result.begin(); it != result.end(); ++it) {
    r_set.insert(*it);
  }
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(true, vf_->Find(key1));
  EXPECT_TRUE(r_set.end() != r_set.find(val1));
  EXPECT_TRUE(r_set.end() != r_set.find(val2));
}

TEST_F(TrieTreeUnittest, TestSubTreeGet) {
  string key0("test");
  string key1("test-1");
  string key2("test-2");
  string key3("test-2-1");
  string key4("test-2-2");
  string key5("test-2-2-1");
  string val1("1.yunrang.com");
  string val2("2.yunrang.com");
  string val3("3.yunrang.com");
  string val4("4.yunrang.com");
  string val5("5.yunrang.com");
  string val6("6.yunrang.com");
  vf_->Put(key1, val1);
  vf_->Put(key2, val2);
  vf_->Put(key3, val3);
  vf_->Put(key4, val4);
  vf_->Put(key4, val6);
  vf_->Put(key5, val5);

  vector<string> result;

  result.clear();
  vf_->SubTreeGet(key0, &result);
  EXPECT_EQ(6, result.size());

  result.clear();
  vf_->SubTreeGet(key1, &result);
  EXPECT_EQ(1, result.size());

  result.clear();
  vf_->SubTreeGet(key2, &result);
  EXPECT_EQ(5, result.size());

  result.clear();
  vf_->SubTreeGet(key3, &result);
  EXPECT_EQ(1, result.size());

  result.clear();
  vf_->SubTreeGet(key4, &result);
  EXPECT_EQ(3, result.size());

  result.clear();
  vf_->SubTreeGet(key5, &result);
  EXPECT_EQ(1, result.size());
}
}
