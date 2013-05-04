// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include "../public/registerer.h"

namespace registerer {

BaseClassMap& global_factory_map() {
  static BaseClassMap *factory_map = new BaseClassMap();
  return *factory_map;
}

}  // namespace registerer
