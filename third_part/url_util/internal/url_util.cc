// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yubogong@yunrang.com (Yu BO Gong)

#include "third_part/url_util/public/url_util.h"

#include <cstdio>
#include <string>
#include <algorithm>

#include "base/public/string_util.h"
#include "base/public/logging.h"

namespace url_util {

void ParseKvlist(const std::string& line,
                 const std::string& key_value_delimiter,
                 char key_value_pair_delimiter,
                 std::map<std::string, std::string> *kv_pair_map,
                 std::vector<std::pair<std::string, std::string> >*
                 kv_pair_vec) {
  std::vector<std::string> pairs;
  std::vector<std::string> kvpair;
  SplitString(line, key_value_pair_delimiter, &pairs);
  VLOG(10) << "pairs num:" << pairs.size();
  for (int i = 0; i < pairs.size(); ++i) {
    kvpair.clear();
    SplitStringUsingSubstr(pairs[i], key_value_delimiter, &kvpair);
    if (kvpair.size() != 2) {
      continue;
    }
    VLOG(10) << "[" << kvpair[0] << "]:[" << kvpair[1] << "]";
    if (kv_pair_map) {
      kv_pair_map->insert(make_pair(kvpair[0], kvpair[1]));
    }
    if (kv_pair_vec) {
      kv_pair_vec->push_back(make_pair(kvpair[0], kvpair[1]));
    }
  }
}
}  // url_util
