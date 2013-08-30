// Copyright 2010. All Rights Reserved.
// Author: shunpingye@gmail.com (Shunping Ye)

#include <string>

#include "base/public/callback.h"
#include "../public/path_master.h"
#include "third_part/parser_master/public/parse_master.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

using path_master::PathMaster;
using path_master::ParserParam;

using namespace base;  // NOLINT

class Simpleypm {
 public:
  Simpleypm() {
    touched_ = false;
    tagname_ = "";
    href_ = "";
  }
  bool ParseOnLink(ParserParam* param) {
    touched_ = true;
    parse_master::DomNode* node = param->node();
    string tag;
    node->GetTagName(&tag);
    tagname_ = tag;
    string href;
    node->GetAttribute("href", &href);
    href_ = href;
    return true;
  }

  bool touched() { return touched_; }
  string& tagname() { return tagname_; }
  string& href() { return href_; }
 private:
  bool touched_;
  string tagname_;
  string href_;
};

TEST(XPathypm, test1) {
  string html = "<html>"
                "<head>"
                "<title>"
                "yeshunping"
                "</title>"
                "</head>"
                "<body>"
                "<p>"
                "hey"
                "</p>"
                "<A class=\"link\""
                " href=\"http://www.google.com/\">"
                "http://www.google.com/"
                "</A>"
                "</body>"
                "</html>";
  string parser_name = "test";
  PathMaster ypm;
  parse_master::ParseMaster pm;
  ypm.Init(&pm);
  Simpleypm sp;
  //  根据运行效果看，回调函数的参数，
  //  最后是一个迭代器和dom，这样既可以获得对应的父亲，孩子和儿子节点，
  ypm.RegistParser(
      parser_name,
      "<html><body><a class=\"link\">",
      NewPermanentCallback(&sp, &Simpleypm::ParseOnLink));
  ypm.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();

  //  先暂时置为false
  EXPECT_TRUE(sp.touched());
  EXPECT_EQ(sp.tagname(), string("A"));
  EXPECT_EQ(sp.href(), string("http://www.google.com/"));
}
