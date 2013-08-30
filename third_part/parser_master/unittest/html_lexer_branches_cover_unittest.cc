// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/logging.h"
#include "file/file.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "util/gtl/stl_util-inl.h"
#include "util/parse_master2/internal/html_lexer.h"
#include "util/parse_master2/public/parse_master.h"

namespace parse_master {
class HtmlLexerUnittest: public testing::Test {
 public:
 protected:
  HtmlLexer lexer_;
  virtual void SetUp() {
    string tmp_html("this is a invalid buffer.");
    for (int i = 0; i < 1000; ++i) {
      BuffIndex bi(i % tmp_html.size());
      bi.SetEndPos(tmp_html.size());
      HtmlToken* ht = lexer_.GetToken(i % tmp_html.size());
      ht->SetEndPos(tmp_html.size());
    }
    VLOG(10) << "unittest setup";
  }
  virtual void TearDown() {
  }
};

TEST_F(HtmlLexerUnittest, BranchCover_1) {
  // new branches: 001
  string html;
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(0, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_2) {
  // new branches: 027 020 005
  string html("<");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_3) {
  // new branches: 026
  string html(" <");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_4) {
  // new branches: 036 006
  string html(" < ");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_5) {
  // new branches: 101
  string html(" < /");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_6) {
  // new branches: 039 040 002 003 007
  string html("<a");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_7) {
  // new branches: 023 043 013
  string html("<a ");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_8) {
  // new branches: 018 064
  string html("<a b");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_9) {
  // new branches: 067
  string html("<a b=");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_10) {
  // new branches: 083
  string html("<a b=\"");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_11) {
  // new branches: 087
  string html("<a b=\"\"");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_12) {
  // new branches: 035 101 099
  string html("<汉a 汉b=\"汉\"");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_13) {
  // new branches: 025 015 100 086 014 062
  string html("汉<a b=\"汉\">");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_14) {
  // new branches: 022 063
  string html("汉<a b=\"汉\"<");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_15) {
  // new branches: 028 042 098
  string html("z<a汉 b=\"汉\"><");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_16) {
  // new branches: 060 061
  string html("<a汉 b=\"汉\"/>");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_17) {
  // new branches: 077 081 097
  string html("<a汉 b=\"汉\"/ c d=>e");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_18) {
  // new branches: 066 024 085 093
  string html("<a汉 b=\"汉\"/ c d=e>f<");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_19) {
  // new branches: 012 038
  string html("<a汉 b=\"汉\"/ c d=e>f</");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_20) {
  // new branches: 046 045 096 109 107 108 115 112 011
  string html("<a汉 b=\"汉\"/ c d=e>f<script>js 1 < 2<//not comment </");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(4, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_21) {
  // new branches: 058 010
  string html("<a汉 b=\"汉\"/ c d=e>f</a");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_22) {
  // new branches: 051
  string html("<a汉 b=\"汉\"/ c d=e>f</ a");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_23) {
  // new branches: 048 056 055
  string html("<a汉 b=\"汉\"/ c d=e>f</a b c=''>");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_24) {
  // new branches: 017 041
  string html("<a汉 b=\"汉\"/ c d=e>f<></a b c=''>");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_25) {
  // new branches: 050
  string html("<a汉 b=\"汉\"/ c d=e>f<></a>");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_26) {
  // new branches: 016
  string html("<a汉 b=\"汉\"/ c d=e>f<</a><a 汉><b c汉>"
      "<d e<f g  h 汉 ><i 汉 <j k=汉汉 l=<m n=oo/><p q=rr<");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(12, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_27) {
  // new branches: 019 113
  string html("<a汉 b=\"汉\"/ c d=e>f<script>js 1 < 2<//not comment </scri"
      "</script</script a汉<script></script ><script></script");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(9, tokens.size());
}

TEST_F(HtmlLexerUnittest, BranchCover_28) {
  // new branches: 037 031 033 034 032 030 103 044 047 052 054 053
  string html("<!a><!汉><! a汉><!-- --><b<c<d><e/>"
      "</f汉></g</h 汉</i \n</j <");
  vector<HtmlToken*> tokens;
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(14, tokens.size());
}

TEST_F(HtmlLexerUnittest, PointerOutOfBoundary) {
  struct {
    const char* const html;
    int tokens_count;
    int last_token_size;
    bool last_token_has_tag_name;
    int last_token_tag_name_size;
  } test_data[] = {
      {"<a", 1, 2, true, 1},
      {"<a>", 1, 3, true, 1},
      {"<ab>", 1, 4, true, 2},
      {"<a汉", 1, 5, true, 4},
      {"<ab href", 1, 8, true, 2},
      {"<ab href=", 1, 9, true, 2},
      {"<ab href=url", 1, 12, true, 2},
      {"<ab href='url", 1, 13, true, 2},
      {"<ab href='url'", 1, 14, true, 2},
      {"<ab href=\"url", 1, 13, true, 2},
      {"<ab href=\"url\"", 1, 14, true, 2},
      {"<ab", 1, 3, true, 2},
      {"<ab></a>", 2, 4, true, 1},
      {"<ab></ab", 2, 4, true, 2},
      {"<ab></ab>", 2, 5, true, 2},
      {"<ab></ab ", 2, 5, true, 2},
      {"<ab></ab a", 2, 6, true, 2},
      {"<ab></ab a=", 2, 7, true, 2},
      {"<ab></ab a='v", 2, 9, true, 2},
      {"<ab></ab a='v'", 2, 10, true, 2},
      {"<ab></ab a=\"v", 2, 9, true, 2},
      {"<ab></ab a=\"v\"", 2, 10, true, 2},
      {"<ab></</ab a=\"v\"", 2, 10, true, 2},
      {"<script>js</", 2, 4, false, 0},
      {"<script>js</<", 2, 5, false, 0},
  };
  for (int i = 0; i < sizeof(test_data) / sizeof(test_data[0]); ++i) {
    vector<HtmlToken*> tokens;
    lexer_.ParseToken(test_data[i].html, strlen(test_data[i].html), &tokens);
    EXPECT_EQ(test_data[i].tokens_count, tokens.size()) << "token size:" << i;
    EXPECT_EQ(test_data[i].last_token_size, tokens.back()->GetLength())
        << "last token size: " << i;
    if (test_data[i].last_token_has_tag_name) {
      EXPECT_FALSE(tokens[0]->GetTagName().Empty())
          << "tag name of last token is null: " << i;
      EXPECT_EQ(test_data[i].last_token_tag_name_size,
          tokens.back()->GetTagName().GetLength())
          << "tag name of last token size: " << i;
    } else {
      EXPECT_TRUE(tokens.back()->GetTagName().Empty())
          << "tag name of last token is not null: " << i;
    }
  }
}
}
