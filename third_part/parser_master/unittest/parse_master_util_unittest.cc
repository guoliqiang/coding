// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/parser_master/public/parse_master_util.h"

namespace parse_master {
class ParseMasterUtilUnittest: public testing::Test {
 public:
 protected:
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

TEST_F(ParseMasterUtilUnittest, ParseTitle) {
  string html("<html><head><table>table<title>test title</title></head>"
      "<body><a>test a</a><a></a>test_body</body></html>");
  ParseMaster pm;
  string title;
  ParseMasterUtil::GetTitle(html.c_str(), html.size(),
      &pm, &title);
  EXPECT_EQ(string("test title"), title);
}

TEST_F(ParseMasterUtilUnittest, ParseTitle2) {
  string html("<html><head><title>test title</head>"
      "<body></body></html>");
  ParseMaster pm;
  string title;
  ParseMasterUtil::GetTitle(html.c_str(), html.size(),
      &pm, &title);
  EXPECT_EQ(string("test title"), title);
}

TEST_F(ParseMasterUtilUnittest, ParseTitle3) {
  string html("<html><head><title>test title");
  ParseMaster pm;
  string title;
  ParseMasterUtil::GetTitle(html.c_str(), html.size(),
      &pm, &title);
  EXPECT_EQ(string("test title"), title);
}

TEST_F(ParseMasterUtilUnittest, ParseTitle4) {
  string html("<html><head><title></head>112233<body></body></html>");
  ParseMaster pm;
  string title;
  ParseMasterUtil::GetTitle(html.c_str(), html.size(),
                            &pm, &title);
  EXPECT_EQ(string(), title);
}

TEST_F(ParseMasterUtilUnittest, ParseAnchors) {
  string html("<html><head><title>test title</head>"
              "112233<body><a>a1</body><a>a2<table>table</a></html>");
  {
    ParseMaster pm;
    string title;
    ParseMasterUtil::GetTitle(html.c_str(), html.size(), &pm, &title);
    EXPECT_EQ(string("test title"), title);
  }
  {
    ParseMaster pm;
    vector<const DomNode*> anchors;
    ParseMasterUtil::GetAnchors(html.c_str(), html.size(), &pm, &anchors);
    EXPECT_EQ(2, anchors.size());
    for (int i = 0; i < anchors.size(); i++) {
      std::string foo;
      anchors[i]->GetContent(&foo);
      LOG(INFO) << foo;
    }
  }
}
}
