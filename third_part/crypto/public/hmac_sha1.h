/******************************************************************************
*  100% free public domain implementation of the HMAC-SHA1 algorithm
*  by Chien-Chung, Chung (Jim Chung) <jimchung1221@gmail.com>
*  Copyright [2010] <jimchung1221@gmail.com>
******************************************************************************/
#ifndef UTIL_CRYPTO_HMAC_SHA1_H_
#define UTIL_CRYPTO_HMAC_SHA1_H_

#include "sha1.h"

namespace util_oauth {

class ChmacSha1: public CSha1 {
 public:

  enum {
    SHA1_DIGEST_LENGTH = 20,
    SHA1_BLOCK_SIZE = 64,
    HMAC_BUF_LEN = 4096
  };

  ChmacSha1() {
    report_ = new char[HMAC_BUF_LEN];
    append_buf1_ = new char[HMAC_BUF_LEN];
    append_buf2_ = new char[HMAC_BUF_LEN];
    sha1_key_ = new char[HMAC_BUF_LEN];
  }
  ~ChmacSha1() {
    delete[] report_;
    delete[] append_buf1_;
    delete[] append_buf2_;
    delete[] sha1_key_;
  }
  void GetHmacEncryption(char *text,
                         int text_len,
                         char *key,
                         int key_len,
                         char *digest);

 private:
  char ipad_[64];
  char opad_[64];

  char *report_;
  char *sha1_key_;
  char *append_buf1_;
  char *append_buf2_;
};
}
#endif  // UTIL_CRYPTO_HMAC_SHA1_H_
