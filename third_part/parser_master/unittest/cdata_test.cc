// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/parser_master/public/parse_master.h"
#include "third_part/parser_master/public/util.h"

using parse_master::ParseMaster;
using parse_master::DomNode;

class TestParser {
 public:
  TestParser() : touched_count_(0) {}

  void ParseOnCDATA(const DomNode& node) {
    string content;
    node.GetHtml(&content);
    string data;
    parse_master::GetCDATASectionText(content, &data);
    VLOG(2) << data;
    cdata_texts_.push_back(data);
    ++touched_count_;
  }

  int touched_count() {
    return touched_count_;
  }
  const vector<string>& cdata_texts() {
    return cdata_texts_;
  }
 private:
  int touched_count_;
  vector<string> cdata_texts_;
};

TEST(TestParser, RegistParserOnCDATA) {
  string path = "./test_data/cdata.xml";
  string page_str;
  file::File::ReadFileToStringOrDie(path, &page_str);

  ParseMaster parser_;
  TestParser p;
  parser_.RegistParserOnCDATA(
          base::NewPermanentCallback(&p, &TestParser::ParseOnCDATA));
  parser_.SetBuff(page_str);
  parser_.Parse();
  EXPECT_EQ(p.touched_count(), 33);
  EXPECT_EQ(p.cdata_texts()[0], "花园酒店50平方案例");
  EXPECT_EQ(p.cdata_texts()[1],
            "http://www.tudou.com/programs/view/3ybsoOIYoww/");
}
