//  Copyright 2010 Yunrang Inc. All Rights Reserved.
//  Author: yongyipeng@yunrang.com (Yongyi PENG)

#ifndef UTIL_PARSE_MASTER2_PUBLIC_HTML_TAG_H_
#define UTIL_PARSE_MASTER2_PUBLIC_HTML_TAG_H_

#include <string>
#include <vector>

#include "base/public/flags.h"
#include "base/public/hash_tables.h"
#include "base/public/logging.h"
#include "base/public/singleton.h"
#include "third_part/testing/gtest/include/gtest/gtest_prod.h"

namespace util {
template<typename Val> class FastTrieTree;
}

namespace parse_master {
class HtmlTag;
typedef util::FastTrieTree<HtmlTag> HtmlTagMap;

class HtmlTag {
 public:
  static HtmlTag A;            // a
  static HtmlTag ABBR;         // abbr
  static HtmlTag ACRONYM;      // acronym
  static HtmlTag ADDRESS;      // address
  static HtmlTag APPLET;       // applet
  static HtmlTag AREA;         // area
  static HtmlTag ARTICLE;      // article
  static HtmlTag ASIDE;        // aside
  static HtmlTag AUDIO;        // audio
  static HtmlTag B;            // b
  static HtmlTag BASE;         // base
  static HtmlTag BASEFONT;     // basefont
  static HtmlTag BDO;          // bdo
  static HtmlTag BGSOUND;      // bgsound
  static HtmlTag BIG;          // big
  static HtmlTag BLINK;        // blink
  static HtmlTag BLOCKQUOTE;   // blockquote
  static HtmlTag BODY;         // body
  static HtmlTag BR;           // br
  static HtmlTag BUTTON;       // button
  static HtmlTag CANVAS;       // canvas
  static HtmlTag CAPTION;      // caption
  static HtmlTag CENTER;       // center
  static HtmlTag CITE;         // cite
  static HtmlTag CODE;         // code
  static HtmlTag COL;          // col
  static HtmlTag COLGROUP;     // colgroup
  static HtmlTag DD;           // dd
  static HtmlTag DEL;          // del
  static HtmlTag DFN;          // dfn
  static HtmlTag DIR;          // dir
  static HtmlTag DIV;          // div
  static HtmlTag DL;           // dl
  static HtmlTag DT;           // dt
  static HtmlTag EM;           // em
  static HtmlTag EMBED;        // embed
  static HtmlTag FIELDSET;     // fieldset
  static HtmlTag FIGCAPTION;   // figcaption
  static HtmlTag FIGURE;       // figure
  static HtmlTag FONT;         // font
  static HtmlTag FOOTER;       // footer
  static HtmlTag FORM;         // form
  static HtmlTag FRAME;        // frame
  static HtmlTag FRAMESET;     // frameset
  static HtmlTag H1;           // h1
  static HtmlTag H2;           // h2
  static HtmlTag H3;           // h3
  static HtmlTag H4;           // h4
  static HtmlTag H5;           // h5
  static HtmlTag H6;           // h6
  static HtmlTag HEAD;         // head
  static HtmlTag HEADER;       // header
  static HtmlTag HGROUP;       // hgroup
  static HtmlTag HR;           // hr
  static HtmlTag HTML;         // html
  static HtmlTag I;            // i
  static HtmlTag IFRAME;       // iframe
  static HtmlTag IMAGE;        // image
  static HtmlTag IMG;          // img
  static HtmlTag INPUT;        // input
  static HtmlTag INS;          // ins
  static HtmlTag ISINDEX;      // isindex
  static HtmlTag KBD;          // kbd
  static HtmlTag KEYGEN;       // keygen
  static HtmlTag LABEL;        // label
  static HtmlTag LEGEND;       // legend
  static HtmlTag LI;           // li
  static HtmlTag LINK;         // link
  static HtmlTag LISTING;      // listing
  static HtmlTag MAP;          // map
  static HtmlTag MARK;         // mark
  static HtmlTag MARQUEE;      // marquee
  static HtmlTag MENU;         // menu
  static HtmlTag META;         // meta
  static HtmlTag MULTICOL;     // multicol
  static HtmlTag NAV;          // nav
  static HtmlTag NOBR;         // nobr
  static HtmlTag NOEMBED;      // noembed
  static HtmlTag NOFRAMES;     // noframes
  static HtmlTag NOSCRIPT;     // noscript
  static HtmlTag OBJECT;       // object
  static HtmlTag OL;           // ol
  static HtmlTag OPTGROUP;     // optgroup
  static HtmlTag OPTION;       // option
  static HtmlTag OUTPUT;       // output
  static HtmlTag P;            // p
  static HtmlTag PARAM;        // param
  static HtmlTag PLAINTEXT;    // plaintext
  static HtmlTag PRE;          // pre
  static HtmlTag Q;            // q
  static HtmlTag S;            // s
  static HtmlTag SAMP;         // samp
  static HtmlTag SCRIPT;       // script
  static HtmlTag SECTION;      // section
  static HtmlTag SELECT;       // select
  static HtmlTag SMALL;        // small
  static HtmlTag SOURCE;       // source
  static HtmlTag SPACER;       // spacer
  static HtmlTag SPAN;         // span
  static HtmlTag STRIKE;       // strike
  static HtmlTag STRONG;       // strong
  static HtmlTag STYLE;        // style
  static HtmlTag SUB;          // sub
  static HtmlTag SUP;          // sup
  static HtmlTag TABLE;        // table
  static HtmlTag TBODY;        // tbody
  static HtmlTag TD;           // td
  static HtmlTag TEXTAREA;     // textarea
  static HtmlTag TFOOT;        // tfoot
  static HtmlTag TH;           // th
  static HtmlTag THEAD;        // thead
  static HtmlTag TITLE;        // title
  static HtmlTag TR;           // tr
  static HtmlTag TT;           // tt
  static HtmlTag U;            // u
  static HtmlTag UL;           // ul
  static HtmlTag VAR;          // var
  static HtmlTag VIDEO;        // video
  static HtmlTag WBR;          // wbr
  static HtmlTag XMP;          // xmp


  static const HtmlTag* FindTag(const std::string& tag);
  static const HtmlTag* FindTag(const char* tag_name, int tag_name_len);
  static void SetSomeTagsSelfClose(bool flag);
  static const HtmlTag* GetTag(int index) {
    return ALL_TAG[index];
  }
  inline static int GetTagsCount() {
    return ALL_TAG.size();
  }

  inline int GetIndex() const {
    return tag_index_;
  }

  inline const std::string& GetName() const {
    return tag_name_;
  }

  inline bool IsSelfClosed() const {
    return self_closed_;
  }

  inline bool IsMustHasChild() const {
    return must_has_child_;
  }

 private:
  HtmlTag(const char* tag_name,
          bool self_closed = false,
          bool must_has_child = false);

  void SetSelfClosed(bool flag) {
    self_closed_ = flag;
  }

  void SetMustHasChild(bool flag) {
    must_has_child_ = flag;
  }

  static std::vector<const HtmlTag*> ALL_TAG;
  static std::vector<HtmlTag*> SELFCLOSED_TAGS;
  static std::vector<HtmlTag*> MUST_HAS_CHILD_TAGS;
  static HtmlTagMap tags_;

  int tag_index_;
  const std::string tag_name_;
  bool self_closed_;
  bool must_has_child_;

  FRIEND_TEST(HtmlTagUnittest, InitTest);
  FRIEND_TEST(HtmlTagUnittest, InitTest2);
  DISALLOW_COPY_AND_ASSIGN(HtmlTag);
};

struct HtmlTagHash {
  size_t operator()(const HtmlTag* const tag) const {
    return tag->GetIndex();
  }
};

class HtmlTagManager {
 public:
  HtmlTagManager();
  ~HtmlTagManager();
  const std::vector<const HtmlTag*>&
        GetCloseBeforeTags(const HtmlTag& tag) const;
  const base::hash_set<const HtmlTag*, HtmlTagHash>&
        GetClosePreTags(const HtmlTag& tag) const;

 private:
  std::vector<const HtmlTag*>* close_before_tags_;
  base::hash_set<const HtmlTag*, HtmlTagHash>* close_pre_tags_;
  FRIEND_TEST(HtmlTagUnittest, InitTest);
  FRIEND_TEST(HtmlTagUnittest, InitTest2);
  DISALLOW_COPY_AND_ASSIGN(HtmlTagManager);
};

typedef Singleton<HtmlTagManager> HtmlTagManagerInstance;
}

#endif  // UTIL_PARSE_MASTER2_PUBLIC_HTML_TAG_H_
