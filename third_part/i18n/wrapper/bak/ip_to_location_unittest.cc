// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com (Lei Lu)

#include <arpa/inet.h>
#include "util/i18n/ip_to_location.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "base/yr.h"
#include "base/logging.h"

TEST(IPToLocation, Lookup) {
  i18n::IPToLocation looker;
  EXPECT_EQ(i18n::UNKNOWNAREA, looker.Lookup("0.0.0.0"));
  EXPECT_EQ(i18n::UNKNOWNAREA, looker.Lookup(inet_addr("0.0.0.0")));
  EXPECT_EQ(i18n::CHINA, looker.Lookup("1.0.1.0"));
  EXPECT_EQ(i18n::CHINA, looker.Lookup(inet_addr("1.0.1.0")));
  EXPECT_EQ(i18n::CHINA, looker.Lookup("1.0.1.118"));
  EXPECT_EQ(i18n::CHINA, looker.Lookup(inet_addr("1.0.1.118")));
  EXPECT_EQ(i18n::CHINA, looker.Lookup("1.0.1.255"));
  EXPECT_EQ(i18n::CHINA, looker.Lookup(inet_addr("1.0.1.255")));
  EXPECT_EQ(i18n::CHINA, looker.Lookup("116.255.137.85"));
  EXPECT_EQ(i18n::CHINA, looker.Lookup(inet_addr("116.255.137.85")));
  EXPECT_EQ(i18n::HONGKONG, looker.Lookup("1.32.1.2"));
  EXPECT_EQ(i18n::HONGKONG, looker.Lookup(inet_addr("1.32.1.2")));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup("61.248.0.0"));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup(inet_addr("61.248.0.0")));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup("61.255.255.255"));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup(inet_addr("61.255.255.255")));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup("61.255.3.4"));
  EXPECT_EQ(i18n::REPUBLICOFKOREA, looker.Lookup(inet_addr("61.255.3.4")));
  EXPECT_EQ(i18n::UNKNOWNAREA, looker.Lookup("255.255.255.255"));
  EXPECT_EQ(i18n::UNKNOWNAREA, looker.Lookup(inet_addr("255.255.255.255")));
}
