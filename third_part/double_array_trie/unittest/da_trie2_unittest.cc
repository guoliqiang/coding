// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include <string>
#include <vector>

#include "base/flags.h"
#include "base/hash_tables.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/double_array_trie/da_trie2.h"

DEFINE_bool(run_bench, false, "");

namespace util {
using std::string;
using std::vector;

class DATrieBasicTest: public testing::Test {
 public:
 protected:
  virtual void SetUp() {
    base::hash_map<string, int> map;

    map["12345"] = 1;
    map["521"] = 2;
    map["213"] = 3;
    map["12"] = 4;
    map["34123j"] = 5;
    map["34532"] = 6;
    map["321"] = 7;
    map["123"] = 8;
    map["45"] = 9;
    map["234533"] = 10;
    map["google"] = 11;
    map["天气"] = 12;
    map["气象"] = 13;
    map["yunrang"] = 14;
    map["yunrangx"] = 15;

    trie_.BuildFrom(map);
  }
  virtual void TearDown() {
  }
  DATrieBasic2<string> trie_;
};

TEST_F(DATrieBasicTest, PrefixMatch) {
  DATrieBasic2<string>::ResultPair pairs[10];
  int n = trie_.prefixMatch("123456", 6, 0, pairs, 10);
  EXPECT_EQ(3, n);
  EXPECT_EQ(2, pairs[0].length);
  EXPECT_EQ(4, pairs[0].value);
  EXPECT_EQ(3, pairs[1].length);
  EXPECT_EQ(8, pairs[1].value);
  EXPECT_EQ(5, pairs[2].length);
  EXPECT_EQ(1, pairs[2].value);
}

TEST_F(DATrieBasicTest, ExactMatch) {
  int i = 0;
  EXPECT_TRUE(trie_.ExactMatch("12345", &i));
  EXPECT_EQ(i, 1);

  EXPECT_TRUE(trie_.ExactMatch("521", &i));
  EXPECT_EQ(i, 2);

  EXPECT_TRUE(trie_.ExactMatch("213", &i));
  EXPECT_EQ(i, 3);

  EXPECT_TRUE(trie_.ExactMatch("12", &i));
  EXPECT_EQ(i, 4);

  EXPECT_TRUE(trie_.ExactMatch("34123j", &i));
  EXPECT_EQ(i, 5);

  EXPECT_TRUE(trie_.ExactMatch("34532", &i));
  EXPECT_EQ(i, 6);

  EXPECT_TRUE(trie_.ExactMatch("321", &i));
  EXPECT_EQ(i, 7);

  EXPECT_TRUE(trie_.ExactMatch("123", &i));
  EXPECT_EQ(i, 8);

  EXPECT_TRUE(trie_.ExactMatch("45", &i));
  EXPECT_EQ(i, 9);

  EXPECT_TRUE(trie_.ExactMatch("234533", &i));
  EXPECT_EQ(i, 10);

  EXPECT_TRUE(trie_.ExactMatch("google", &i));
  EXPECT_EQ(i, 11);

  EXPECT_TRUE(trie_.ExactMatch("天气", &i));
  EXPECT_EQ(i, 12);

  EXPECT_TRUE(trie_.ExactMatch("气象", &i));
  EXPECT_EQ(i, 13);

  EXPECT_FALSE(trie_.ExactMatch("气象1", &i));
  EXPECT_FALSE(trie_.ExactMatch("googl", &i));
}

TEST_F(DATrieBasicTest, MatchAny) {
  EXPECT_TRUE(trie_.MatchAny("12"));
  EXPECT_TRUE(trie_.MatchAny("123"));
  EXPECT_TRUE(trie_.MatchAny("12345"));
  EXPECT_TRUE(trie_.MatchAny("213"));
  EXPECT_TRUE(!trie_.MatchAny("21"));
  EXPECT_TRUE(trie_.MatchAny("234533"));
  EXPECT_TRUE(trie_.MatchAny("34123j"));
  EXPECT_TRUE(trie_.MatchAny("34532"));
  EXPECT_TRUE(trie_.MatchAny("321"));
  EXPECT_TRUE(trie_.MatchAny("45"));
  EXPECT_TRUE(trie_.MatchAny("521"));
  EXPECT_TRUE(trie_.MatchAny("google"));
  EXPECT_TRUE(trie_.MatchAny("天气"));
  EXPECT_TRUE(!trie_.MatchAny("天"));
  EXPECT_TRUE(trie_.MatchAny("气象"));
}

TEST_F(DATrieBasicTest, LikelyMatch) {
  vector<string> vec;

  trie_.LikelyMatch("12345", &vec);  // not like self
  ASSERT_EQ(0, vec.size());

  trie_.LikelyMatch("uynrang", &vec);  // switch two silibing char
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunragn", &vec);  // switch two silibing char
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("ayunrang", &vec);  // add on char at the begin
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunbrang", &vec);  // add on char at the middle
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunran", &vec);  // omit at the end
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunrng", &vec);  // omit one char
  ASSERT_EQ(1, vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("unrang", &vec);  // omit at the begin
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunrbng", &vec);  // replace one char
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("aunrang", &vec);  // replace one char at the begin
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunrangb", &vec);  // add on char at the end
  ASSERT_EQ(vec.size(), 2);
  EXPECT_EQ("yunrang", vec[0]);
  EXPECT_EQ("yunrangx", vec[1]);
}

DATrieBasic2<string> *g_trie;
void ExactMatchBench(int n) {
  std::string s = "天气";
  int res = 0;
  int sum = 0;
  DATrieBasic2<string>::ResultPair result_pair;
  for (int i = 0;i < n;++i) {
//    g_trie->ExactMatch(s, &res);
//    sum += res;
    g_trie->exactMatch(s.c_str(), s.length(), 0, &result_pair);
    sum += result_pair.value;
  }
  sum += res;
  VLOG(0) << sum;
}

BENCHMARK(ExactMatchBench)->ThreadRange(1, testing::NumCPUs());

void LikelyMatchBench(int n) {
  std::string s = "yunran";
  int sum = 0;
  vector<string> vec;
  for (int i = 0;i < n;++i) {
//    g_trie->ExactMatch(s, &res);
//    sum += res;
    g_trie->LikelyMatch(s, &vec);
    sum += vec.size();
  }
  VLOG(0) << sum;
}
BENCHMARK(LikelyMatchBench)->ThreadRange(1, testing::NumCPUs());

TEST_F(DATrieBasicTest, Bench) {
  g_trie = &trie_;
  if (FLAGS_run_bench) {
    testing::RUN_ALL_BENCHMARKS();
  }
}
}  // namespace util
