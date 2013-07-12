// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#include <iostream>
#include "base/scoped_ptr.h"
#include "util/encode/encoderbase.h"
#include "util/crypto/base64.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(EncodeTest, Basic) {
  std::string src("See you tomorrow,\n hello world,\r\n 哈哈");
  std::string encoded, decoded;
  string key;

  scoped_ptr<util::EncoderBase> coder;

  coder.reset(
      util::EncoderBaseRegisterer::GetInstanceByName("ConverseEncoder"));
  coder->Encode(src, &encoded);
  coder->Decode(encoded, &decoded);
  std::cout << "ConverseEncoder: " << encoded << std::endl;
  std::cout << "ConverseEncoder: " << decoded << std::endl;
  CHECK_EQ(src, decoded);

  coder.reset(
      util::EncoderBaseRegisterer::GetInstanceByName("Base64Encoder"));
    coder->GetEncodeKey(&key);
  std::cout << "When no define encode key. Encode key:" << key << std::endl;
  coder->Encode(src, &encoded);
  coder->Decode(encoded, &decoded);
  std::cout << "Base64Encoder: " << encoded << std::endl;
  std::cout << "Base64Encoder: " << decoded << std::endl;
  CHECK_EQ(src, decoded);

  coder->SetEncodeKey("test key");
  coder->GetEncodeKey(&key);
  std::cout << "Encode key:" << key << std::endl;
  coder->Encode(src, &encoded);
  coder->Decode(encoded, &decoded);
  std::cout << "Base64Encoder(Caesar): " << encoded << std::endl;
  std::cout << "Base64Encoder(Caesar): " << decoded << std::endl;
  CHECK_EQ(src, decoded);
}

