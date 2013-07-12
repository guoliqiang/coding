// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yusun@yunrang.com (Sun Yu)

#include "base/public/hash.h"
#include "base/public/string_util.h"
#include "../public/base64encoder.h"
#include "../../crypto/public/base64.h"

namespace util {

REGISTER_ENCODERBASE(Base64Encoder);

void Base64Encoder::CaesarEncrypt(std::string* buf) {
  char c;
  for (int i = 0; i < buf->size(); ++i) {
    c = (*buf)[i];
    if (c >= 'A' && c <= 'Z') {
      (*buf)[i] = caesar_enstr_uc_[c - 'A'];
    } else if (c >= 'a' && c <= 'z') {
      (*buf)[i] = caesar_enstr_lc_[c - 'a'];
    }
  }
}

void Base64Encoder::CaesarDecrypt(std::string* buf) {
  char c;
  for (int i = 0; i < buf->size(); ++i) {
    c = (*buf)[i];
    if (c >= 'A' && c <= 'Z') {
      (*buf)[i] = caesar_destr_uc_[c - 'A'];
    } else if (c >= 'a' && c <= 'z') {
      (*buf)[i] = caesar_destr_lc_[c - 'a'];
    }
  }
}

void Base64Encoder::SetEncodeKey(const std::string& key) {
  CHECK(!key.empty());
  key_.assign(key);

  // Make Caesar code by key
  caesar_enstr_lc_ = "abcdefghijklmnopqrstuvwxyz";
  int inds = 0;
  int indt = 0;
  for (int i = 0; i < key.size(); i++) {
    inds = i % 26;
    indt = inds + key[i] % (26 - inds);
    caesar_enstr_lc_ =
        caesar_enstr_lc_.substr(0, inds) +
        caesar_enstr_lc_.substr(indt) +
        caesar_enstr_lc_.substr(inds, indt - inds);
  }
  caesar_enstr_uc_.assign(caesar_enstr_lc_);
  StringToUpperASCII(&caesar_enstr_uc_);

  caesar_destr_uc_.resize(26);
  caesar_destr_lc_.resize(26);
  char c;
  for (int i = 0; i < 26; i++) {
    c = caesar_enstr_uc_[i];
    caesar_destr_uc_[c - 'A'] = static_cast<char>('A' + i);
    c = caesar_enstr_lc_[i];
    caesar_destr_lc_[c - 'a'] = static_cast<char>('a' + i);
  }
}

void Base64Encoder::GetEncodeKey(std::string* key) const {
  key->assign(key_);
}

bool Base64Encoder::Encode(const std::string& src, std::string* des) {
  std::string buf;
  if (crypto::Base64Encode(src, &buf)) {
    if (!caesar_enstr_uc_.empty()) {
      CaesarEncrypt(&buf);
    }
    des->assign(buf);
    return true;
  }
  return false;
}

bool Base64Encoder::Decode(const std::string& src, std::string* des) {
  std::string buf(src);
  if (!caesar_destr_uc_.empty()) {
    CaesarDecrypt(&buf);
  }
  return crypto::Base64Decode(buf, des);
}

}  // namespace util
