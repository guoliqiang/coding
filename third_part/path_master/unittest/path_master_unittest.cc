// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: pengyongyi@yunrang.com

#include <set>

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file.h"
#include "../public/path_master.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT

namespace path_master {

using parse_master::DomNode;

class PathMasterUnittest: public testing::Test {
 public:
  PathMaster* ypm;
  parse_master::ParseMaster* pm;
  string parser_name;
 protected:
  virtual void SetUp() {
    parser_name = "test";
    pm = new parse_master::ParseMaster;
    ypm = new PathMaster();
    ypm->Init(pm);
  }
  virtual void TearDown() {
    delete ypm;
    delete pm;
  }
};

class TestParser {
 public:
  TestParser() {
    opened_ = false;
    div_count_ = 0;
    first_node_ = NULL;
  }

  bool ParseDivId(ParserParam *param) {
    ++div_count_;
    DomNode *node = param->node();
    string s;
    node->GetAttribute("id", &s);
    string html;
    node->GetHtml(&html);
//    LOG(INFO) << "html:" << html;
    div_id_str_.append(s);
    div_id_str_.append(",");
    if (!first_node_) {
      first_node_ = node;
    }
    return true;
  }

  bool ParserActivate(ParserParam *param) {
    opened_ = true;
    return true;
  }

  bool opened_;
//  bool closed_;
  int div_count_;
  string div_id_str_;
  DomNode* first_node_;
};

TEST_F(PathMasterUnittest, ParseId) {
  string html("<html><head></head><body>abc def ghi"
              "<div></div>"
              "<div id></div>"
              "<div id=\"1\"></div>"
              "<div id=\"2\"></div>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div id>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(3, p.div_count_);
  EXPECT_EQ(",1,2,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, ParseTag) {
  string html("<html><head></head><body>abc def ghi"
              "<div></div>"
              "<div id></div>"
              "<div id=\"1\"></div>"
              "<div id=\"2\"></div>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(4, p.div_count_);
  EXPECT_EQ(",,1,2,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, ParseNest) {
  string html("<html><head></head>"
              "<body>abc def ghi"
              "<div>"
              "<div id=\"0\">"
              "<div id=\"1\">"
              "</div>"
              "<div id=\"2\">"
              "</div>"
              "</div>"
              "</div>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div><div>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(3, p.div_count_);

  EXPECT_EQ("1,2,0,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, ParseExactMatch) {
  string html("<html><head></head><body>abc def ghi"
              "<div class=abc></div><div class=abc>"
              "<div id=\"a1\" class>"
              "<div id=\"a2\" class></div>"
              "</div></div>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div id class>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(2, p.div_count_);

  EXPECT_EQ("a2,a1,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, ParseMultiMatch) {
  string html("<html><head></head><body>abc def ghi"
              "<div id></div><div class><div id=\"a1\" class>"
              "<div id=\"a2\" class></div>"
              "</div></div>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div id class>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));

  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(2, p.div_count_);
  //  父节点的close在字节点之后
  EXPECT_EQ("a2,a1,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, ParseNodePosition) {
  string html("<html>7890<head>7890</head>890<body>7890abc def gh"
              "<div id>90</div><div class>890<div id=\"a1\" class>"
              "<div id=\"a2\"></div>"
              "</div></div>7890"
              "</body>890</html>890");
  TestParser p;
  ypm->RegistParser(parser_name, string("<div id class>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(1, p.div_count_);

  EXPECT_TRUE(p.first_node_);
  int begin = p.first_node_->GetHtmlOffset();
  int end = p.first_node_->GetHtmlOffset() +
      p.first_node_->GetHtmlSize();
  EXPECT_EQ(80, begin);
  EXPECT_EQ(html.size() - 30, end);
}

TEST_F(PathMasterUnittest, ParseDianpingProfile) {
  string html;
  const string& file =
      string("./testdata/dianpingProfile.html");
  file::File::ReadFileToStringOrDie(file, &html);
  TestParser p;
  ypm->RegistParser(
      parser_name,
      string("<div class=\"main_w\"><div class=\"asider_m0\">"
             "<div><dl class=\"avatar\"><dt>"),
             NewPermanentCallback(&p, &TestParser::ParseDivId));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(1, p.div_count_);

  EXPECT_TRUE(p.first_node_);
}

TEST_F(PathMasterUnittest, TestHtmlEnabler) {
  string html;
  const string& file =
      string("./testdata/dianpingProfile.html");
  file::File::ReadFileToStringOrDie(file, &html);
  TestParser p;
  ypm->RegistParser(
      parser_name,
      string("<html>"),
      NewPermanentCallback(&p, &TestParser::ParserActivate));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
}

TEST_F(PathMasterUnittest, TestBodyEnabler) {
  string html;
  const string& file =
      string("./testdata/dianpingProfile.html");
  file::File::ReadFileToStringOrDie(file, &html);

  TestParser p;
  ypm->RegistParser(
      parser_name,
      string("<body>"),
      NewPermanentCallback(&p, &TestParser::ParserActivate));

  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
}

TEST_F(PathMasterUnittest, TagWithUnderLine) {
  string html("<html><head></head><body>abc def ghi"
              "<table_name id=a2>hello world</table_name>"
              "<table_name id=a1>hi</table_name>"
              "</body></html>");
  TestParser p;
  ypm->RegistParser(parser_name, string("<table_name>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId));

  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();

  EXPECT_TRUE(ypm);
  EXPECT_EQ(2, p.div_count_);
  //  父节点的close在字节点之后
  EXPECT_EQ("a2,a1,", p.div_id_str_);
}

TEST_F(PathMasterUnittest, BadXpath) {
  string html("<html><head></head><body>abc def ghi"
              "<div id>"
              "</div>"
              "<div class>"
              "<div id=\"a1\" class>"
              "<div id=\"a2\" class>"
              "</div>"
              "</div>"
              "</div>"
              "</body></html>");
  TestParser p;
  //  Bad Xpath,
  // FIXME(yesp) :  after xpath can support space in tag, change this test case
  EXPECT_FALSE(ypm->RegistParser(parser_name,
      string("<div><div>< div >"),
      NewPermanentCallback(&p, &TestParser::ParseDivId)));
  EXPECT_FALSE(ypm->RegistParser(parser_name,
      string("<div><div><div>[abc]"),
      NewPermanentCallback(&p, &TestParser::ParseDivId)));
  EXPECT_TRUE(ypm->RegistParser(parser_name,
      string("<div><div><div>"),
      NewPermanentCallback(&p, &TestParser::ParseDivId)));
  ypm->ResetCurrentParser(parser_name);
  pm->SetBuff(html);
  pm->Parse();
  EXPECT_EQ(1, p.div_count_);
  }
}
