// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 16:51:13
// File  : cache_unittest.cc
// Brief :
#include "../../public/cache.h"

int main(int argc, char** argv) {
  nltk::svm::Cache cache(10, 20);
  base::shared_ptr<nltk::svm::CacheNode> data(NULL);

  LOG(INFO) << cache.GetData(1, 10, &data);
  LOG(INFO) << data.get()->data;
  LOG(INFO) << cache.GetData(1, 5, &data);
  LOG(INFO) << data.get()->data;
  LOG(INFO) << cache.GetData(1, 20, &data);
  LOG(INFO) << data.get()->data;
  LOG(INFO) << cache.GetData(2, 10, &data);
  LOG(INFO) << data.get()->data;
  
  return 0;
}
