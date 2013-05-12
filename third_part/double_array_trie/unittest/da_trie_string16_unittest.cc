// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include <string>
#include <vector>

#include "../../../base/public/flags.h"
#include "../../../base/public/hash_tables.h"
#include "../../../base/public/utf_string_conversions.h"
#include "../../testing/gtest/include/gtest/gtest.h"
#include "../public/da_trie_string16.h"


using std::string;
using std::vector;

class DATrie16BasicTest: public testing::Test {
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
  DATrie16Basic trie_;
};

TEST_F(DATrie16BasicTest, ExactMatch) {
  int i = 0;
  EXPECT_TRUE(trie_.ExactMatchSearch("12345", &i));
  EXPECT_EQ(i, 1);

  EXPECT_TRUE(trie_.ExactMatchSearch("521", &i));
  EXPECT_EQ(i, 2);

  EXPECT_TRUE(trie_.ExactMatchSearch("213", &i));
  EXPECT_EQ(i, 3);

  EXPECT_TRUE(trie_.ExactMatchSearch("12", &i));
  EXPECT_EQ(i, 4);

  EXPECT_TRUE(trie_.ExactMatchSearch("34123j", &i));
  EXPECT_EQ(i, 5);

  EXPECT_TRUE(trie_.ExactMatchSearch("34532", &i));
  EXPECT_EQ(i, 6);

  EXPECT_TRUE(trie_.ExactMatchSearch("321", &i));
  EXPECT_EQ(i, 7);

  EXPECT_TRUE(trie_.ExactMatchSearch("123", &i));
  EXPECT_EQ(i, 8);

  EXPECT_TRUE(trie_.ExactMatchSearch("45", &i));
  EXPECT_EQ(i, 9);

  EXPECT_TRUE(trie_.ExactMatchSearch("234533", &i));
  EXPECT_EQ(i, 10);

  EXPECT_TRUE(trie_.ExactMatchSearch("google", &i));
  EXPECT_EQ(i, 11);

  EXPECT_TRUE(trie_.ExactMatchSearch("天气", &i));
  EXPECT_EQ(i, 12);

  EXPECT_TRUE(trie_.ExactMatchSearch("气象", &i));
  EXPECT_EQ(i, 13);
}

TEST_F(DATrie16BasicTest, MatchAny) {
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

TEST_F(DATrie16BasicTest, LikelyMatch) {
  vector<string> vec;
  trie_.LikelyMatch("yunrbng", &vec);  // replace one char
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("aunrang", &vec);  // replace one char at the begin
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunrng", &vec);  // omit one char
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunran", &vec);  // omit at the end
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("unrang", &vec);  // omit at the begin
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("ayunrang", &vec);  // add on char at the begin
  ASSERT_TRUE(vec.size());
  EXPECT_EQ("yunrang", vec[0]);

  trie_.LikelyMatch("yunrangb", &vec);  // add on char at the end
  ASSERT_EQ(vec.size(), 2);
  EXPECT_EQ("yunrang", vec[0]);
  EXPECT_EQ("yunrangx", vec[1]);

  trie_.LikelyMatch("yunragn", &vec);  // switch two silibing char
  ASSERT_EQ(vec.size(), 1);
  EXPECT_EQ("yunrang", vec[0]);
}

