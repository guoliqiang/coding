// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#include "util/i18n/ip_to_location.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <algorithm>
#include "base/yr.h"
#include "base/string_util.h"
#include "file/file.h"
#include "util/release/release_path.h"

DEFINE_string(ip_list, "util/i18n/ip_range_list.txt", "");

namespace i18n {
bool SortByStartIP(const IPMapping& left, const IPMapping& right) {
  return left.start < right.start;
}

void IPToLocation::LoadIPListFile() {
  if (FLAGS_ip_list.empty()) {
    LOG(WARNING) << "empty ip range list";
    return;
  }

  string path = release::GetReleaseYrDataPath(FLAGS_ip_list);
  string content;
  CHECK(file::File::ReadFileToString(path, &content))
      << "load file failed:" << path;
  LOG(INFO) << "load file:" << path;

  vector<string> lines;
  SplitString(content, '\n', &lines);
  size_t line_count = lines.size();

  for (size_t i = 0; i < line_count; ++i) {
    const string& line = lines[i];
    if (line.empty() || line[0] == '#') {
      continue;
    }

    vector<string> segs;
    SplitString(line, ':', &segs);
    CHECK_EQ(segs.size(), 7);

    ips_.push_back(IPMapping(
        ConvertIPToUInt64(segs[0]),
        ConvertIPToUInt64(segs[1]),
        StringToRegionCode(segs[3])));
  }
  sort(ips_.begin(), ips_.end(), SortByStartIP);
}

Region IPToLocation::Lookup(int32 ip) {
  struct in_addr ip_addr = *reinterpret_cast<struct in_addr*>(&ip);
  return Lookup(inet_ntoa(ip_addr));
}

Region IPToLocation::Lookup(const string& ip) {
  uint64 ip_value = ConvertIPToUInt64(ip);
  int begin = 0;
  int end = ips_.size();
  while (begin <= end) {
    int index = (begin + end) / 2;
    const IPMapping& ip_range = ips_[index];
    if (ip_value > ip_range.start) {
      if (ip_value <= ip_range.end) {
        return ip_range.region;
      }

      if (index+1 >= ips_.size() ||
          (index+1 < ips_.size() && ip_value < ips_[index+1].start)) {
        return UNKNOWNAREA;
      } else {
        begin = index + 1;
      }
    } else if (ip_value < ip_range.start) {
      end = index - 1;
    } else {
      return ip_range.region;
    }
  }
  return UNKNOWNAREA;
}

uint64 IPToLocation::ConvertIPToUInt64(const std::string& ip) {
  vector<string> segs;
  SplitString(ip, '.', &segs);
  CHECK_EQ(segs.size(), 4);

  uint64 value = 0;
  for (size_t i = 0; i < segs.size() ; ++i) {
    value = value * 256 + StringToInt(segs[i]);
  }
  return value;
}

Region IPToLocation::StringToRegionCode(const std::string& region) {
#define StringToRegionCode(region_str, region_code) \
  if (region == region_str) {  \
    return region_code;         \
  }

  StringToRegionCode("af", AFGHANISTAN);
  StringToRegionCode("ap", AFRICANREGIONALINDUSTRIALPROPERTYORGANIZATION);
  StringToRegionCode("as", AMERICANSAMOA);
  StringToRegionCode("au", AUSTRALIA);
  StringToRegionCode("bd", BANGLADESH);
  StringToRegionCode("bn", BRUNEIDARUSSALAM);
  StringToRegionCode("bt", BHUTAN);
  StringToRegionCode("ck", COOKISLANDS);
  StringToRegionCode("cn", CHINA);
  StringToRegionCode("fj", FIJI);
  StringToRegionCode("fm", FEDERATEDSSTATESOFMICRONESIA);
  StringToRegionCode("gb", UNITEDKINGDOM);
  StringToRegionCode("gu", GUAM);
  StringToRegionCode("hk", HONGKONG);
  StringToRegionCode("id", INDONESIA);
  StringToRegionCode("in", INDIA);
  StringToRegionCode("io", BRITISHINDIANOCEANTERRITORY);
  StringToRegionCode("jp", JAPAN);
  StringToRegionCode("kh", CAMBODIA);
  StringToRegionCode("ki", KIRIBATI);
  StringToRegionCode("kp", DEMOCRATICPEOPLESREPUBLICOFKOREA);
  StringToRegionCode("kr", REPUBLICOFKOREA);
  StringToRegionCode("la", LAOPEOPLESDEMOCRATICREPUBLIC);
  StringToRegionCode("lk", SRILANKA);
  StringToRegionCode("mh", MARSHALLISLANDS);
  StringToRegionCode("mm", MYANMAR);
  StringToRegionCode("mn", MONGOLIA);
  StringToRegionCode("mo", MACAO);
  StringToRegionCode("mp", NORTHERNMARIANAISLANDS);
  StringToRegionCode("mu", MAURITIUS);
  StringToRegionCode("mv", MALDIVES);
  StringToRegionCode("my", MALAYSIA);
  StringToRegionCode("nc", NEWCALEDONIA);
  StringToRegionCode("nf", NORFOLKISLAND);
  StringToRegionCode("np", NEPAL);
  StringToRegionCode("nr", NAURU);
  StringToRegionCode("nu", NIUE);
  StringToRegionCode("nz", NEWZEALAND);
  StringToRegionCode("pf", FRENCHPOLYNESIA);
  StringToRegionCode("pg", PAGUANEWGUINEA);
  StringToRegionCode("ph", PHILIPPINES);
  StringToRegionCode("pk", PAKISTAN);
  StringToRegionCode("pw", PALAU);
  StringToRegionCode("sb", SOLOMONISLANDS);
  StringToRegionCode("sg", SINGAPORE);
  StringToRegionCode("th", THAILAND);
  StringToRegionCode("tk", TOKELAU);
  StringToRegionCode("tl", TIMORLESTE);
  StringToRegionCode("to", TONGA);
  StringToRegionCode("tv", TUVALU);
  StringToRegionCode("tw", TAIWAN);
  StringToRegionCode("us", UNITEDSTATES);
  StringToRegionCode("vn", VIETNAM);
  StringToRegionCode("vu", VANUATU);
  StringToRegionCode("wf", WALLISANDFUTUNA);
  StringToRegionCode("ws", SAMOA);

  return UNKNOWNAREA;
}
}  // namespace i18n

