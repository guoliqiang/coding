// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-27 21:16:48
// File  : raw_merge_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/multi_index/public/multiindexdb.h"

namespace multi_index {

TEST(RwaMerge, Serialize) {
  multiindex<int, int, int> db;
  db.insert(multiindex<int, int, int>::entry_type(1, 1, 1));
  db.insert(multiindex<int, int, int>::entry_type(1, 3, 3));
  db.insert(multiindex<int, int, int>::entry_type(2, 4, 4));
  db.keySet(1, 10);
  for (multiindex<int, int, int>::key_iterator i = db.begin(); i != db.end();
       i++) {
    LOG(INFO) << i->key << " " << i->index << " " << " " << i->value;
  }
  db.keyErase(1);
  for (multiindex<int, int, int>::key_iterator i = db.begin(); i != db.end();
       i++) {
    LOG(INFO) << i->key << " " << i->index << " " << " " << i->value;
  }
}
}  // namespace multiindexdb
