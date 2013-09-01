// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (wangbin)

#ifndef UTIL_SIMHASH_SIMHASH_H_
#define UTIL_SIMHASH_SIMHASH_H_

#include <vector>
#include <string>
#include <utility>

#include "base/public/basictypes.h"

namespace util {
namespace simhash {
  bool Fingerprint(const std::vector<std::pair<std::string, int> >&
                   eigenvectors, uint64* fingerprint);

  bool FingerprintUsingBigram(const std::vector<std::pair<std::string, int> >&
                              eigenvectors, uint64* fingerprint);

  bool Fingerprint128(const std::vector<std::pair<std::string, int> >&
                      eigenvectors, uint64* fingerprint);

  bool Fingerprint128UsingBigram(const std::vector<std::pair<std::string, int> >&
                                 eigenvectors, uint64* fingerprint);

  int32 HammingDistance(const uint64_t &, const uint64_t &);
  int32 HammingDistanceFast(const uint64_t &, const uint64_t &);
}  // namespace simhash
}  // namespace util

#endif  // UTIL_SIMHASH_SIMHASH_H_
