/******************************************************************************
* HMAC_SHA1.cpp : Implementation of HMAC SHA1 algorithm
*                 Comfort to RFC 2104
* Copyright [2010] <jimchung1221@gmail.com>
******************************************************************************/
#include <memory>
#include "../public/hmac_sha1.h"

namespace util_oauth {

void ChmacSha1::GetHmacEncryption(char *text,
                                  int text_len,
                                  char *key,
                                  int key_len,
                                  char *digest) {
  memset(sha1_key_, 0, SHA1_BLOCK_SIZE);

  /* repeated 64 times for values in ipad_ and opad_ */
  memset(ipad_, 0x36, sizeof(ipad_));
  memset(opad_, 0x5c, sizeof(opad_));

  /* STEP 1 */
  if (key_len > SHA1_BLOCK_SIZE) {
    CSha1::Reset();
    CSha1::Update(key, key_len);
    CSha1::Final();

    CSha1::GetHash(sha1_key_);
  } else {
    memcpy(sha1_key_, key, key_len);
  }

  /* STEP 2 */
  for (int i = 0; i < sizeof(ipad_); i++) {
    ipad_[i] ^= sha1_key_[i];
  }

  /* STEP 3 */
  memcpy(append_buf1_, ipad_, sizeof(ipad_));
  memcpy(append_buf1_ + sizeof(ipad_), text, text_len);

  /* STEP 4 */
  CSha1::Reset();
  CSha1::Update(append_buf1_, sizeof(ipad_) + text_len);
  CSha1::Final();

  CSha1::GetHash((report_));

  /* STEP 5 */
  for (int j = 0; j < sizeof(opad_); j++) {
    opad_[j] ^= sha1_key_[j];
  }

  /* STEP 6 */
  memcpy(append_buf2_, opad_, sizeof(opad_));
  memcpy(append_buf2_ + sizeof(opad_), report_, SHA1_DIGEST_LENGTH);

  /*STEP 7 */
  CSha1::Reset();
  CSha1::Update(append_buf2_, sizeof(opad_) + SHA1_DIGEST_LENGTH);
  CSha1::Final();

  CSha1::GetHash(digest);
}
}
