// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include "../public/parser_holder.h"

#include "base/public/hash_tables.h"
#include "third_part/gtl/public/stl_util-inl.h"

using base::Callback1;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace parse_master {

Parsers::Parsers() : active_(true) {
  tag_open_parsers_ =
      new vector<Callback1<const DomNode&>*>[HtmlTag::GetTagsCount()];
  tag_close_parsers_ =
      new vector<Callback1<const DomNode&>*>[HtmlTag::GetTagsCount()];
};

Parsers::~Parsers() {
  Clear();
  delete[] tag_open_parsers_;
  delete[] tag_close_parsers_;
};
}
