// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/public/logging.h"
#include "base/public/string_util.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/parser_master/public/html_lexer.h"
#include "third_part/parser_master/public/parse_master.h"


namespace parse_master {
const char* KPostParser = "post";

class ParseMasterUnittest: public testing::Test {
 public:
 protected:
  ParseMaster parser_;
  virtual void SetUp() {
    parser_.CreateParsers(KPostParser);
  }
  virtual void TearDown() {
  }
  const DomNode* Parse() {
    parser_.EnableParserOnly(parse_master::kDefaultParsers);
    return parser_.Parse();
  }
  const DomNode* ParseWithName(const string& parser_name) {
    parser_.EnableParserOnly(parser_name);
    return parser_.Parse();
  }

  void PreProcess(DomNode* node) {
    string tag_name;
    node->FetchTagNameToTail(&tag_name);
    if (node->Children().size() > 0) {
      for (int i = 0; i < node->Children().size(); i++) {
        if (node->Children()[i]->IsRoot() || node->Children()[i]->IsTextBlock())
          continue;
        string child_name;
        node->Children()[i]->FetchTagNameToTail(&child_name);
        PreProcess(node->Children()[i]);
      }
    }
  }
};

class TestParser {
 public:
  TestParser() {
    tag_open_count = tag_close_count = text_count = word_count_ = 0;
  }

  void ParseOnOpen(const DomNode& dom_node) {
    dom_node.FetchTagNameToTail(&tag_open_str_);
    tag_open_str_.append(",");
    tag_open_count++;
  }

  void ParseOnOpenTestAttr(const DomNode& dom_node) {
    tag_open_count++;
    dom_node.FetchTagNameToTail(&tag_open_str_);
    tag_open_str_.append(",");
    string val;
    string name("onclick");
    dom_node.FetchAttributeToTail(name, &val);
    attr_str_.append(name);
    attr_str_.append("=");
    attr_str_.append(val);
    attr_str_.append(",");
  }

  void ParseOnClose(const DomNode& dom_node) {
    dom_node.FetchTagNameToTail(&tag_close_str_);
    tag_close_str_.append(",");
    tag_close_count++;
  }

  void ParseOnText(const DomNode& dom_node) {
    text_count++;
    dom_node.FetchHtmlToTail(&text_str_);
    text_str_.append(",");
  }

  void ParseOnCloseContent(const DomNode& dom_node) {
    dom_node.FetchContentToTail(&text_str_);
    text_str_.append(",");
    tag_close_count++;
  }


  int tag_open_count;
  int tag_close_count;
  int text_count;
  int word_count_;
  string tag_open_str_;
  string tag_close_str_;
  string text_str_;
  string attr_str_;
};

class TextPrinter {
 public:
  void ParseOnText(const DomNode& dom_node) {
    string a;
    dom_node.FetchContentToTail(&a);
    str.append(a);
    str.append("\n");
  }
  string str;
};

class TestFormalHtmlBuilder {
 public:
  string& GetHtml() {
    return html_;
  }
  void Clear() {
    html_.clear();
  }
  void ParseOnOpen(const DomNode& dom_node) {
    html_.push_back('<');
    html_.append(dom_node.GetLowerTagName());
    for (HtmlAttrIterator it = dom_node.GetAttrIter(); it.NotEnd();
        it.Next()) {
      const string* attr_key = it.GetName();
      html_.push_back(' ');
      html_.append(*attr_key);
      if (it.GetValue()) {
        html_.push_back('=');
        html_.push_back('"');
        html_.append(*(it.GetValue()));
        html_.push_back('"');
      }
    }
    const HtmlTag* tag = dom_node.GetTag();
    if (tag != NULL && tag->IsSelfClosed()) {
      html_.append("/>");
    } else {
      html_.push_back('>');
    }
  }

  void ParseOnClose(const DomNode& dom_node) {
    const HtmlTag* tag = dom_node.GetTag();
    if (tag == NULL || !tag->IsSelfClosed()) {
      html_.append("</");
      html_.append(dom_node.GetLowerTagName());
      html_.push_back('>');
    }
  }

  void ParseOnText(const DomNode& dom_node) {
    dom_node.FetchContentToTail(&html_);
  }
 private:
  string html_;
};


TEST_F(ParseMasterUnittest, ParseToDom) {
  int i = 0;
  string html;
  string str;
  const DomNode* root = NULL;
  TestFormalHtmlBuilder html_builder;
  parser_.RegistNamedParserOnAllTagOpen(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnOpen));
  parser_.RegistNamedParserOnAllTagClose(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnClose));
  parser_.RegistNamedParserOnText(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnText));

  i = 0;
  html_builder.Clear();
  html.clear();
  html.append("<! DOCTYPE html>");
  parser_.SetBuff(html.c_str(), html.size());
  root = Parse();
  ParseWithName(KPostParser);
  EXPECT_NE(static_cast<DomNode*>(NULL), root);
  EXPECT_EQ(0, root->Children().size());

  i = 0;
  html_builder.Clear();
  html.clear();
  html.append("<! DOCTYPE html><HTML><p CLASS='STYLE'>InParagraph<LEX></html>"
              "<br>111222334");
  parser_.SetBuff(html.c_str(), html.size());
  Parse();
  root = ParseWithName(KPostParser);
  EXPECT_EQ(3, root->Children().size());
  EXPECT_EQ(&HtmlTag::HTML, root->Children()[0]->GetTag());
  root->Children()[0]->FetchHtmlToTail(&str);
  EXPECT_EQ(string("<HTML><p CLASS='STYLE'>InParagraph<LEX></html>"), str);
  EXPECT_EQ(&HtmlTag::BR, root->Children()[1]->GetTag());
  str.clear();
  root->Children()[1]->FetchHtmlToTail(&str);
  EXPECT_EQ(string("<br>"), str);
  EXPECT_EQ(static_cast<HtmlTag*>(NULL), root->Children()[2]->GetTag());
  EXPECT_TRUE(root->Children()[2]->IsTextBlock());
  str.clear();
  root->Children()[2]->FetchHtmlToTail(&str);
  EXPECT_EQ(string("111222334"), str);
  str.clear();
  root->FetchFormalHtmlToTail(&str);
  EXPECT_EQ(string("<html><p class=\"STYLE\">InParagraph<lex></lex></p>"
                   "</html><br/>111222334"),
      str);
  EXPECT_EQ(str, html_builder.GetHtml());
  str.clear();
  root->FetchFormalHtmlToTail(0, true, &str);
  EXPECT_EQ(string("<html><p class=\"STYLE\"><lex></lex></p></html><br/>"),
            str);
  str.clear();
  root->FetchFormalHtmlToTail(2, true, &str);
  EXPECT_EQ(string("<html><p class=\"STYLE\"></p></html><br/>"), str);
  str.clear();
  root->FetchFormalHtmlToTail(1, true, &str);
  EXPECT_EQ(string("<html></html><br/>"), str);
  EXPECT_EQ(0, root->GetDepth());

  i = 0;
  html_builder.Clear();
  html.clear();
  html.append("<! DOCTYPE html><html> yunrang.com </html>end text");
  parser_.SetBuff(html.c_str(), html.size());
  root = Parse();
  str.clear();
  root->FetchFormalHtmlToTail(&str);
  EXPECT_EQ(string("<html> yunrang.com </html>end text"), str);
  EXPECT_EQ(0, root->GetDepth());

  i = 0;
  html_builder.Clear();
  html.clear();
  html.append("<div class=text id=\"div_id\"> yunrang.com </html"
      "<div><!-- ha<\"\">ha-->end text");
  parser_.SetBuff(html.c_str(), html.size());
  root = Parse();
  str.clear();
  root->FetchFormalHtmlToTail(&str);
  EXPECT_EQ(string("<div class=\"text\" id=\"div_id\"> yunrang.com "
      "<div><!-- ha<\"\">ha-->end text</div></div>"), str);
  EXPECT_EQ(0, root->GetDepth());

  i = 0;
  html_builder.Clear();
  html.clear();
  html.append("<img/><div class=text id='div_id'> yunrang.com <script>"
      "hello \";//*/*//</script </html<div><!-- ha<\"\">ha-->end text"
      "<not_valid/><open>");
  parser_.SetBuff(html.c_str(), html.size());
  Parse();
  root = ParseWithName(KPostParser);
  str.clear();
  root->FetchFormalHtmlToTail(&str);
  EXPECT_EQ(string("<img/><div class=\"text\" id=\"div_id\"> yunrang.com "
      "<script>hello \";//*/*//</script><div><!-- ha<\"\">ha-->end text"
      "<not_valid></not_valid><open></open></div></div>"), str);
  EXPECT_EQ(string("<img/><div class=\"text\" id=\"div_id\"> yunrang.com "
      "<script></script><div>end text"
      "<not_valid></not_valid><open></open></div></div>"),
      html_builder.GetHtml());
}

TEST_F(ParseMasterUnittest, ParseNormal) {
  string page_str("<html><head></head><body>abc def ghi"
      "</body></html>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  string tag("a");
  parser_.RegistParserOnCustomTagOpen(tag,
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnCustomTagClose(tag,
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(3, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,"), p.tag_open_str_);
  EXPECT_EQ(string("head,body,html,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ClosePreOpen) {
  string page_str("<html><head><body>abc def ghi"
      "</body><a><li><Li><a><td><Td><Tr><tr></html>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(11, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,a,li,Li,a,td,Td,Tr,tr,"), p.tag_open_str_);
  EXPECT_EQ(string("head,body,li,Li,a,td,Td,Tr,tr,a,html,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ParseNormalXhtml) {
  string page_str("<html><head></head><body>abc def ghi<img/><br/>"
      "<input/><img/></body></html>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  const DomNode* root = Parse();
  EXPECT_EQ(7, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,img,br,input,img,"), p.tag_open_str_);
  EXPECT_EQ(string("head,img,br,input,img,body,html,"), p.tag_close_str_);
  string html;
  root->FetchFormalHtmlToTail(&html);
  EXPECT_EQ(string("<html><head></head><body>abc def ghi"
      "<img/><br/><input/><img/></body></html>"), html);
}

TEST_F(ParseMasterUnittest, SubContent) {
  string page_str("<a><font>text</font></a>");
  TestParser p;
  parser_.RegistParserOnTagClose(HtmlTag::A,
      base::NewPermanentCallback(&p, &TestParser::ParseOnCloseContent));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_close_count);

  EXPECT_EQ(string("text,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, AbnormalTagName) {
  string page_str("<abnormal-tag><abnormal_font>text"
      "</abnormal_font></abnormal-tag>");
  TestParser p;
  parser_.RegistParserOnCustomTagOpen("abnormal-tag",
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnCustomTagOpen("abnormal_font",
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(2, p.tag_open_count);

  EXPECT_EQ(string("abnormal-tag,abnormal_font,"), p.tag_open_str_);
}

TEST_F(ParseMasterUnittest, ParseDisclosedTags) {
  string page_str("<html><head><title><body><a><font>text</font");
  TestParser p;
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(6, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
}

TEST_F(ParseMasterUnittest, ParseScriptTag) {
  string page_str("<script>var str=\"<br/><img>\";/**///\n</script>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(1, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string("script,"), p.tag_open_str_);
  EXPECT_EQ(string("script,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ParseScriptTag2) {
  string page_str("<script src=\"http://img3.douban.com/js/packed_douban99.js"
      "\"></script>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(1, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string("script,"), p.tag_open_str_);
  EXPECT_EQ(string("script,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, TestStyle) {
  string page_str("<html><style>\n<!-- comment -->\n"
      "</style><head></head></html>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  const DomNode* root = Parse();
  EXPECT_EQ(3, p.tag_open_count);
  EXPECT_EQ(3, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string("html,style,head,"), p.tag_open_str_);
  EXPECT_EQ(string("style,head,html,"), p.tag_close_str_);
  string html;
  root->GetFormalHtml(&html);
  EXPECT_EQ(page_str, html);
}

TEST_F(ParseMasterUnittest, ParseComment) {
  string page_str("<!-- <script>var str=\"<br/><img>\";</script> -->");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(0, p.tag_open_count);
  EXPECT_EQ(0, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string(""), p.tag_open_str_);
}

TEST_F(ParseMasterUnittest, ParseDoctype) {
  string page_str("<!DOCTYPE ...><html></html>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(1, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string(""), p.text_str_);
  EXPECT_EQ(string("html,"), p.tag_open_str_);
  EXPECT_EQ(string("html,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ParseCommentNotClose) {
  string page_str("<!-- ");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(0, p.tag_open_count);
  EXPECT_EQ(0, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string(""), p.tag_open_str_);
}

class TestAttributeParser {
 public:
  TestAttributeParser() {}

  void ParseTagAAttribute(const DomNode& dom_node) {
    const string* attr_null = dom_node.GetLowerAttribute("not_href");
    EXPECT_TRUE(attr_null == NULL);
    const string* attr = dom_node.GetLowerAttribute("href");
    EXPECT_TRUE(attr != NULL);
    href_attr = *attr;
  }
  void ParseTagPAttribute(const DomNode& dom_node) {
    const string* attr = dom_node.GetLowerAttribute("class");
    EXPECT_TRUE(attr != NULL);
    class_attr = *attr;
    attr = dom_node.GetLowerAttribute("id");
    EXPECT_TRUE(attr != NULL);
    id_attr = *attr;
    attr = dom_node.GetLowerAttribute("dummy");
    EXPECT_TRUE(attr == NULL);
  }
  string href_attr;
  string class_attr;
  string id_attr;
  string dummy_attr;
};


TEST_F(ParseMasterUnittest, GetLowerAttribute) {
  string page_str("<a href=\"http://www.baidu.com/\"></a>"
      "<p class=\"TiTle\" id=\"article_Content\">content</p>");
  TestAttributeParser p;
  parser_.RegistParserOnTagClose(HtmlTag::A,
      base::NewPermanentCallback(&p, &TestAttributeParser::ParseTagAAttribute));
  parser_.RegistParserOnTagClose(HtmlTag::P,
      base::NewPermanentCallback(&p, &TestAttributeParser::ParseTagPAttribute));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ("http://www.baidu.com/", p.href_attr);
  EXPECT_EQ("title", p.class_attr);
  EXPECT_EQ("article_content", p.id_attr);
  EXPECT_EQ("", p.dummy_attr);
}

TEST_F(ParseMasterUnittest, ParseCData) {
  string page_str("<![CDATA[ ");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(0, p.tag_open_count);
  EXPECT_EQ(0, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string(""), p.tag_open_str_);
}

TEST_F(ParseMasterUnittest, ParseAttribute) {
  // this case not same as old parse master
  string page_str("<p onclick=\"var tag='<div>content</div>'\">text</p>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpenTestAttr));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(1, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("onclick=var tag='<div>content</div>',"), p.attr_str_);
  EXPECT_EQ(string("p,"), p.tag_open_str_);
  EXPECT_EQ(string("p,"), p.tag_close_str_);
  EXPECT_EQ(string("text,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseAttributeValue) {
  string page_str("<p onclick=\"var theTag='<div>Content</div>'\">Text</p>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpenTestAttr));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(1, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);
  EXPECT_EQ(string("onclick=var theTag='<div>Content</div>',"), p.attr_str_);
  EXPECT_EQ(string("p,"), p.tag_open_str_);
  EXPECT_EQ(string("p,"), p.tag_close_str_);
  EXPECT_EQ(string("Text,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseNotClosed) {
  string page_str("<html><head></head><body><p>abc def</p>ghi");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpenTestAttr));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(4, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(2, p.text_count);

  EXPECT_EQ(string("html,head,body,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,ghi,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteTag) {
  string page_str("<html><head></head><body><p>abc def<");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(4, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def<,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteTag2) {
  string page_str("<html><head></head><body><p>abc def</p><p class='' ");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteTag3) {
  string page_str("<p>abc def < hij klm</p>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("p,"), p.tag_open_str_);
  EXPECT_EQ(string("p,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def < hij klm,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteTagClose) {
  string page_str("<html><head></head><body><p>abc def</");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(4, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def</,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteAttribute) {
  string page_str("<html><head></head><body><p>abc def</p><p aaa");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteAttribute2) {
  string page_str("<html><head></head><body><p>abc def</p><p aaa ");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteAttribute3) {
  string page_str("<html><head></head><body><p>abc def</p><p aaa=");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteValue) {
  string page_str("<html><head></head><body><p>abc def</p><p aaa=\"");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteValue2) {
  string page_str("<html><head></head><body><p>abc def</p><p aaa='");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseIncompleteValue3) {
  string page_str("<html><head></head><body><p>abc def</p><p class=ab");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));

  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(5, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("html,head,body,p,p,"), p.tag_open_str_);
  EXPECT_EQ(string("head,p,p,body,html,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseCustomTag) {
  string page_str("<cr>abc def</cr>");
  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(1, p.text_count);

  EXPECT_EQ(string("cr,"), p.tag_open_str_);
  EXPECT_EQ(string("cr,"), p.tag_close_str_);
  EXPECT_EQ(string("abc def,"), p.text_str_);
}

TEST_F(ParseMasterUnittest, ParseScript3) {
  string page_str("<script>"
      "function $HeaderCKSearch(TT, KU, KN){"
      "var A=function(H){var G=H.match(/^\\s*(\\S+(\\s+\\S+)*)\\s*$/);"
      "return(G==null)?'':G[1]};var E=function(G){"
      "return document.getElementById(G)};"
      "var C=E('$h_c_k'),D=E('$h_c_b');C.onfocus=function(){"
      "var J=this;var I=A(J.value);if(TT==I){"
      "J.value='';J.className='on';}};"
      "C.onblur=function(){var K=this;var J=A(K.value);"
      "if(J=='' || J == TT){K.value=TT;K.className=''}};"
      "C.onkeydown=function(G){var G=G||window.event;"
      "if(G.keyCode==13){D.click()}};D.onclick=function(){"
      "var K=A(C.value);if(K != ''&&K != TT){"
      "K=A(K.replace(/:/g,' ').replace(/!/g,' ').replace(/\\./g,' ')"
      ".replace(/\\+/g,' ').replace(/\"/g,' ').replace(/;/g,' ')"
      ".replace(/%/g,' ').replace(/\\\\/g,' ').replace(/</g,' ')"
      ".replace(/>/g,' ').replace(/\\^/g,' ').replace(/\\'/g,' ')"
      ".replace(/\\//g,' '));}"
      "if (K==''&&!KN){alert('请输入搜索关键字!');C.focus();return false}"
      "var H=K==''||K==TT?KN:KU+encodeURIComponent(K);"
      "window.location.href=H;return false;};C.onblur();};"
      "$HeaderCKSearch(\"美食: 商户名、地址等...\", \"/search/keyword/2/10_\", "
      "\"/search/category/2/10\");</script>"
      "<ul class=\"tag\"><li>"
      "<a onclick=\"pageTracker._trackPageview('dp_home_navigate1_beijing')\""
      " href=\"/beijing/food\"  title=\"美食频道\" >"
      "<strong class=\"no\"  style=\"color:0\" >美食</strong></a></li><li>"
      "<a onclick=\"pageTracker._trackPageview('dp_home_navigate2_beijing')\""
      " href=\"/beijing/beauty\"  title=\"丽人频道\" >"
      "<strong class=\"no\"  style=\"color:0\" >丽人</strong></a></li><li>"
      "<a onclick=\"pageTracker._trackPageview('dp_home_navigate3_beijing')\""
      " href=\"/beijing/wedding\"  title=\"结婚频道\" >"
      "<strong class=\"no\"  style=\"color:0\" >结婚"
      "<span class=\"newIcon\" style=\"position:relative;top:-9px;\">"
      "<em class=\"newpic\"></em></span></strong></a></li><li>"
      "<a onclick=\"pageTracker._trackPageview('dp_home_navigate4_beijing')\""
      " href=\"/beijing/hotel\"  title=\"酒店\" >"
      "<strong class=\"no\"  style=\"color:0\" >酒店</strong></a></li><li>"
      "<a onclick=\"pageTracker._trackPageview('dp_home_navigate5_beijing')\""
      " href=\"/card/beijing\"  title=\"会员卡\" >"
      "<strong class=\"no\"  style=\"color:0\" >会员卡</strong></a></li>"
      "<li><a onclick=\"pageTracker._trackPageview('dp_header_ganji')\" "
      "href=\"http://www.ganji.com/\" target=\"_blank\" title=\"分类信息\">"
      "<strong class=\"no\">分类信息</strong></a></li></ul></div> "
      "<ul class=\"hdSub\"><li><a href=\"/shopall/2/10\">餐饮大全</a></li><li>"
      "<a href=\"/shoplist/best_2_10\""
      "onclick=\"pageTracker._trackPageview('dp_header_sencondnav');\">"
      "最佳餐厅</a></li><li><a href=\"/shoplist/taste_2_10\""
      "onclick=\"pageTracker._trackPageview('dp_header_sencondnav');\">口味最佳"
      "</a></li><li><a href=\"/shoplist/hot_2_10\""
      "onclick=\"pageTracker._trackPageview('dp_header_sencondnav');\">"
      "本周热门</a></li><li><a href=\"/shoplist/officelady_2_10\""
      "onclick=\"pageTracker._trackPageview('dp_header_sencondnav');\">"
      "白领女性最钟情</a></li><li><a href=\"/shoplist/student_2_10\""
      "onclick=\"pageTracker._trackPageview('dp_header_sencondnav');\">"
      "学生最喜欢</a></li><li><a href=\"/reviewlist/2/10_ab1\">"
      "热门点评</a></li><li><a href=\"/search/map/2\">"
      "地图搜索</a></li></ul></div><!-- Header Render : 0ms -->"
      "<form name=\"aspnetForm\" method=\"post\" id=\"aspnetForm\">\n");

  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_LT(1, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_LT(1, p.text_count);
}

TEST_F(ParseMasterUnittest, ParseScript4) {
  string page_str("<script>"
      "var c=Math.floor((b-g)/2);"
      "</script>");

  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string("script,"), p.tag_open_str_);
  EXPECT_EQ(string("script,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, ParseScript5) {
  string page_str("<script language=\"javascript\">"
      "if(parent.main){"
      "//alert(\"no\");"
      "}"
      "else{"
      "document.writeln(\"<!-- 导航 -->\");"
      "document.writeln(\"<div class=\\\"hd\\\">\");"
      "document.writeln(\"<!--hd--><\\/div>\");"
      "document.writeln(\"<!-- 导航结束 -->\");"
      "}"
      "</script>");

  TestParser p;
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TestParser::ParseOnText));
  parser_.RegistParserOnAllTagOpen(
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnAllTagClose(
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.SetBuff(page_str.c_str(), page_str.length());
  Parse();
  EXPECT_EQ(1, p.tag_open_count);
  EXPECT_EQ(p.tag_open_count, p.tag_close_count);
  EXPECT_EQ(0, p.text_count);

  EXPECT_EQ(string("script,"), p.tag_open_str_);
  EXPECT_EQ(string("script,"), p.tag_close_str_);
}

TEST_F(ParseMasterUnittest, TestGetToken) {
  string page_html("<meta></meta><div></div></meta><head><div></div>"
    "</head><body></body><br></br>");
  parser_.SetBuff(page_html.c_str(), page_html.length());
  const DomNode* root = Parse();
  EXPECT_EQ(5, root->Children().size());
}

TEST_F(ParseMasterUnittest, TestCustomTag) {
  string page_html("<div><cr><pre>");
  parser_.SetBuff(page_html.c_str(), page_html.length());
  const DomNode* root = Parse();
  EXPECT_EQ(1, root->Children().size());
  DomNode* div = root->Children()[0];
  EXPECT_FALSE(div->IsCustomTag());
  EXPECT_FALSE(div->IsTextBlock());
  DomNode* cr = div->Children()[0];
  EXPECT_TRUE(cr->IsCustomTag());
  EXPECT_FALSE(cr->IsTextBlock());
  DomNode* pre = cr->Children()[0];
  EXPECT_FALSE(pre->IsCustomTag());
  EXPECT_FALSE(pre->IsTextBlock());
}

TEST_F(ParseMasterUnittest, TestGetTextWithUserDefinedTag) {
  string page_html("<body>body<aaa>in aaa<bbb>in bbb</bbb></aaa>"
    "<ccc/><ddd/><eee/></body>");
  parser_.SetBuff(page_html.c_str(), page_html.length());
  const DomNode* root = Parse();
  DomNode* body = root->Children()[0];
  std::string str;
  body->FetchContentToTail(&str);
  EXPECT_EQ("bodyin aaain bbb", str);
  vector<const string*> text_blocks;
  body->GetContent(&text_blocks);
  EXPECT_EQ(3, text_blocks.size());
  EXPECT_EQ(string("body"), *(text_blocks[0]));
  EXPECT_EQ(string("in aaa"), *(text_blocks[1]));
  EXPECT_EQ(string("in bbb"), *(text_blocks[2]));
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile) {
  string html;
  file::File::ReadFileToString("./test_data/error2.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  const DomNode* root = Parse();
  EXPECT_EQ(4, root->Children().size());
  DomNode* node_html = root->Children()[1];
  EXPECT_EQ(&HtmlTag::HTML, node_html->GetTag());
  DomNode* node_body = root->Children()[3];
  EXPECT_EQ(&HtmlTag::BODY, node_body->GetTag());
}

TEST_F(ParseMasterUnittest, TestParseBigHtmlFile) {
  string html;
  file::File::ReadFileToString("./test_data/error5.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 10; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(4, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    DomNode* node_html = root->Children()[2];
    EXPECT_EQ(&HtmlTag::HTML, node_html->GetTag());
  }
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile2) {
  string html;
  file::File::ReadFileToString("./test_data/bad6.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 5; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(5, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    DomNode* node_html = root->Children()[1];
    EXPECT_TRUE(node_html->IsTextBlock());
  }
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile6) {
  string html;
  file::File::ReadFileToString("./test_data/error6.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 5; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(2, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    DomNode* node_html = root->Children()[0];
    EXPECT_TRUE(node_html->IsTextBlock());
    node_html = root->Children()[1];
    EXPECT_FALSE(node_html->IsTextBlock());
  }
}

TEST_F(ParseMasterUnittest, TestParseHtmlFileFormNode) {
  string html;
  TestParser p;
  file::File::ReadFileToString("./test_data/fenghuang.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  parser_.RegistParserOnTagOpen(HtmlTag::FORM,
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  parser_.RegistParserOnTagClose(HtmlTag::FORM,
      base::NewPermanentCallback(&p, &TestParser::ParseOnClose));
  parser_.RegistParserOnTagOpen(HtmlTag::TITLE,
      base::NewPermanentCallback(&p, &TestParser::ParseOnOpen));
  Parse();
  EXPECT_EQ(3, p.tag_close_count);
  EXPECT_EQ(4, p.tag_open_count);
}

TEST_F(ParseMasterUnittest, TestParseHtmlFileDoctype) {
  string html;
  TextPrinter p;
  file::File::ReadFileToString("./test_data/193.html", &html);
  string html2;
  file::File::ReadFileToString("./test_data/196.html", &html2);
  parser_.RegistParserOnText(
      base::NewPermanentCallback(&p, &TextPrinter::ParseOnText));

  parser_.SetBuff(html2.c_str(), html2.length());
  const DomNode* root = Parse();
//  LOG(INFO) << p.str;
  p.str.clear();
  int num = root->Children()[0]->GetContentLength();

  parser_.SetBuff(html.c_str(), html.length());
  Parse();
//  LOG(INFO) << p.str;
  p.str.clear();

  parser_.SetBuff(html2.c_str(), html2.length());
  root = Parse();
//  LOG(INFO) << p.str;
  p.str.clear();
  EXPECT_EQ(num, root->Children()[0]->GetContentLength());
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile7) {
  string html;
  file::File::ReadFileToString("./test_data/error7.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 5; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(3, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    DomNode* node_html = root->Children()[0];
    EXPECT_TRUE(node_html->IsTextBlock());
    node_html = root->Children()[1];
    EXPECT_FALSE(node_html->IsTextBlock());
  }
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile8) {
  string html;
  file::File::ReadFileToString("./test_data/error8.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  const DomNode* root = Parse();
  EXPECT_EQ(4, root->Children().size());
  string str;
  root->FetchFormalHtmlToTail(&str);
  vector<DomNode*> nodes;
  nodes.push_back(const_cast<DomNode*>(root));
  while (nodes.size() > 0) {
    DomNode* first = nodes.front();
    string str;
    first->FetchTagNameToTail(&str);
    if (first->GetType() != DOM_NODE_TAG) {
      EXPECT_EQ(0, str.size());
    } else {
      EXPECT_LT(0, str.size());
    }
    string path;
    if (first->IsRoot()) {
    } else if (first->IsTextBlock()) {
      first->FetchContentToTail(&path);
      path = string("/TEXT-") + IntToString(first->GetIndexOfParent())
          + ": " + path;
    } else {
      path.append("/");
      path.append(first->GetLowerTagName());
      path.append("-");
      path.append(IntToString(first->GetIndexOfParent()));
    }
    const DomNode* tmp = first->Parent();
    while (tmp != NULL && !tmp->IsRoot()) {
      path = "/" + tmp->GetLowerTagName() + "-" +
          IntToString(tmp->GetIndexOfParent()) + path;
      tmp = tmp->Parent();
    }
//    LOG(INFO) << path;
    for (int i = 0; i < first->ChildrenCount(); ++i) {
      EXPECT_TRUE(first->Children()[i]);
      if (first->Children()[i]->GetType() != DOM_NODE_SCRIPT) {
        EXPECT_LT(0, first->Children()[i]->GetHtmlSize()) <<
            first->Children()[i]->GetType();
      }
      nodes.push_back(first->Children()[i]);
    }
    nodes.erase(nodes.begin());
  }

  DomNode* node = NULL;
  for (int i = 0; i < root->Children().size(); i++) {
    if (root->Children()[i]->GetTag() == &HtmlTag::HTML) {
      node = root->Children()[i];
      break;
    }
  }

  EXPECT_TRUE(node);
  PreProcess(node);
}

TEST_F(ParseMasterUnittest, TestParseHtmlFile9) {
  string html;
  file::File::ReadFileToString("./test_data/error9.html", &html);
  TestFormalHtmlBuilder html_builder;
  parser_.RegistNamedParserOnAllTagOpen(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnOpen));
  parser_.RegistNamedParserOnAllTagClose(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnClose));
  parser_.RegistNamedParserOnText(KPostParser,
      base::NewPermanentCallback(
      &html_builder, &TestFormalHtmlBuilder::ParseOnText));
  parser_.SetBuff(html.c_str(), html.length());
  const DomNode* root = Parse();
  ParseWithName(KPostParser);
  EXPECT_EQ(2, root->Children().size());
  string str;
  root->FetchFormalHtmlToTail(&str);
  // EXPECT_EQ(str, html_builder.GetHtml());
  vector<DomNode*> nodes;
  nodes.push_back(const_cast<DomNode*>(root));
  while (nodes.size() > 0) {
    DomNode* first = nodes.front();
    string str;
    first->FetchTagNameToTail(&str);
    if (first->GetType() != DOM_NODE_TAG) {
      EXPECT_EQ(0, str.size());
    } else {
      EXPECT_LT(0, str.size());
    }
    string temp;
    first->FetchContentToTail(&temp);
    string path;
    if (first->IsTextBlock()) {
      first->FetchContentToTail(&path);
      path = string("/TEXT-") + IntToString(first->GetIndexOfParent())
          + ": " + path;
    } else if (first->GetType() == DOM_NODE_TAG) {
      path.append("/");
      path.append(first->GetLowerTagName());
      path.append("-");
      path.append(IntToString(first->GetIndexOfParent()));
    }
    const DomNode* tmp = first->Parent();
    while (tmp != NULL && !tmp->IsRoot()) {
      path = "/" + tmp->GetLowerTagName() + "-" +
          IntToString(tmp->GetIndexOfParent()) + path;
      tmp = tmp->Parent();
    }
//    LOG(INFO) << path;
    for (int i = 0; i < first->Children().size(); ++i) {
      EXPECT_TRUE(first->Children()[i]);
      if (first->Children()[i]->GetType() != DOM_NODE_SCRIPT) {
        EXPECT_LT(0, first->Children()[i]->GetHtmlSize()) <<
            first->Children()[i]->GetType();
      }
      nodes.push_back(first->Children()[i]);
    }
    nodes.erase(nodes.begin());
  }
}

TEST_F(ParseMasterUnittest, TestParseHtmlLuntanFile) {
  string html;
  file::File::ReadFileToString("./test_data/luntan1.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 5; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(6, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    int i = 0;
    DomNode* node_html = root->Children()[i];
    EXPECT_TRUE(node_html->IsTextBlock());
    str.clear();
    node_html->FetchContentToTail(&str);
    EXPECT_NE(string(""), str);

    node_html = root->Children()[++i];
    EXPECT_TRUE(node_html->IsTextBlock());
    str.clear();
    node_html->FetchContentToTail(&str);
    EXPECT_NE(string(""), str);

    node_html = root->Children()[++i];
    EXPECT_FALSE(node_html->IsTextBlock());
    EXPECT_EQ(&HtmlTag::HTML, node_html->GetTag());

    node_html = root->Children()[++i];
    EXPECT_TRUE(node_html->IsTextBlock());
    str.clear();
    node_html->FetchContentToTail(&str);
    EXPECT_EQ(string("\n"), str);

    node_html = root->Children()[++i];
    EXPECT_FALSE(node_html->IsTextBlock());
    EXPECT_EQ(&HtmlTag::SCRIPT, node_html->GetTag());

    node_html = root->Children()[++i];
    EXPECT_TRUE(node_html->IsTextBlock());
    str.clear();
    node_html->FetchContentToTail(&str);
    EXPECT_EQ(string("\n"), str);
  }
  html.clear();

  file::File::ReadFileToString("./test_data/luntan9.html", &html);
  parser_.SetBuff(html.c_str(), html.length());
  for (int i = 0; i < 5; ++i) {
    const DomNode* root = Parse();
    EXPECT_EQ(6, root->Children().size());
    string str;
    root->FetchFormalHtmlToTail(&str);
    DomNode* node_html = root->Children()[0];
    EXPECT_TRUE(node_html->IsTextBlock());
    node_html = root->Children()[1];
    EXPECT_TRUE(node_html->IsTextBlock());
  }
}

class AppendParser {
 public:
  void OnOpen(const DomNode& node) {
    CheckDepth(node);
    ++tag_open_count;
    node.FetchTagNameToTail(&tag_open_str_);
    tag_open_str_.append(",");
    string val;
    string name("onclick");
    node.FetchAttributeToTail(name, &val);
    attr_str_.append(name);
    attr_str_.append("=");
    attr_str_.append(val);
    attr_str_.append(",");
    for (HtmlAttrIterator it = node.GetAttrIter(); it.NotEnd(); it.Next()) {
      const string* attr_name = it.GetName();
      attr_str_.append(",");
      attr_str_.append(*attr_name);
      const string* value = node.GetAttribute(*attr_name);
      if (value) {
        attr_str_.append(*(value));
      }
    }

    EXPECT_GT(buff_len_, node.GetHtmlOffset());
    EXPECT_GE(buff_len_, node.GetHtmlOffset() + node.GetHtmlSize());

    EXPECT_NE(0, node.GetHtmlSize());
    if (last_start_pos_ > 0) {
      EXPECT_LT(last_start_pos_, node.GetHtmlOffset());
    } else {
      EXPECT_LE(last_start_pos_, node.GetHtmlOffset());
    }
    EXPECT_NE(0, node.GetHtmlSize());
    last_start_pos_ = node.GetHtmlOffset();
  }

  void Reset(int buff_len) {
    last_end_pos_ = last_start_pos_ = 0;
    last_close_node_ = NULL;
    buff_len_ = buff_len;
  }

  void OnClose(const DomNode& dom_node) {
    CheckDepth(dom_node);
    ++tag_close_count;
    dom_node.FetchTagNameToTail(&tag_close_str_);
    tag_close_str_.append(",");

    EXPECT_GT(buff_len_, dom_node.GetHtmlOffset());
    EXPECT_GE(buff_len_, dom_node.GetHtmlOffset() + dom_node.GetHtmlSize());

    EXPECT_NE(0, dom_node.GetHtmlSize());
    EXPECT_LE(last_end_pos_, dom_node.GetHtmlOffset() + dom_node.GetHtmlSize());
    if (last_end_pos_ > (dom_node.GetHtmlOffset() + dom_node.GetHtmlSize())) {
      string tmp;
      last_close_node_->FetchHtmlToTail(&tmp);
      LOG(INFO) << "last size: " << last_close_node_->GetHtmlSize()
              << " of:" << tmp <<"||end";
      tmp.clear();
      dom_node.FetchHtmlToTail(&tmp);
      LOG(INFO) << "this size: " << dom_node.GetHtmlSize()
          << " of:" << tmp <<"||end";
    }
    last_close_node_ = &dom_node;
    last_end_pos_ = dom_node.GetHtmlOffset() + dom_node.GetHtmlSize();
  }

  void OnText(const DomNode& dom_node) {
    CheckDepth(dom_node);
    if (dom_node.GetContent().size() > 20) {
//      LOG(INFO) << dom_node.GetContent();
    }
    ++text_count;
    dom_node.FetchHtmlToTail(&text_str_);
    text_str_.append(",");

    EXPECT_GT(buff_len_, dom_node.GetHtmlOffset());
    EXPECT_GE(buff_len_, dom_node.GetHtmlOffset() + dom_node.GetHtmlSize());

    EXPECT_NE(0, dom_node.GetHtmlSize());
    if (last_start_pos_ > 0) {
      EXPECT_LT(last_start_pos_, dom_node.GetHtmlOffset());
    } else {
      EXPECT_LE(last_start_pos_, dom_node.GetHtmlOffset());
    }
    last_start_pos_ = dom_node.GetHtmlOffset();
    EXPECT_LE(last_end_pos_, dom_node.GetHtmlOffset() + dom_node.GetHtmlSize());
    last_end_pos_ = dom_node.GetHtmlOffset() + dom_node.GetHtmlSize();
    last_close_node_ = &dom_node;
  }


  void Clear() {
    tag_open_str_.clear();
    tag_close_str_.clear();
    text_str_.clear();
    attr_str_.clear();
  }

  void LogContent() {
    LOG(INFO) << tag_open_str_;
    LOG(INFO) << tag_close_str_;
    LOG(INFO) << text_str_;
    LOG(INFO) << attr_str_;
  }

  static AppendParser* Get() {
    return &INSTANCE;
  }

 private:
  AppendParser() {
    tag_open_count = tag_close_count = text_count = word_count_ = 0;
    Reset(0);
  }

  void CheckDepth(const DomNode& node) {
    int depth = 0;
    const DomNode* tmp = &node;
    while (tmp->Parent() != NULL) {
      ++depth;
      tmp = tmp->Parent();
    }
    EXPECT_EQ(depth, node.GetDepth());
  }
  int tag_open_count;
  int tag_close_count;
  int text_count;
  int word_count_;
  string tag_open_str_;
  string tag_close_str_;
  string text_str_;
  string attr_str_;
  int last_start_pos_;
  int last_end_pos_;
  int buff_len_;
  const DomNode* last_close_node_;
  static AppendParser INSTANCE;
};
AppendParser AppendParser::INSTANCE;

TEST_F(ParseMasterUnittest, TestParseAllTestFile) {
  string cur_file;
  AppendParser* p = AppendParser::Get();
  parser_.RegistParserOnAllTagOpen(p, &AppendParser::OnOpen);
  parser_.RegistParserOnAllTagClose(p, &AppendParser::OnClose);
  parser_.RegistParserOnText(p, &AppendParser::OnText);
//  for (int i = 0; i < HtmlTag::GetTagsCount(); ++i) {
//    parser_.RegistParserOnTagOpen(*HtmlTag::GetTag(i), p,
//        &AppendParser::OnOpen);
//    parser_.RegistParserOnTagClose(*HtmlTag::GetTag(i), p,
//        &AppendParser::OnClose);
//  }
  std::vector<std::string> files;
  file::File::GetFilesInDir("./test_data/", &files);
  for (int f = 0; f < files.size(); f++) {
    cur_file = files[f];
    if (cur_file.find(".svn") != string::npos) {
      continue;
    }
    string html;
    file::File::ReadFileToString(cur_file, &html);
    for (int i = 0; i < html.size() / 2; ++i) {
      string html_a(html.c_str() + i, html.length() - i);
      p->Reset(html_a.size());
      parser_.SetBuff(html_a.c_str(), html_a.size());
      Parse();
      if (i % 1000 == 0) {
//        p->LogContent();
        p->Clear();
        break;
      }
    }
//    LOG(INFO) << cur_file << "\r\r\r\r\r\r\r";
  }
}

TEST_F(ParseMasterUnittest, TestSerialize) {
  string cur_file;
  ParseMaster pm2;
  std::vector<std::string> files;
  file::File::GetFilesInDir("./test_data/", &files);
  for (int f = 0; f < files.size(); f++) {
    cur_file = files[f];
    if (cur_file.find(".svn") != string::npos) {
      continue;
    }
    for (int i = 0; i < 2; ++i) {
      bool text_mode = (i == 0);
      string html;
      file::File::ReadFileToString(cur_file, &html);
      parser_.SetBuff(html.c_str(), html.length());
      pm2.SetBuff(html.c_str(), html.length());
      Parse();
      string s_data;
      parser_.SerializeToString(text_mode, &s_data);
      pm2.UnserializeFromString(text_mode, s_data);

      string html1;
      string html2;
      parser_.GetDomRoot()->FetchFormalHtmlToTail(&html1);
      pm2.GetDomRoot()->FetchFormalHtmlToTail(&html2);
      EXPECT_EQ(html1, html2);
    }
  }
}

class TestTextParser {
 public:
  TestTextParser() : second_(false) {
    pm.RegistParserOnText(base::NewPermanentCallback(this,
                          &TestTextParser::ParseOnText));
  }
  ~TestTextParser() {
  }

  void ParseContent(const string& content) {
    pm.SetBuff(content.c_str(), content.length());
    pm.Parse();
    second_ = true;
  }
 private:
  ParseMaster pm;
  bool second_;

  void ParseOnText(const DomNode& node) {
    if (second_) {
//      LOG(INFO) << node.GetContent();
      EXPECT_NE("觊觎酸奶霸主 光明低调猎食优诺", node.GetContent());
    } else {
      node.GetContent();
    }
  }
  DISALLOW_COPY_AND_ASSIGN(TestTextParser);
};

TEST_F(ParseMasterUnittest, TestParseTwoFileGetContent) {
  TestTextParser parser;
  string content1;
  const string file1 = "./test_data/xinhua1.html";
  file::File::ReadFileToString(file1, &content1);
  parser.ParseContent(content1);

  string content2;
  const string file2 = "./test_data/xinhua2.html";
  file::File::ReadFileToString(file2, &content2);
  parser.ParseContent(content2);
}

class TestSelfCloseParser {
 public:
  TestSelfCloseParser()
    :text_count_(0) {
    pm_.RegistParserOnTagClose(HtmlTag::AREA,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::BASE,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::BASEFONT,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::BR,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::COL,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::FRAME,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::HR,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::IMG,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::INPUT,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::ISINDEX,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::KEYGEN,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::LINK,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::META,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::PARAM,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::SPACER,
        this, &TestSelfCloseParser::ParseOnText);
    pm_.RegistParserOnTagClose(HtmlTag::WBR,
        this, &TestSelfCloseParser::ParseOnText);
  }
  ~TestSelfCloseParser() {
  }

  void SetXmlMode(bool flag) {
    pm_.SetXmlMode(flag);
  }
  void ParseContent(const string& content) {
    pm_.SetBuff(content.c_str(), content.length());
    pm_.Parse();
  }
  int text_count() {
    return text_count_;
  }
  void Reset() {
    text_count_ = 0;
  }
 private:
  ParseMaster pm_;
  int text_count_;

  void ParseOnText(const DomNode& node) {
    string str;
    node.GetContent(&str);
    if (!str.empty()) {
      text_count_++;
    }
  }
  DISALLOW_COPY_AND_ASSIGN(TestSelfCloseParser);
};

TEST_F(ParseMasterUnittest, TestXMLMode) {
  TestSelfCloseParser parser;
  string content = "<area>test</area>"
      "<base>test</base>"
      "<basefont>test</basefont>"
      "<br>test</br>"
      "<col>test</col>"
      "<frame>test</frame>"
      "<hr>test</hr>"
      "<img>test</img>"
      "<input>test</input>"
      "<isindex>test</isindex>"
      "<keygen>test</kengen>"
      "<link>test</link>"
      "<meta>test</meta>"
      "<param>test</param>"
      "<spacer>test</spacer>"
      "<wbr>test</wbr>";
  parser.SetXmlMode(true);
  parser.ParseContent(content);
  EXPECT_EQ(parser.text_count(), 16);

  parser.Reset();
  parser.SetXmlMode(false);
  parser.ParseContent(content);
  EXPECT_EQ(parser.text_count(), 0);
}

class CallCountParser {
 public:
  CallCountParser() {
    Clear();
  }
  void Clear() {
    token_parser_call_count_ = 0;
  }
  int token_parser_call_count_;
 private:
  DISALLOW_COPY_AND_ASSIGN(CallCountParser);
};

TEST_F(ParseMasterUnittest, TestLineFeed) {
  string content("[\r\n"
      "<a\r\n"
      "nothing \t\thref=\"http://bbs1.people.com.cn/postDetail.do"
      "?boardId=2&treeView=1&view=2&id=110654493\"\r\n"
      "class=\"link-list\" target=\"_blank\"><font color=\"#ff0000\">"
      "【强国论坛邀您一同“追问”】国家保障性住房千千万，试问“我的保障房在哪？”"
      "</font></a>\r\n"
      "] ");
  ParseMaster pm;
  pm.SetBuff(content.c_str(), content.size());
  const DomNode* root = pm.Parse();
  EXPECT_EQ(3, root->ChildrenCount());
  EXPECT_EQ(&HtmlTag::A, root->Children()[1]->GetTag());
  EXPECT_EQ(string("http://bbs1.people.com.cn/postDetail.do"
      "?boardId=2&treeView=1&view=2&id=110654493"),
      *(root->Children()[1]->GetAttribute("href")));
  EXPECT_TRUE(root->Children()[1]->GetAttribute("nothing") != NULL);
  EXPECT_TRUE(root->Children()[1]->GetAttribute("nothing")->empty());
}

TEST_F(ParseMasterUnittest, TestSelfClosedLink) {
  string content("[\r\n"
      "<a href=\"http://dzh.mop.com/xbq/20110705/0/885F5OI26fdccaFO.shtml\"/>"
      "\r\n"
      "“赃款”用英语怎么说 \r\n"
      "</a> \r\n"
      "] ");
  ParseMaster pm;
  pm.SetBuff(content.c_str(), content.size());
  const DomNode* root = pm.Parse();
  EXPECT_EQ(3, root->ChildrenCount());
  EXPECT_EQ(&HtmlTag::A, root->Children()[1]->GetTag());
  EXPECT_EQ(string("http://dzh.mop.com/xbq/20110705/0/885F5OI26fdccaFO.shtml"),
      *(root->Children()[1]->GetAttribute("href")));
}

TEST_F(ParseMasterUnittest, TestSelfClosedLink2) {
  string content("<a href=http://dzh.mop.com/>");
  ParseMaster pm;
  pm.SetBuff(content.c_str(), content.size());
  const DomNode* root = pm.Parse();
  EXPECT_EQ(1, root->ChildrenCount());
  EXPECT_EQ(&HtmlTag::A, root->Children()[0]->GetTag());
  EXPECT_EQ(string("http://dzh.mop.com/"),
      *(root->Children()[0]->GetAttribute("href")));
}

TEST_F(ParseMasterUnittest, TestInnerHtml) {
  string content("<div><span>string</span></div>"
      "<div><![if ie6 end -><![CDATA[string]]></div>");
  ParseMaster pm;
  pm.SetBuff(content.c_str(), content.size());
  const DomNode* root = pm.Parse();
  EXPECT_EQ(2, root->ChildrenCount());
  string html;
  root->GetInnerHtml(&html);
  EXPECT_EQ(content, html);
  root->Children()[0]->GetInnerHtml(&html);
  EXPECT_EQ(string("<span>string</span>"), html);
  root->Children()[1]->GetInnerHtml(&html);
  EXPECT_EQ(2, root->Children()[1]->ChildrenCount());
  EXPECT_EQ(string("<![if ie6 end -><![CDATA[string]]>"), html);
}

class XpathTestParser {
 public:
  XpathTestParser() {}
  ~XpathTestParser() {}
  void ParseXpath(const DomNode& node) {
    xpath_ = node.GetXpath();
    reverse_xpath_ = node.GetReverseXpath();

    const char* xpath_expect[] = {
        "//html",
        "//html/body",
        "//html/body/p",
        "//html/body/p/a"
    };

    const char* r_xpath_expect[] = {
        "a//",
        "a/p//",
        "a/p/body//",
        "a/p/body/html//"
    };
    for (int i = 0; i < arraysize(xpath_expect); ++i) {
      string xpath;
      EXPECT_EQ(node.GetXpath(i + 1, &xpath), i + 1);
      EXPECT_EQ(xpath, xpath_expect[i]);

      string reversed_path;
      EXPECT_EQ(node.GetReverseXpath(i + 1, &reversed_path), i + 1);
      EXPECT_EQ(reversed_path, r_xpath_expect[i]);
    }

    string xpath;
    EXPECT_EQ(node.GetXpath(6, &xpath), 4);
    EXPECT_EQ(xpath, "//html/body/p/a");

    string reversed_path;
    EXPECT_EQ(node.GetReverseXpath(6, &reversed_path), 4);
    EXPECT_EQ(reversed_path, "a/p/body/html//");
  }
  string xpath() {
    return xpath_;
  }
  string reverse_xpath() {
    return reverse_xpath_;
  }
 private:
  string xpath_;
  string reverse_xpath_;
};

TEST(ParseMasterXpathUnittest, TestXpath) {
  string content("<html><head>tt</head>"
      "<body><p><a>link address</a></p></body></html>");
  XpathTestParser parser;
  ParseMaster pm;
  pm.RegistParserOnTagClose(HtmlTag::A, &parser, &XpathTestParser::ParseXpath);
  pm.SetBuff(content);
  pm.Parse();
  EXPECT_EQ(parser.xpath(), "//html/body/p/a");
  EXPECT_EQ(parser.reverse_xpath(), "a/p/body/html//");
}

class ReParseTestParser {
 public:
  ReParseTestParser()
    : parse_number_(0),
      post_parse_number_(0),
      other_parse_number_(0) {}

  void ParseOnLink(const DomNode& node) {
    string content;
    node.GetContent(&content);
    VLOG(2) << "ParseOnLink:\t" << content;
    ++parse_number_;
  }
  void PostParseOnLink(const DomNode& node) {
    ++post_parse_number_;
    string content;
    node.GetContent(&content);
    VLOG(2) << "PostParseOnLink:\t" << content;
  }
  void OtherParseOnLink(const DomNode& node) {
    ++other_parse_number_;
  }
  int parse_number_;
  int post_parse_number_;
  int other_parse_number_;
 private:
  DISALLOW_COPY_AND_ASSIGN(ReParseTestParser);
};


TEST(ParseMasterReParsehUnittest, TestReParse) {
  string post_parser = "post";
  ParseMaster pm;
  pm.CreateParsers(post_parser);
  ReParseTestParser p;
  pm.RegistParserOnTagClose(HtmlTag::A, &p, &ReParseTestParser::ParseOnLink);
  pm.RegistNamedParserOnTagClose(post_parser, HtmlTag::A,
      base::NewPermanentCallback(&p, &ReParseTestParser::PostParseOnLink));
  string page_str = "<html><head>head</head><body><a>anchor</a></body>";
  pm.SetBuff(page_str.c_str(), page_str.length());
  pm.EnableParserOnly(parse_master::kDefaultParsers);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 1);
  EXPECT_EQ(p.post_parse_number_, 0);
  EXPECT_EQ(p.other_parse_number_, 0);
  for (size_t i = 0; i < 10; ++i) {
    pm.EnableParserOnly(parse_master::kDefaultParsers);
    pm.Parse();
    pm.EnableParserOnly(post_parser);
    pm.Parse();
  }
  EXPECT_EQ(p.parse_number_, 11);
  EXPECT_EQ(p.post_parse_number_, 10);
  EXPECT_EQ(p.other_parse_number_, 0);
  for (size_t i = 0; i < 10; ++i) {
    pm.EnableParserOnly(post_parser);
    pm.Parse();
  }
  EXPECT_EQ(p.parse_number_, 11);
  EXPECT_EQ(p.post_parse_number_, 20);
  EXPECT_EQ(p.other_parse_number_, 0);
  string parser_name = "other_parser";
  pm.CreateParsers(parser_name);
  pm.RegistNamedParserOnTagClose(parser_name, HtmlTag::A,
      base::NewPermanentCallback(&p, &ReParseTestParser::OtherParseOnLink));
  pm.EnableParserOnly(parser_name);
  pm.Parse();
  //  here you cannot parse with not exist parser_name
  EXPECT_EQ(p.parse_number_, 11);
  EXPECT_EQ(p.post_parse_number_, 20);
  EXPECT_EQ(p.other_parse_number_, 1);
  for (size_t i = 0; i < 10; ++i) {
    pm.EnableParserOnly(parse_master::kDefaultParsers);
    pm.Parse();
  }
  EXPECT_EQ(p.parse_number_, 21);
  EXPECT_EQ(p.post_parse_number_, 20);
  EXPECT_EQ(p.other_parse_number_, 1);
}

TEST(ParseMasterReParsehUnittest, MaxBuffSize) {
  ParseMaster pm;
  string page_str = "<html><head>head</head><body>";
  //  more than 4M
  for (int i = 0; i < 1024 * 1024; ++i) {
    page_str += "<a>anchor</a>";
  }
  page_str += "</body></html>";
  pm.SetBuff(page_str.c_str(), page_str.length());
  const DomNode* root = pm.Parse();
  EXPECT_EQ(root->GetHtmlSize(), parse_master::KDefaultMaxBuffSize);
}

TEST(ParseMasterReParsehUnittest, SetMaxBuffSize) {
  ParseMaster pm;
  string page_str = "<html><head>head</head><body>";
  //  more than 4M
  for (int i = 0; i < 1024 * 1024; ++i) {
    page_str += "<a>anchor</a>";
  }
  page_str += "</body></html>";
  pm.SetMaxBuffSize(1024 * 1024 * 100);
  pm.SetBuff(page_str);
  const DomNode* root = pm.Parse();
  EXPECT_EQ(root->GetHtmlSize(), page_str.length());
}

class DisableTestParser {
 public:
  DisableTestParser()
    : parse_number_(0),
      post_parse_number_(0),
      custom_number_(0) {}

  void ParseOnLink(const DomNode& node) {
    string content;
    node.GetContent(&content);
    VLOG(2) << "ParseOnLink:\t" << content;
    ++parse_number_;
  }
  void PostParseOnLink(const DomNode& node) {
    ++post_parse_number_;
    string content;
    node.GetContent(&content);
    VLOG(2) << "PostParseOnLink:\t" << content;
  }
  void PostParseOnLink2(const DomNode& node) {
    ++custom_number_;
    string content;
    node.GetContent(&content);
    VLOG(2) << "PostParseOnLink2:\t" << content;
  }
  void Reset() {
    parse_number_ = 0;
    post_parse_number_ = 0;
    custom_number_ = 0;
  }
  int parse_number_;
  int post_parse_number_;
  int custom_number_;
 private:
  DISALLOW_COPY_AND_ASSIGN(DisableTestParser);
};

TEST(ParseMasterDisableUnittest, Disable) {
  string post_parser = "post";
  ParseMaster pm;
  pm.CreateParsers(post_parser);
  DisableTestParser p;
  pm.RegistParserOnTagClose(HtmlTag::A, &p, &DisableTestParser::ParseOnLink);
  pm.RegistNamedParserOnTagClose(post_parser, HtmlTag::A,
      base::NewPermanentCallback(&p, &DisableTestParser::PostParseOnLink));
  pm.RegistNamedParserOnCustomTagOpen(post_parser, "a",
      base::NewPermanentCallback(&p, &DisableTestParser::PostParseOnLink2));
  pm.RegistNamedParserOnCustomTagClose(post_parser, "a",
      base::NewPermanentCallback(&p, &DisableTestParser::PostParseOnLink2));

  string page_str = "<html><head>head</head><body><a>anchor</a></body></html>";
  pm.SetBuff(page_str);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 1);
  EXPECT_EQ(p.post_parse_number_, 1);
  EXPECT_EQ(p.custom_number_, 2);

  p.Reset();
  pm.DisableParser(post_parser);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 1);
  EXPECT_EQ(p.post_parse_number_, 0);
  EXPECT_EQ(p.custom_number_, 0);

  p.Reset();
  pm.EnableParser(post_parser);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 1);
  EXPECT_EQ(p.post_parse_number_, 1);
  EXPECT_EQ(p.custom_number_, 2);

  p.Reset();
  pm.EnableParserOnly(post_parser);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 0);
  EXPECT_EQ(p.post_parse_number_, 1);
  EXPECT_EQ(p.custom_number_, 2);

  p.Reset();
  pm.DisableParserOnly(post_parser);
  pm.Parse();
  EXPECT_EQ(p.parse_number_, 1);
  EXPECT_EQ(p.post_parse_number_, 0);
  EXPECT_EQ(p.custom_number_, 0);
}
}
