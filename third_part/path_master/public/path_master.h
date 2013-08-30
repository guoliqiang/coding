// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_YPATH_MASTER2_PUBLIC_YPATH_MASTER_H_
#define UTIL_YPATH_MASTER2_PUBLIC_YPATH_MASTER_H_

#include <map>
#include <vector>
#include <string>

#include "base/public/hash_tables.h"
#include "base/public/callback.h"
#include "base/public/scoped_ptr.h"
#include "third_part/parser_master/public/parse_master.h"
#include "callback_define.h"

namespace re2 {
class RE2;
}

namespace path_master {
class PathItem;
class Parser;
class PathTag;

class PathMaster {
 public:
  PathMaster();
  ~PathMaster();

  void Init(parse_master::ParseMaster* pm);

  //  If you want to use a regular expression in xptah, you can write a tag
  //  like this:
  //  <tagname keyname="reg(xxxx)">
  //  YPathMaster will extract xxxx as regular expression for value of keyname
  //  If attribute value of keyname match regx, callball will be called.
  //  for exmaple , there are two tags in html:
  //  <a href="/user_profile/1234.html">
  //  <a href="/user_profile/1235.html">
  //  you can use xpath : <a href="reg(/user_profile/\d+\.html)"> for match two
  //  tags.
  bool RegistParser(const std::string& parser,
                    const std::string& xpath,
                    base::ResultCallback1<bool, ParserParam*>* callback);

  //  Tag name and arrtibute of xpath is not case sensitive
  //  You can use this function to regist a path, and bind a object
  //  with this callback, so when callback is called by YPathMaster,
  //  you can get bind_info by YParserParam
  bool RegistParser(const std::string& parser,
                    const std::string& xpath,
                    base::ResultCallback1<bool, ParserParam*>* callback,
                    CallBackBindInfo* bind_info);

  void UnregistAllParsers();

  //  If force_check_attrs is not empty, If xpath Does NOT contain this
  //  tag attribute, a path with this attribute won't be matched!
  //  For example force_check_attrs contains "id" and "class",
  //  if a xpath is "<div><div id="xx">"
  //  Below cases won't be matched:
  //  <div id="yy"><div id="xx">
  //  <div class="yy"><div id="xx">
  //  <div id="yy" class="tt"><div id="xx">
  //  <div><div id="xx" class="tt">
  //
  //  But "<div font="xx"><div id="xx" onclick="xx">" or "<div><div id="xx">"
  //  will be matched
  void SetForceCheckAttrs(const std::string& parser,
                          std::vector<std::string>& force_check_attrs);
  
  //  You MUST call Reset when a url is not matched by any parser for
  //  ypath_master, if not , ypath_master will try to use xpath set of
  //  previous parser_name to match domtree of next documents.And if a xpath
  //  is found, callback of PREVIOUS parser will be called.
  //  If you DO really not want to call it, please make sure that
  //  UnRegistCallback is called.
  void Reset();
  void ResetCurrentParser(std::string parser_name);

  void RegistCallback();
  void UnRegistCallback();

 private:
  void RunCallbackOnCloseTag(const parse_master::DomNode&);
  bool MatchPath(PathItem* item,
                 const parse_master::DomNode& node,
                 const std::vector<std::string>& force_check_attrs);
  Parser* FindParser(const std::string& parser_name);
  //  We put this function in class YPathMaster rather than in
  //  class YPathTag, since you will hold all re2 objects in a
  //  YPathMaster instance
  bool MatchTag(const PathTag& tag,
                const parse_master::DomNode* node,
                const std::vector<std::string>& force_check_attrs);
  re2::RE2* GetRe2Object(const std::string& regx);

 private:
  parse_master::ParseMaster* parse_master_;
  std::map<std::string, Parser*> parsers_;
  Parser* current_parser_;

  base::Callback1<const parse_master::DomNode&>* callback_;
  bool callback_unregisted_;
  base::hash_map<std::string, re2::RE2*> regx_re2_map_;
  DISALLOW_COPY_AND_ASSIGN(PathMaster);
};
}
#endif  // UTIL_YPATH_MASTER2_PUBLIC_YPATH_MASTER_H_
