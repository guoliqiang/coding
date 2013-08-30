// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <map>
#include <set>

#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/gtl/public/stl_util-inl.h"
#include "third_part/parser_master/public/html_lexer.h"
#include "third_part/parser_master/public/parse_master.h"

using base::utfcharsize;

namespace parse_master {
class HtmlLexerUnittest: public testing::Test {
 public:
 protected:
  HtmlLexer lexer_;
  virtual void SetUp() {
  }
  virtual void TearDown() {
  }
};

TEST_F(HtmlLexerUnittest, UtfCharSize0) {
  std::string html;
  file::File::ReadFileToStringOrDie("./gbk.txt", &html);
  LOG(INFO) << html;
  const char * ptr = html.c_str();
  int word_len = 0;
  int offset = 0;
  do {
    word_len = utfcharsize(ptr);
    LOG(INFO) << string(ptr, 0, word_len) << " " << word_len;
    ptr += word_len;
    offset += word_len;
  } while (offset < html.size());
}

TEST_F(HtmlLexerUnittest, UtfCharSize) {
  string html;
  html += static_cast<char>(0xfe);
  html += static_cast<char>(0xff);
  html += "abc<!DOCTYPE html>";
  StringToLowerASCII(&html);
  EXPECT_EQ(1, utfcharsize(html.c_str()));
  EXPECT_EQ(1, utfcharsize(html.c_str() + 1));
}

TEST_F(HtmlLexerUnittest, Style) {
  string html("<style><!--#loginpes1,.legend{display:none};-->"
              "</style//去新旧>1234567890");
  vector<HtmlToken*> tokens;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(4, tokens.size());
  EXPECT_EQ("<style>", lexer_.GetStringInBuff(tokens[0]->GetBuffIndex()));
  EXPECT_EQ("<!--#loginpes1,.legend{display:none};-->",
            lexer_.GetStringInBuff(tokens[1]->GetBuffIndex()));
  EXPECT_EQ("</style//去新旧>",
            lexer_.GetStringInBuff(tokens[2]->GetBuffIndex()));
  EXPECT_EQ("1234567890", lexer_.GetStringInBuff(tokens[3]->GetBuffIndex()));
  EXPECT_EQ("style", lexer_.GetStringInBuff(tokens[2]->GetTagName()));
}

TEST_F(HtmlLexerUnittest, Style2) {
  string html("<style><!--#loginpes1,.legend{display:none};-->"
              "</style去新旧>1234567890");
  vector<HtmlToken*> tokens;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ("<style>", lexer_.GetStringInBuff(tokens[0]->GetBuffIndex()));
  EXPECT_EQ("<!--#loginpes1,.legend{display:none};-->",
            lexer_.GetStringInBuff(tokens[1]->GetBuffIndex()));
  EXPECT_EQ("</style去新旧>1234567890",
            lexer_.GetStringInBuff(tokens[2]->GetBuffIndex()));
}

TEST_F(HtmlLexerUnittest, Style3) {
  string html("<style><!--#loginpes1,.legend{display:none};-->"
              "</style去新旧></style>123");
  vector<HtmlToken*> tokens;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(5, tokens.size());
  EXPECT_EQ("<style>", lexer_.GetStringInBuff(tokens[0]->GetBuffIndex()));
  EXPECT_EQ("<!--#loginpes1,.legend{display:none};-->",
            lexer_.GetStringInBuff(tokens[1]->GetBuffIndex()));
  EXPECT_EQ("</style去新旧>",
            lexer_.GetStringInBuff(tokens[2]->GetBuffIndex()));
  EXPECT_EQ("</style>", lexer_.GetStringInBuff(tokens[3]->GetBuffIndex()));
  EXPECT_EQ("123", lexer_.GetStringInBuff(tokens[4]->GetBuffIndex()));
  EXPECT_EQ("style", lexer_.GetStringInBuff(tokens[3]->GetTagName()));
}

TEST_F(HtmlLexerUnittest, ParseToken) {
  int i = 0;
  string html;
  vector<HtmlToken*> tokens;
  string str;

  i = 0;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(0, tokens.size());
  tokens.clear();

  i = 0;
  html.assign("<! DOCTYPE html>");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[i]->GetType());
  tokens.clear();

  i = 0;
  html.assign("<DIV align=\"center\"><?xml:namespace prefix = \"uc\" />"
      "<?XML:NAMESPACE PREFIX = \"uc\" /><#-- not standard comment -->"
      "<div id=\"FreeDefinePlaceholderControl1_"
      "PresentationModeControlsContainer__ctl0_AdvertisArea\">");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(5, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[1]->GetType());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[2]->GetType());
  EXPECT_EQ(TOKEN_TYPE_COMMENT, tokens[3]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html += static_cast<char>(0xfe);
  html += static_cast<char>(0xff);
  html += "abc<!DOCTYPE html>";
  EXPECT_LT(10, html.size());
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
  EXPECT_EQ(5, tokens[0]->GetLength());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[1]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional"
      "//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(1, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[i]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<! DOCTYPE html><<html> yunrang.com </</html>end text");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(6, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[i]->GetType());
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[++i]->GetType());
  EXPECT_EQ("<", lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[++i]->GetType());
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[++i]->GetType());
  EXPECT_EQ(" yunrang.com ",
            lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[++i]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<div class=text id =\"div_id\"> yunrang.com </html"
      "<div><!-- ha<\"\">ha-->end text");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(6, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(2, tokens[i]->GetAttrs()->at(1).name_.GetLength());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<p onclick=\"var tag='<div>content</div>'\">text</p>");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<script><\\/script></script>"
      "<script>go with script </scr ipt></script>");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(6, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_SCRIPT, tokens[i]->GetType());
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<\\/script>"), str);
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  ++i;
  str.clear();
  EXPECT_EQ(TOKEN_TYPE_SCRIPT, tokens[i]->GetType());
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("go with script </scr ipt>"), str);
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<! DOCTYPE html><HTML></html>");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<! DOCTYPE html><html> pku.com </html>end text");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(5, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<html>"), str);
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string(" pku.com "), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("end text"), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<div class=text id=\"div_id\"> pku.com </html"
      "<div><!-- ha<\"\">ha-->end text");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(6, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<div class=text id=\"div_id\">"), str);
  EXPECT_EQ(2, tokens[i]->GetAttrs()->size());
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(0).name_));
  EXPECT_EQ(string("class"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(0).val_));
  EXPECT_EQ(string("text"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(1).name_));
  EXPECT_EQ(string("id"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(1).val_));
  EXPECT_EQ(string("div_id"), str);
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string(" pku.com "), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<!-- ha<\"\">ha-->"), str);
  EXPECT_EQ(TOKEN_TYPE_COMMENT, tokens[i]->GetType());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("end text"), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  tokens.clear();

  i = 0;
  html.clear();
  html.append("<img/><div class=text id=\"div_id\"> pku.com "
      "<script>hello \";//*/*//</script </html"
      "<div><!-- ha<\"\">ha-->end text<not_valid/>"
      "<span>< Prev</span>");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  str.clear();
  EXPECT_EQ(14, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_SELF_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::IMG, tokens[i]->GetTag());
  ++i;
  str.clear();
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<div class=text id=\"div_id\">"), str);
  EXPECT_EQ(2, tokens[i]->GetAttrs()->size());
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(0).name_));
  EXPECT_EQ(string("class"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(0).val_));
  EXPECT_EQ(string("text"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(1).name_));
  EXPECT_EQ(string("id"), str);
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetAttrs()->at(1).val_));
  EXPECT_EQ(string("div_id"), str);
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string(" pku.com "), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  EXPECT_EQ(string("<script>"), str);
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(TOKEN_TYPE_SCRIPT, tokens[i]->GetType());
  EXPECT_EQ(string("hello \";//*/*//"), str);
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SCRIPT, tokens[i]->GetTag());
  EXPECT_EQ(string("</script "), str);
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<!-- ha<\"\">ha-->"), str);
  EXPECT_EQ(TOKEN_TYPE_COMMENT, tokens[i]->GetType());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("end text"), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<not_valid/>"), str);
  EXPECT_EQ(TOKEN_TYPE_SELF_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(static_cast<HtmlTag*>(NULL), tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("<span>"), str);
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SPAN, tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("< prev"), str);
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  EXPECT_EQ(static_cast<HtmlTag*>(NULL), tokens[i]->GetTag());
  ++i;
  str.clear();
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string("</span>"), str);
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::SPAN, tokens[i]->GetTag());
  tokens.clear();
}

TEST_F(HtmlLexerUnittest, ParseDisclosedTags) {
  string html("<html><head><title><body><a><font>text</font");
  vector<HtmlToken*> tokens;
  string str;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(8, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[7]->GetType());
  EXPECT_EQ(&HtmlTag::FONT, tokens[7]->GetTag());
}

TEST_F(HtmlLexerUnittest, ParseDisclosedScriptTag) {
  string html("<html><head><title><body><a><font>text</font><script>js</");
  vector<HtmlToken*> tokens;
  string str;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(10, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_SCRIPT, tokens[9]->GetType());
  EXPECT_EQ(static_cast<HtmlTag*>(NULL), tokens[9]->GetTag());
  EXPECT_TRUE(tokens[9]->GetTagName().Empty());
  EXPECT_EQ(4, tokens[9]->GetLength());
}

TEST_F(HtmlLexerUnittest, ParseIncompleteTag) {
  int i = 0;
  string html;
  vector<HtmlToken*> tokens;
  string str;

  i = 0;
  html.clear();
  html.append("<p> abc <");
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(2, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  str.append(lexer_.GetStringInBuff(tokens[i]->GetBuffIndex()));
  EXPECT_EQ(string(" abc <"), str);
  tokens.clear();
}

TEST_F(HtmlLexerUnittest, ParseStyleClose) {
  int i = 0;
  string html("<!-- saved from url=(0022)http://www.sbs.com.au/ -->\n"
      "<html><body><div class=\"webkit-line-gutter-backdrop\"></div><table>"
      "<tbody><tr><td class=\"webkit-line-number\">"
      "<td class=\"webkit-line-content\"><span class=\"webkit-html-doctype\">"
      "&lt;!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" "
      "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"&gt; "
      "</span></td></tr><tr><td class=\"webkit-line-number\">"
      "<td class=\"webkit-line-content\"><span class=\"webkit-html-tag\">"
      "&lt;html <span class=\"webkit-html-attribute-name\">xmlns</span>");
  vector<HtmlToken*> tokens;
  string str;

  i = 0;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(TOKEN_TYPE_COMMENT, tokens[i]->GetType());
  ++i;
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::HTML, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::BODY, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::DIV, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::TABLE, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::TBODY, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::TR, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::TD, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(&HtmlTag::TD, tokens[i]->GetTag());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("span"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("span"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("td"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("tr"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("tr"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("td"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("td"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("span"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_OPEN_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("span"), tokens[i]->GetTag()->GetName());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[i]->GetType());
  ++i;
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[i]->GetType());
  EXPECT_EQ(string("span"), tokens[i]->GetTag()->GetName());
  for (vector<HtmlToken*>::iterator it = tokens.begin();
      it != tokens.end();
      ++it) {
  }
  tokens.clear();
}

TEST_F(HtmlLexerUnittest, ParseBigFile) {
  int i = 0;
  string html;
  file::File::ReadFileToString("./test_data/error5.html", &html);
  vector<HtmlToken*> tokens;
  string str;

  i = 0;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  tokens.clear();
}

TEST_F(HtmlLexerUnittest, ParseDoctypeInFile) {
  vector<HtmlToken*> tokens;
  string html;
  file::File::ReadFileToString("./test_data/193.html", &html);
  string html2;
  file::File::ReadFileToString("./test_data/196.html", &html2);

  StringToLowerASCII(&html);
  lexer_.ParseToken(html2.c_str(), html2.size(), &tokens);
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[1]->GetType());
  int num = tokens[0]->GetLength();
  tokens.clear();

  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  tokens.clear();

  StringToLowerASCII(&html);
  lexer_.ParseToken(html2.c_str(), html2.size(), &tokens);
  EXPECT_EQ(TOKEN_TYPE_DOCTYPE, tokens[1]->GetType());
  EXPECT_EQ(num, tokens[0]->GetLength());
  tokens.clear();
}

TEST_F(HtmlLexerUnittest, ParseLuntanFile) {
  string html;
  file::File::ReadFileToString("./test_data/luntan1.html", &html);
  vector<HtmlToken*> tokens;
  string str;

  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_LT(5, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[0]->GetType());
  tokens.clear();
  html.clear();
  str.clear();

  file::File::ReadFileToString("./test_data/luntan9.html", &html);
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_LT(5, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_TEXT_BLOCK, tokens[0]->GetType());
  tokens.clear();
  html.clear();
  str.clear();
}

TEST_F(HtmlLexerUnittest, ParseBadFile) {
  string html("<script>var a = \"</a>、<a href=''>\";</script>");
  vector<HtmlToken*> tokens;
  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_EQ(3, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[2]->GetType());
  EXPECT_EQ("</script>", lexer_.GetStringInBuff(tokens[2]->GetBuffIndex()));
  tokens.clear();
  html.clear();

  file::File::ReadFileToString("./test_data/bad.html", &html);
  string str;

  StringToLowerASCII(&html);
  lexer_.ParseToken(html.c_str(), html.size(), &tokens);
  EXPECT_LT(21, tokens.size());
  EXPECT_EQ(TOKEN_TYPE_CLOSE_TAG, tokens[36]->GetType());
  EXPECT_EQ("</script>", lexer_.GetStringInBuff(tokens[36]->GetBuffIndex()));
  tokens.clear();
  html.clear();
  str.clear();
}


TEST_F(HtmlLexerUnittest, CheckTokenPosition) {
  string cur_file;
  std::vector<std::string> files;
  file::File::GetFilesInDir("./test_data/", &files);
  for (int f = 0; f < files.size(); f++) {
    cur_file = files[f];
    if (cur_file.find(".svn") != string::npos) {
      continue;
    }
    LOG(INFO) << cur_file;
    string html;
    file::File::ReadFileToString(cur_file, &html);

    vector<HtmlToken*> tokens;
    StringToLowerASCII(&html);
    lexer_.ParseToken(html.c_str(), html.size(), &tokens);
    if (tokens.size() <= 0) {
      continue;
    }
    vector<HtmlToken*>::const_iterator it = tokens.begin();
    HtmlToken* last_token = *it;
    ++it;
    while (it != tokens.end()) {
      HtmlToken* token = *it;
      EXPECT_NE(0, token->GetBeginPos());
      EXPECT_NE(token->GetBeginPos(), token->GetEndPos());
      if (token->GetBeginPos() == token->GetEndPos()) {
        LOG(INFO) << lexer_.GetStringInBuff(last_token->GetBuffIndex());
      }
      EXPECT_LT(last_token->GetBeginPos(), token->GetBeginPos());
      EXPECT_EQ(last_token->GetEndPos(), token->GetBeginPos());
      EXPECT_LT(last_token->GetEndPos(), token->GetEndPos());
      last_token = token;
      ++it;
    }
  }
}
}
