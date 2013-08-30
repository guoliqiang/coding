// Copyright 2010. All Rights Reserved.
// Author: shunpingye@gmail.com (Shunping Ye)

//  这个文件主要是测试带有下标的xpath
#include <string>

#include "base/public/callback.h"
#include "../public/path_master.h"
#include "third_part/parser_master/public/parse_master.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "file/public/file.h"

using path_master::PathMaster;
using path_master::ParserParam;

using namespace std;  // NOLINT
using namespace base;  // NOLINT

class SimpleParser {
 public:
  //  NOTE: make all data members public so we can add more
  //  data members for more test cases easily
  bool touched;
  int touched_count;
  string tagname;
  string href;
  string title;
  vector<string> contents;
  vector<string> hrefs;

 public:
  SimpleParser()
    : touched(false),
      touched_count(0) {
  }
  bool ParseOnDiv(ParserParam* param) {
    touched = true;
    ++touched_count;
    string content;
    param->node()->GetContent(&content);
    contents.push_back(content);
    return true;
  }
  bool ParseOnLink(ParserParam* param) {
    touched = true;
    parse_master::DomNode* node = param->node();
    string tag;
    node->GetTagName(&tag);
    tagname = tag;
    node->GetAttribute("href", &href);
    hrefs.push_back(href);
    return true;
  }
  bool ParseOnTitle(ParserParam* param) {
    string content;
    param->node()->GetContent(&content);
    title = content;
    return true;
  }
};


TEST(XPathParser, test1) {
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
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  parser.RegistParser(
      parser_name,
      "<html>[0]<body>[1]<a class=\"link\">[1]",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnLink));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_TRUE(sp.touched);
  EXPECT_EQ(sp.tagname, string("A"));
  EXPECT_EQ(sp.href, string("http://www.google.com/"));
}

TEST(XPathParser, test2) {
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
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  parser.RegistParser(
      parser_name,
      "<html><body><a class=\"link\">[1]",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnLink));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();

  EXPECT_TRUE(sp.touched);
  EXPECT_EQ(sp.tagname, string("A"));
  EXPECT_EQ(sp.href, string("http://www.google.com/"));
}

TEST(XPathParser, test3) {
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
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  parser.RegistParser(
      parser_name,
      "<html>[1]<body><a class=\"link\">[1]",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnLink));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_FALSE(sp.touched);
  EXPECT_EQ(sp.tagname, string());
  EXPECT_EQ(sp.href, string());
}

TEST(XPathParser, test4) {
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
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  EXPECT_TRUE(parser.RegistParser(
      parser_name,
      "<html>[1]<body><a class=\"link\">[1]",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnLink)));

  EXPECT_TRUE(parser.RegistParser(
      parser_name,
      "<html><head><title>",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnTitle)));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_FALSE(sp.touched);
  EXPECT_TRUE(sp.tagname.empty());
  EXPECT_TRUE(sp.href.empty());
  EXPECT_EQ(sp.title, string("yeshunping"));
}

TEST(XPathParser, XpathWithRegx) {
  string html = "<html>"
                "<head>"
                "<title>"
                "yeshunping"
                "</title>"
                "</head>"
                "<body>"
                "<div id=\"post1\">"
                "post1"
                "</div>"
                "<div id=\"post1\">"
                "post2"
                "</div>"
                "<div id=\"title\">"
                "not post"
                "</div>"
                "</body>"
                "</html>";
  string parser_name = "test";
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  parser.RegistParser(
      parser_name,
      "<html><body><div id=\"reg(post\\d+)\">",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnDiv));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_TRUE(sp.touched);
  ASSERT_EQ(sp.touched_count, 2);
  EXPECT_EQ(sp.contents[0], "post1");
  EXPECT_EQ(sp.contents[1], "post2");
}

TEST(XPathParser, XpathWithRegxTest2) {
  string html = "<html>"
                "<head>"
                "<title>"
                "yeshunping"
                "</title>"
                "</head>"
                "<body>"
                "<a href=\"profile/1234.html\">"
                "user 1"
                "</a>"
                "<a href=\"profile/1235.html\">"
                "user 2"
                "</a>"
                "<a id=\"title\">"
                "not post"
                "</a>"
                "</body>"
                "</html>";
  string parser_name = "test";
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  parser.RegistParser(
      parser_name,
      "<html><body><a href=\"reg(profile/\\d+\\.html)\">",
      NewPermanentCallback(&sp, &SimpleParser::ParseOnDiv));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_TRUE(sp.touched);
  ASSERT_EQ(sp.touched_count, 2);
  EXPECT_EQ(sp.contents[0], "user 1");
  EXPECT_EQ(sp.contents[1], "user 2");
}
TEST(XPathParser, XpathWithRegxTest3) {
  string html;
  //  http://club.ent.sina.com.cn/default.php?s=user&a=profile&uid=1845125562
  string path = "./testdata/bbs.html";
  file::File::ReadFileToStringOrDie(path, &html);
  string parser_name = "test";
  PathMaster parser;
  parse_master::ParseMaster pm;
  parser.Init(&pm);
  SimpleParser sp;
  string xpath = "<a href=\"reg(http://club\\.ent\\.sina\\.com\\.cn"
      "/default\\.php\\?s=user&a=profile&uid=\\d+)\">";
  parser.RegistParser(
      parser_name,
      xpath,
      NewPermanentCallback(&sp, &SimpleParser::ParseOnLink));
  parser.ResetCurrentParser(parser_name);
  pm.SetBuff(html);
  pm.Parse();
  EXPECT_TRUE(sp.touched);
  ASSERT_EQ(sp.hrefs.size(), 20);
  EXPECT_EQ(sp.hrefs[0],
      "http://club.ent.sina.com.cn/default.php?s=user&a=profile&uid=2464637001");  // NOLINT
  EXPECT_EQ(sp.hrefs[1],
      "http://club.ent.sina.com.cn/default.php?s=user&a=profile&uid=2464637001");  // NOLINT
}
