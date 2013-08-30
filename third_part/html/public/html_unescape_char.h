/*
 * Copyright 2010 <yunrang.com>
 * author jiangwang@yunrang.com (Wang Jiang)
 * TODO(james):
 *       1. move logic code to cc file is better,
 *       2. reference to w3c.org
 *       3. Use prefix tree
 * reference to: http://en.wikipedia.org/wiki/List_of_XML_and_HTML_character_entity_references#Character_entities_in_HTML
 */

#ifndef UTIL_HTML_HTML_UNESCAPE_CHAR_H_
#define UTIL_HTML_HTML_UNESCAPE_CHAR_H_

#include <stdlib.h>
#include <map>
#include <string>
#include "base/public/utf.h"
#include "base/public/string_util.h"

namespace util_html {

inline void SimpleEscapeHtml(std::string str, std::string* out) {
  std::string result;
  for (int i = 0; i < str.length(); ++i) {
    switch (str[i]) {
    case '<':
      out->append("&lt;");
      break;
    case '>':
      out->append("&gt;");
      break;
    case '&':
      out->append("&amp;");
      break;
    case '"':
      out->append("&quot;");
      break;
    case ' ':
      out->append("&nbsp;");
      break;
    case '\'':
      out->append("&apos;");
      break;
    default:
      out->append(1, str[i]);
      break;
    }
  }
}

class HtmlUtil {
 private:
  std::map<std::string, std::string> set_;
  std::map<std::string, std::string> ex_set_;
  void InsertPair(const std::string &s1, const std::string &s2) {
    set_.insert(std::pair<std::string, std::string>(s1, s2));
  }
 public:

  // input: an unescaped string. begin with '&', end with ';'
  // format: "&#CODE;" or "&NAME;"
  void Unescape(const std::string &es, std::string *os) {
    if (es[0] != '&') *os = es;
    int len = es.length();
    if (es[len-1] != ';') *os = es;
    if (es[1] == '#') {
      std::string code_s = es.substr(2, len-3);
      base::UniCodeStrToStr(code_s, os);
      if ("" == *os) *os = es;
    } else {
      std::string code_s = es.substr(1, len-2);
      Find(code_s, os);
      if ("" == *os) *os = es;
    }
  }

  void Find(const std::string s, std::string *os) {
    std::map<std::string, std::string>::iterator it = set_.find(s);
    if (it == set_.end()) {
      *os = "";
    } else {
      *os = it->second;
    }
  }

  // try to unscape an special xml character,
  // if no escaped char found, dest_len will be set to zero.
  // @param buf - The input stream to check an escape character
  // @param buf_len - The input stream length
  // @param skip_len - output, the length of the escaped string, if any
  // @param dest - output, the buffer to save unescaped char, if any
  // @param dest_len - output, the ouput UTF-8 encoded char length (max 5)
  void UnescapeChar(const char *buf, int buf_len,
                    int *skip_len, char *dest, int *dest_len) {
    *dest_len = 0;
    if (buf_len <= 3 || buf[0] !='&') return;
    const char *s = buf;
    const int MAX_ENTITY_LEN = 9;
    if (buf[1] == '#') {
      int iCode = 0;
      s += 2;
      buf_len -= 2;
      if (buf_len > MAX_ENTITY_LEN) buf_len = MAX_ENTITY_LEN;
      if (s[0] == 'x' || s[0] == 'X') {
        s++;
        for (int i = 0; isxdigit(*s) && i < buf_len; ++i) {
          iCode = iCode * 16 + (*s <= '9' ? (s[0] & 0x0F) : (s[0] & 0x0F) + 9);
          s++;
        }
      } else {
        for (int i = 0; '0' <= *s && *s <= '9' && i < buf_len; ++i) {
          iCode = iCode * 10 + (*s++) - '0';
        }
      }
      if (iCode == 0 || *s != ';') {
        *dest_len = 0;
        return;
      }
      *skip_len = s - buf + 1;
      *dest_len = base::runetochar(dest, &iCode);
      return;
    }

    if ((s[1] >= 'a' && s[1] <= 'z') || (s[1] >= 'A' && s[1] <= 'Z')) {
      while (*s && *s != ';' && s-buf <= MAX_ENTITY_LEN) {
        s++;
      }
      if (*s == ';') {
        std::string escaped(buf+1, (s - buf -1));
        StringToLowerASCII(&escaped);
        std::string unes;
        Find(escaped, &unes);
        if (unes.size() >0) {
          *skip_len = s - buf + 1;
          *dest_len = unes.size();
          strncpy(dest, unes.c_str(), unes.size());
        }
      }
    }
  }

  // TODO(james): reimplement this function
  uint32 UnescapeChar(const char *buf, int buf_len, int *skip_len) {
    char tmp_derefered_char[6];
    int len;
    UnescapeChar(buf, buf_len, skip_len, tmp_derefered_char, &len);
    if (*skip_len) {
      base::Rune cp;
      base::chartorune(&cp, tmp_derefered_char);
      return static_cast<uint32>(cp);
    }
    return 0;
  }

  void UnescapeString(const std::string& in, std::string *out) {
    UnescapeString(in.c_str(), in.length(), out);
  }

  void UnescapeString(const char* in, int len, std::string *out) {
    out->clear();
    out->reserve(len);
    char unescaped_buf[1024];
    const char * const buf_end = in + len;
    for (const char *buf = in; buf < buf_end;) {
      int dest_len = 0;
      int skip_len = 0;
      if (*buf == '&') {
        UnescapeChar(buf, buf_end - buf,
            &skip_len, unescaped_buf, &dest_len);
      }
      if (dest_len) {
        out->append(unescaped_buf, dest_len);
        buf += skip_len;
      } else {
        out->push_back(*buf++);
      }
    }
  }

  // 全角转半角，对应 0x21 - 0x7E 之间的半角ASCII码, 除减号之外
  void FullWidthToHalfWidth(const std::string& in, std::string *out) {
    FullWidthToHalfWidth(in.c_str(), in.length(), out);
  }

  void FullWidthToHalfWidth(const char* in, int len, std::string *out) {
    out->clear();
    out->reserve(len);
    const unsigned char* buf = reinterpret_cast<const unsigned char*>(in);
    for (int i = 0; i < len; ++i) {
      if (buf[i] == 0xEF && (i + 2) < len) {
        if (buf[i + 1] == 0xbc) {
          // ！-- ＿
          if (buf[i + 2] >= 0x81 && buf[i + 2] <= 0xBF) {
            out->push_back(buf[i + 2] - 0x60);
            i += 2;
            continue;
          }
        } else if (buf[i + 1] == 0xbd) {
          // ｀ -- ～
          if (buf[i + 2] >= 0x80 && buf[i + 2] <= 0x9E) {
            out->push_back(buf[i + 2] - 0x20);
            i += 2;
            continue;
          }
        }
      }
      out->push_back(buf[i]);
    }
  }

  void Normalize(const char* in, int len, std::string *out) {
    std::string tmp;
    UnescapeString(in, len, &tmp);
    FullWidthToHalfWidth(tmp, out);
  }

  HtmlUtil() {
    InsertPair("quot", "\"");
    InsertPair("amp", "&");
    InsertPair("apos", "'");
    InsertPair("lt", "<");
    InsertPair("gt", ">");
    InsertPair("nbsp", " ");
    InsertPair("iexcl", "¡");
    InsertPair("cent", "¢");
    InsertPair("pound", "£");
    InsertPair("curren", "¤");
    InsertPair("yen", "¥");
    InsertPair("brvbar", "¦");
    InsertPair("sect", "§");
    InsertPair("uml", "¨");
    InsertPair("copy", "©");
    InsertPair("ordf", "ª");
    InsertPair("laquo", "«");
    InsertPair("not", "¬");
    InsertPair("shy", " ");
    InsertPair("reg", "®");
    InsertPair("macr", "¯");
    InsertPair("deg", "°");
    InsertPair("plusmn", "±");
    InsertPair("sup2", "²");
    InsertPair("sup3", "³");
    InsertPair("acute", "´");
    InsertPair("micro", "µ");
    InsertPair("para", "¶");
    InsertPair("middot", "·");
    InsertPair("cedil", "¸");
    InsertPair("sup1", "¹");
    InsertPair("ordm", "º");
    InsertPair("raquo", "»");
    InsertPair("frac14", "¼");
    InsertPair("frac12", "½");
    InsertPair("frac34", "¾");
    InsertPair("iquest", "¿");
    InsertPair("Agrave", "À");
    InsertPair("Aacute", "Á");
    InsertPair("Acirc", "Â");
    InsertPair("Atilde", "Ã");
    InsertPair("Auml", "Ä");
    InsertPair("Aring", "Å");
    InsertPair("AElig", "Æ");
    InsertPair("Ccedil", "Ç");
    InsertPair("Egrave", "È");
    InsertPair("Eacute", "É");
    InsertPair("Ecirc", "Ê");
    InsertPair("Euml", "Ë");
    InsertPair("Igrave", "Ì");
    InsertPair("Iacute", "Í");
    InsertPair("Icirc", "Î");
    InsertPair("Iuml", "Ï");
    InsertPair("ETH", "Ð");
    InsertPair("Ntilde", "Ñ");
    InsertPair("Ograve", "Ò");
    InsertPair("Oacute", "Ó");
    InsertPair("Ocirc", "Ô");
    InsertPair("Otilde", "Õ");
    InsertPair("Ouml", "Ö");
    InsertPair("times", "×");
    InsertPair("Oslash", "Ø");
    InsertPair("Ugrave", "Ù");
    InsertPair("Uacute", "Ú");
    InsertPair("Ucirc", "Û");
    InsertPair("Uuml", "Ü");
    InsertPair("Yacute", "Ý");
    InsertPair("THORN", "Þ");
    InsertPair("szlig", "ß");
    InsertPair("agrave", "à");
    InsertPair("aacute", "á");
    InsertPair("acirc", "â");
    InsertPair("atilde", "ã");
    InsertPair("auml", "ä");
    InsertPair("aring", "å");
    InsertPair("aelig", "æ");
    InsertPair("ccedil", "ç");
    InsertPair("egrave", "è");
    InsertPair("eacute", "é");
    InsertPair("ecirc", "ê");
    InsertPair("euml", "ë");
    InsertPair("igrave", "ì");
    InsertPair("iacute", "í");
    InsertPair("icirc", "î");
    InsertPair("iuml", "ï");
    InsertPair("eth", "ð");
    InsertPair("ntilde", "ñ");
    InsertPair("ograve", "ò");
    InsertPair("oacute", "ó");
    InsertPair("ocirc", "ô");
    InsertPair("otilde", "õ");
    InsertPair("ouml", "ö");
    InsertPair("divide", "÷");
    InsertPair("oslash", "ø");
    InsertPair("ugrave", "ù");
    InsertPair("uacute", "ú");
    InsertPair("ucirc", "û");
    InsertPair("uuml", "ü");
    InsertPair("yacute", "ý");
    InsertPair("thorn", "þ");
    InsertPair("yuml", "ÿ");
    InsertPair("OElig", "Œ");
    InsertPair("oelig", "œ");
    InsertPair("Scaron", "Š");
    InsertPair("scaron", "š");
    InsertPair("Yuml", "Ÿ");
    InsertPair("fnof", "ƒ");
    InsertPair("circ", "ˆ");
    InsertPair("tilde", "˜");
    InsertPair("Alpha", "Α");
    InsertPair("Beta", "Β");
    InsertPair("Gamma", "Γ");
    InsertPair("Delta", "Δ");
    InsertPair("Epsilon", "Ε");
    InsertPair("Zeta", "Ζ");
    InsertPair("Eta", "Η");
    InsertPair("Theta", "Θ");
    InsertPair("Iota", "Ι");
    InsertPair("Kappa", "Κ");
    InsertPair("Lambda", "Λ");
    InsertPair("Mu", "Μ");
    InsertPair("Nu", "Ν");
    InsertPair("Xi", "Ξ");
    InsertPair("Omicron", "Ο");
    InsertPair("Pi", "Π");
    InsertPair("Rho", "Ρ");
    InsertPair("Sigma", "Σ");
    InsertPair("Tau", "Τ");
    InsertPair("Upsilon", "Υ");
    InsertPair("Phi", "Φ");
    InsertPair("Chi", "Χ");
    InsertPair("Psi", "Ψ");
    InsertPair("Omega", "Ω");
    InsertPair("alpha", "α");
    InsertPair("beta", "β");
    InsertPair("gamma", "γ");
    InsertPair("delta", "δ");
    InsertPair("epsilon", "ε");
    InsertPair("zeta", "ζ");
    InsertPair("eta", "η");
    InsertPair("theta", "θ");
    InsertPair("iota", "ι");
    InsertPair("kappa", "κ");
    InsertPair("lambda", "λ");
    InsertPair("mu", "μ");
    InsertPair("nu", "ν");
    InsertPair("xi", "ξ");
    InsertPair("omicron", "ο");
    InsertPair("pi", "π");
    InsertPair("rho", "ρ");
    InsertPair("sigmaf", "ς");
    InsertPair("sigma", "σ");
    InsertPair("tau", "τ");
    InsertPair("upsilon", "υ");
    InsertPair("phi", "φ");
    InsertPair("chi", "χ");
    InsertPair("psi", "ψ");
    InsertPair("omega", "ω");
    InsertPair("thetasym", "ϑ");
    InsertPair("upsih", "ϒ");
    InsertPair("piv", "ϖ");
    InsertPair("ensp", " ");
    InsertPair("emsp", " ");
    InsertPair("thinsp", " ");
    InsertPair("zwnj", " ");
    InsertPair("zwj", " ");
    InsertPair("lrm", " ");
    InsertPair("rlm", " ");
    InsertPair("ndash", "–");
    InsertPair("mdash", "—");
    InsertPair("lsquo", "‘");
    InsertPair("rsquo", "’");
    InsertPair("sbquo", "‚");
    InsertPair("ldquo", "“");
    InsertPair("rdquo", "”");
    InsertPair("bdquo", "„");
    InsertPair("dagger", "†");
    InsertPair("Dagger", "‡");
    InsertPair("bull", "•");
    InsertPair("hellip", "…");
    InsertPair("permil", "‰");
    InsertPair("prime", "′");
    InsertPair("Prime", "″");
    InsertPair("lsaquo", "‹");
    InsertPair("rsaquo", "›");
    InsertPair("oline", "‾");
    InsertPair("frasl", "⁄");
    InsertPair("euro", "€");
    InsertPair("image", "ℑ");
    InsertPair("weierp", "℘");
    InsertPair("real", "ℜ");
    InsertPair("trade", "™");
    InsertPair("alefsym", "ℵ");
    InsertPair("larr", "←");
    InsertPair("uarr", "↑");
    InsertPair("rarr", "→");
    InsertPair("darr", "↓");
    InsertPair("harr", "↔");
    InsertPair("crarr", "↵");
    InsertPair("lArr", "⇐");
    InsertPair("uArr", "⇑");
    InsertPair("rArr", "⇒");
    InsertPair("dArr", "⇓");
    InsertPair("hArr", "⇔");
    InsertPair("forall", "∀");
    InsertPair("part", "∂");
    InsertPair("exist", "∃");
    InsertPair("empty", "∅");
    InsertPair("nabla", "∇");
    InsertPair("isin", "∈");
    InsertPair("notin", "∉");
    InsertPair("ni", "∋");
    InsertPair("prod", "∏");
    InsertPair("sum", "∑");
    InsertPair("minus", "−");
    InsertPair("lowast", "∗");
    InsertPair("radic", "√");
    InsertPair("prop", "∝");
    InsertPair("infin", "∞");
    InsertPair("ang", "∠");
    InsertPair("and", "∧");
    InsertPair("or", "∨");
    InsertPair("cap", "∩");
    InsertPair("cup", "∪");
    InsertPair("int", "∫");
    InsertPair("there4", "∴");
    InsertPair("sim", "∼");
    InsertPair("cong", "≅");
    InsertPair("asymp", "≈");
    InsertPair("ne", "≠");
    InsertPair("equiv", "≡");
    InsertPair("le", "≤");
    InsertPair("ge", "≥");
    InsertPair("sub", "⊂");
    InsertPair("sup", "⊃");
    InsertPair("nsub", "⊄");
    InsertPair("sube", "⊆");
    InsertPair("supe", "⊇");
    InsertPair("oplus", "⊕");
    InsertPair("otimes", "⊗");
    InsertPair("perp", "⊥");
    InsertPair("sdot", "⋅");
    InsertPair("lceil", "⌈");
    InsertPair("rceil", "⌉");
    InsertPair("lfloor", "⌊");
    InsertPair("rfloor", "⌋");
    InsertPair("lang", "〈");
    InsertPair("rang", "〉");
    InsertPair("loz", "◊");
    InsertPair("spades", "♠");
    InsertPair("clubs", "♣");
    InsertPair("hearts", "♥");
    InsertPair("diams", "♦");
  }
};  // HtmlUtil
}  // namespace

#endif  // UTIL_HTML_HTML_UNESCAPE_CHAR_H_
