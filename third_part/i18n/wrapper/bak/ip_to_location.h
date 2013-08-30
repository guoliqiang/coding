// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#ifndef UTIL_I18N_IP_TO_LOCATION_H_
#define UTIL_I18N_IP_TO_LOCATION_H_

#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/hash_tables.h"
#include "util/i18n/region.h"

namespace i18n {
struct IPMapping {
  uint64 start;
  uint64 end;
  Region region;

  IPMapping(uint64 ip_start, uint64 ip_end, Region region_code)
    : start(ip_start), end(ip_end), region(region_code) {}
};

class IPToLocation {
 public:
  IPToLocation() {
    LoadIPListFile();
  }

  ~IPToLocation() {}

  Region Lookup(const std::string& ip);
  Region Lookup(int32 ip);

 private:
  Region StringToRegionCode(const std::string& region);
  uint64 ConvertIPToUInt64(const std::string& ip);
  void LoadIPListFile();

  std::vector<IPMapping> ips_;
  DISALLOW_COPY_AND_ASSIGN(IPToLocation);
};  // class IPToLocation
}  // namespace i18n

#endif  // UTIL_I18N_IP_TO_LOCATION_H_

