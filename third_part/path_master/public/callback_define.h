// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_YPATH_MASTER2_PUBLIC_CALLBACK_DEFINE_H_
#define UTIL_YPATH_MASTER2_PUBLIC_CALLBACK_DEFINE_H_

#include <string>

#include "base/public/callback.h"
#include "third_part/parser_master/public/parse_master.h"

namespace path_master {

//  All parser who want to bind some info with a callback,
//  it must wrap bind info with a subclass of CallBackBindInfo
class CallBackBindInfo {
 public:
  CallBackBindInfo() {}
  virtual ~CallBackBindInfo() {}
 private:
  DISALLOW_COPY_AND_ASSIGN(CallBackBindInfo);
};

class ParserParam {
 public:
  ParserParam(parse_master::DomNode* node,
               const std::string* path,
               CallBackBindInfo* bind = NULL): node_(node),
                                               path_(path),
                                               bind_info_(bind) {}
  ~ParserParam() {}

  parse_master::DomNode* node() { return node_; }
  const std::string* path() const { return path_; }
  CallBackBindInfo* bind_info() {
    return bind_info_;
  }

 private:
  parse_master::DomNode* node_;
  const std::string* path_;
  CallBackBindInfo* bind_info_;
 
 private:
  DISALLOW_COPY_AND_ASSIGN(ParserParam);
};

typedef base::ResultCallback1<bool, ParserParam*>* PathCallback;
}

#endif  // UTIL_YPATH_MASTER2_PUBLIC_CALLBACK_DEFINE_H_
