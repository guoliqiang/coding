// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: jianxu@yunrang.com (Jian Xu)
//
// base62 is used to shorten the url of the microblogging.
// When a microblogging released, its original url is stored in
// the database while the database will generate an id for it,
// By encoding the id, we can get the encoded string, this string
// will be used as part of the url of microblogging.

#ifndef UTIL_CRYPTO_BASE62_H__
#define UTIL_CRYPTO_BASE62_H__

#include <string>

#include "base/public/basictypes.h"

namespace crypto {

// Encodes the input string in base62.  Returns true if successful and false
// otherwise.  The output string is only modified if successful.
bool Base62Encode(int64 input, std::string* output);

std::string Base62Encode(int64 input);

// Decodes the base62 input string.  Returns true if successful and false
// otherwise.  The output string is only modified if successful.
bool Base62Decode(const std::string& input, int64 *output);

int64 Base62Decode(const std::string& input);

}  // namespace crypto

#endif  // UTIL_CRYPTO_BASE62_H__
