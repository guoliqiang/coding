// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-03-26 01:28:43
// File  : profile_analyzer.h
// Brief :

#ifndef  __PROFILE_ANALYZER_H_
#define  __PROFILE_ANALYZER_H_

#include <map>
#include "base/public/logging.h"
#include "base/public/hash_tables.h"

class ProfileAnalyzer {
 public:
  void Build();
  void SimpleList(std::map<std::string, int> * rs);

 private:
  // grap_[key][hash_map]: han_map call key
  base::hash_map<std::string, base::hash_map<std::string, int> > grap_;
  // reverse_grap_[key][hash_map]: key call hash_map
  base::hash_map<std::string, base::hash_map<std::string, int> > reverse_grap_;
  base::hash_map<uint64_t, std::string> symbol_;
  base::hash_map<std::string, int> hit_num_;
};

#endif  //__PROFILE_ANALYZER_H_
