// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "../public/parser.h"
#include "../public/path_item.h"

namespace path_master {

using parser_master::ParseMaster;
using parser_master::DomNode;

Parser::Parser() {}

Parser::~Parser() {
  gtl::STLDeleteElements(&paths_);
}

bool Parser::AddPath(const std::string& xpath,
                     PathCallback callback,
                     CallBackBindInfo* bind_info) {
  PathItem*  item = new PathItem(xpath, callback, bind_info);
  if (item->ParseXPath()) {
    paths_.push_back(item);
    return true;
  } else {
    delete item;
    return false;
  }
}
}
