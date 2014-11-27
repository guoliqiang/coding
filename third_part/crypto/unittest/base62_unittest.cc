// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Jian Xu)

#include "third_part/crypto/public/base62.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

TEST(Base64Test, Basic) {
  const std::string kBase62Str1 = "4EV8";
  int64 kBase62Int1 = 1110614;
  const std::string kBase62Str2 = "5aqp";
  int64 kBase62Int2 = 1231717;

  std::string encoded;
  int64 decoded;

  crypto::Base62Encode(kBase62Int1);
  EXPECT_TRUE(crypto::Base62Encode(kBase62Int1, &encoded));
  EXPECT_EQ(kBase62Str1, encoded);
  encoded.clear();

  EXPECT_TRUE(crypto::Base62Encode(kBase62Int2, &encoded));
  EXPECT_EQ(kBase62Str2, encoded);
  encoded.clear();


  EXPECT_TRUE(crypto::Base62Decode(kBase62Str1, &decoded));
  EXPECT_EQ(kBase62Int1, decoded);

  EXPECT_TRUE(crypto::Base62Decode(kBase62Str2, &decoded));
  EXPECT_EQ(kBase62Int2, decoded);
}
