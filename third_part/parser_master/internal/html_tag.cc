// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include "../public/html_tag.h"
#include "base/public/hash_tables.h"
#include "../public/fast_trie_tree.h"

using base::hash_map;
using base::hash_set;
using std::string;
using std::vector;

namespace parse_master {
HtmlTagMap HtmlTag::tags_;
vector<const HtmlTag*> HtmlTag::ALL_TAG;
vector<HtmlTag*> HtmlTag::SELFCLOSED_TAGS;
vector<HtmlTag*> HtmlTag::MUST_HAS_CHILD_TAGS;

HtmlTag HtmlTag::A("a", false, true);  // a
HtmlTag HtmlTag::ABBR("abbr");  // abbr
HtmlTag HtmlTag::ACRONYM("acronym");  // acronym
HtmlTag HtmlTag::ADDRESS("address");  // address
HtmlTag HtmlTag::APPLET("applet");  // applet
HtmlTag HtmlTag::AREA("area", true);  // area
HtmlTag HtmlTag::ARTICLE("article");  // article
HtmlTag HtmlTag::ASIDE("aside");  // aside
HtmlTag HtmlTag::AUDIO("audio");  // audio
HtmlTag HtmlTag::B("b");  // b
HtmlTag HtmlTag::BASE("base", true);  // base
HtmlTag HtmlTag::BASEFONT("basefont", true);  // basefont
HtmlTag HtmlTag::BDO("bdo");  // bdo
HtmlTag HtmlTag::BGSOUND("bgsound");  // bgsound
HtmlTag HtmlTag::BIG("big");  // big
HtmlTag HtmlTag::BLINK("blink");  // blink
HtmlTag HtmlTag::BLOCKQUOTE("blockquote");  // blockquote
HtmlTag HtmlTag::BODY("body");  // body
HtmlTag HtmlTag::BR("br", true);  // br
HtmlTag HtmlTag::BUTTON("button");  // button
HtmlTag HtmlTag::CANVAS("canvas");  // canvas
HtmlTag HtmlTag::CAPTION("caption");  // caption
HtmlTag HtmlTag::CENTER("center");  // center
HtmlTag HtmlTag::CITE("cite");  // cite
HtmlTag HtmlTag::CODE("code");  // code
HtmlTag HtmlTag::COL("col", true);  // col
HtmlTag HtmlTag::COLGROUP("colgroup");  // colgroup
HtmlTag HtmlTag::DD("dd");  // dd
HtmlTag HtmlTag::DEL("del");  // del
HtmlTag HtmlTag::DFN("dfn");  // dfn
HtmlTag HtmlTag::DIR("dir");  // dir
HtmlTag HtmlTag::DIV("div");  // div
HtmlTag HtmlTag::DL("dl");  // dl
HtmlTag HtmlTag::DT("dt");  // dt
HtmlTag HtmlTag::EM("em");  // em
HtmlTag HtmlTag::EMBED("embed");  // embed
HtmlTag HtmlTag::FIELDSET("fieldset");  // fieldset
HtmlTag HtmlTag::FIGCAPTION("figcaption");  // figcaption
HtmlTag HtmlTag::FIGURE("figure");  // figure
HtmlTag HtmlTag::FONT("font");  // font
HtmlTag HtmlTag::FOOTER("footer");  // footer
HtmlTag HtmlTag::FORM("form");  // form
HtmlTag HtmlTag::FRAME("frame", true);  // frame
HtmlTag HtmlTag::FRAMESET("frameset");  // frameset
HtmlTag HtmlTag::H1("h1");  // h1
HtmlTag HtmlTag::H2("h2");  // h2
HtmlTag HtmlTag::H3("h3");  // h3
HtmlTag HtmlTag::H4("h4");  // h4
HtmlTag HtmlTag::H5("h5");  // h5
HtmlTag HtmlTag::H6("h6");  // h6
HtmlTag HtmlTag::HEAD("head");  // head
HtmlTag HtmlTag::HEADER("header");  // header
HtmlTag HtmlTag::HGROUP("hgroup");  // hgroup
HtmlTag HtmlTag::HR("hr", true);  // hr
HtmlTag HtmlTag::HTML("html");  // html
HtmlTag HtmlTag::I("i");  // i
HtmlTag HtmlTag::IFRAME("iframe");  // iframe
HtmlTag HtmlTag::IMAGE("image");  // image
HtmlTag HtmlTag::IMG("img", true);  // img
HtmlTag HtmlTag::INPUT("input", true);  // input
HtmlTag HtmlTag::INS("ins");  // ins
HtmlTag HtmlTag::ISINDEX("isindex", true);  // isindex
HtmlTag HtmlTag::KBD("kbd");  // kbd
HtmlTag HtmlTag::KEYGEN("keygen", true);  // keygen
HtmlTag HtmlTag::LABEL("label");  // label
HtmlTag HtmlTag::LEGEND("legend");  // legend
HtmlTag HtmlTag::LI("li");  // li
HtmlTag HtmlTag::LINK("link", true);  // link
HtmlTag HtmlTag::LISTING("listing");  // listing
HtmlTag HtmlTag::MAP("map");  // map
HtmlTag HtmlTag::MARK("mark");  // mark
HtmlTag HtmlTag::MARQUEE("marquee");  // marquee
HtmlTag HtmlTag::MENU("menu");  // menu
HtmlTag HtmlTag::META("meta", true);  // meta
HtmlTag HtmlTag::MULTICOL("multicol");  // multicol
HtmlTag HtmlTag::NAV("nav");  // nav
HtmlTag HtmlTag::NOBR("nobr");  // nobr
HtmlTag HtmlTag::NOEMBED("noembed");  // noembed
HtmlTag HtmlTag::NOFRAMES("noframes");  // noframes
HtmlTag HtmlTag::NOSCRIPT("noscript");  // noscript
HtmlTag HtmlTag::OBJECT("object");  // object
HtmlTag HtmlTag::OL("ol");  // ol
HtmlTag HtmlTag::OPTGROUP("optgroup");  // optgroup
HtmlTag HtmlTag::OPTION("option");  // option
HtmlTag HtmlTag::OUTPUT("output");  // output
HtmlTag HtmlTag::P("p");  // p
HtmlTag HtmlTag::PARAM("param", true);  // param
HtmlTag HtmlTag::PLAINTEXT("plaintext");  // plaintext
HtmlTag HtmlTag::PRE("pre");  // pre
HtmlTag HtmlTag::Q("q");  // q
HtmlTag HtmlTag::S("s");  // s
HtmlTag HtmlTag::SAMP("samp");  // samp
HtmlTag HtmlTag::SCRIPT("script");  // script
HtmlTag HtmlTag::SECTION("section");  // section
HtmlTag HtmlTag::SELECT("select");  // select
HtmlTag HtmlTag::SMALL("small");  // small
HtmlTag HtmlTag::SOURCE("source");  // source
HtmlTag HtmlTag::SPACER("spacer", true);  // spacer
HtmlTag HtmlTag::SPAN("span");  // span
HtmlTag HtmlTag::STRIKE("strike");  // strike
HtmlTag HtmlTag::STRONG("strong");  // strong
HtmlTag HtmlTag::STYLE("style");  // style
HtmlTag HtmlTag::SUB("sub");  // sub
HtmlTag HtmlTag::SUP("sup");  // sup
HtmlTag HtmlTag::TABLE("table");  // table
HtmlTag HtmlTag::TBODY("tbody");  // tbody
HtmlTag HtmlTag::TD("td");  // td
HtmlTag HtmlTag::TEXTAREA("textarea");  // textarea
HtmlTag HtmlTag::TFOOT("tfoot");  // tfoot
HtmlTag HtmlTag::TH("th");  // th
HtmlTag HtmlTag::THEAD("thead");  // thead
HtmlTag HtmlTag::TITLE("title");  // title
HtmlTag HtmlTag::TR("tr");  // tr
HtmlTag HtmlTag::TT("tt");  // tt
HtmlTag HtmlTag::U("u");  // u
HtmlTag HtmlTag::UL("ul");  // ul
HtmlTag HtmlTag::VAR("var");  // var
HtmlTag HtmlTag::VIDEO("video");  // video
HtmlTag HtmlTag::WBR("wbr", true);  // wbr
HtmlTag HtmlTag::XMP("xmp");  // xmp

HtmlTag::HtmlTag(const char* tag_name,
                 bool self_closed,
                 bool must_has_child) : tag_name_(tag_name),
                                        self_closed_(self_closed),
                                        must_has_child_(must_has_child) {
  tag_index_ = ALL_TAG.size();
  tags_.Put(tag_name, this);
  ALL_TAG.push_back(this);
  if (self_closed) {
    SELFCLOSED_TAGS.push_back(this);
    // arae base col frame input isindex keygen link meta param wbr
  }
  if (must_has_child) {
    MUST_HAS_CHILD_TAGS.push_back(this);
    // a
  }
}

const HtmlTag* HtmlTag::FindTag(const string &tag) {
  return tags_.Get(tag);
}

const HtmlTag* HtmlTag::FindTag(const char* tag_name, int tag_name_len) {
  return tags_.Get(tag_name, tag_name_len);
}

void HtmlTag::SetSomeTagsSelfClose(bool flag) {
  for (size_t i = 0; i < SELFCLOSED_TAGS.size(); ++i) {
    SELFCLOSED_TAGS[i]->SetSelfClosed(flag);
  }
  for (size_t i = 0; i < MUST_HAS_CHILD_TAGS.size(); ++i) {
    MUST_HAS_CHILD_TAGS[i]->SetMustHasChild(flag);
  }
}


HtmlTagManager::HtmlTagManager() {
  close_before_tags_ = new vector<const HtmlTag*>[HtmlTag::GetTagsCount()];
  close_before_tags_[HtmlTag::A.GetIndex()].push_back(&HtmlTag::A);
  close_before_tags_[HtmlTag::BODY.GetIndex()].push_back(&HtmlTag::HEAD);
  close_before_tags_[HtmlTag::P.GetIndex()].push_back(&HtmlTag::P);

  close_pre_tags_ = new hash_set<const HtmlTag*,
                                 HtmlTagHash>[HtmlTag::GetTagsCount()];
  close_pre_tags_[HtmlTag::DIV.GetIndex()].insert(&HtmlTag::A);
  close_pre_tags_[HtmlTag::LI.GetIndex()].insert(&HtmlTag::LI);
  close_pre_tags_[HtmlTag::TD.GetIndex()].insert(&HtmlTag::TD);
  close_pre_tags_[HtmlTag::TR.GetIndex()].insert(&HtmlTag::TD);
  close_pre_tags_[HtmlTag::TR.GetIndex()].insert(&HtmlTag::TR);
  close_pre_tags_[HtmlTag::OPTION.GetIndex()].insert(&HtmlTag::OPTION);
}

HtmlTagManager::~HtmlTagManager() {
  delete[] close_pre_tags_;
  delete[] close_before_tags_;
}

const vector<const HtmlTag*>& HtmlTagManager::GetCloseBeforeTags(
      const HtmlTag& tag) const {
  return close_before_tags_[tag.GetIndex()];
}

const hash_set<const HtmlTag*, HtmlTagHash>& HtmlTagManager::GetClosePreTags(
      const HtmlTag& tag) const {
  return close_pre_tags_[tag.GetIndex()];
}
}
