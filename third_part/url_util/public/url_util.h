// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yubogong@yunrang.com (Yu Bo Gong)

#ifndef  WEB_UTIL_URL_UTIL_URL_UTIL_H_
#define  WEB_UTIL_URL_UTIL_URL_UTIL_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/public/singleton.h"
#include "base/public/logging.h"

namespace url_util {

void ParseKvlist(const std::string &line,
                 const std::string &key_value_delimiter,
                 char key_value_pair_delimiter,
                 std::map<std::string, std::string> *kv_pair_map,
                 std::vector<std::pair<std::string, std::string> > *kv_pair_vec);

}  // url_util

#endif  // WEB_UTIL_URL_UTIL_URL_UTIL_H_
