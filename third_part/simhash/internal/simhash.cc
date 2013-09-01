// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (wangbin)

#include "../public/simhash.h"
#include "base/public/hash.h"
#include "base/public/flags.h"
#include "base/public/hash_tables.h"


using namespace base;  // NOLINT
using namespace std;  // NOLINT

namespace util {
namespace simhash {
struct Hash128 {
  uint64 hash[2];
};

bool FingerprintInternal(const vector<pair<string, int> >& eigenvectors,
                         uint64* fingerprint,
                         bool use_bigram) {
  hash_map<string, uint64> fingerprint_map;
  int len = eigenvectors.size();
  if (len == 0) {
    return false;
  }
  int vint[64] = {0};
  uint64 one = 1;
  uint64 fingerprint_each_word = 0;
  string bigram;
  for (int i = 0; i < len; i++) {
    bigram.assign(eigenvectors[i].first);
    if (use_bigram && i + 1 < len) {
      bigram.append(eigenvectors[i + 1].first);
    }
    if (fingerprint_map.find(bigram) != fingerprint_map.end()) {
      fingerprint_each_word = fingerprint_map[bigram];
    } else {
      fingerprint_each_word = base::Fingerprint(bigram);
      fingerprint_map.insert(make_pair(bigram, fingerprint_each_word));
    }
    for (int j = 0; j < 64; j++) {
      if (fingerprint_each_word & (one << j)) {
        vint[j] += eigenvectors[i].second;
      } else {
        vint[j] -= eigenvectors[i].second;
      }
    }
  }
  *fingerprint = 0;
  for (int i = 0; i < 64; i++) {
    if (vint[i] > 0) {
      *fingerprint |= (one << i);
    }
  }
  return true;
}

bool Fingerprint128Internal(const std::vector<std::pair<std::string, int> >&
                            eigenvectors, uint64* fingerprint,
                            bool use_bigram) {
  hash_map<string, Hash128> fingerprint_map;
  int len = eigenvectors.size();
  if (len == 0) {
    return false;
  }
  int vint[128] = {0};
  uint64 one = 1;
  Hash128 fingerprint_each_word;
  fingerprint_each_word.hash[0] = 0;
  fingerprint_each_word.hash[1] = 0;
  string bigram;
  for (int i = 0; i < len; i++) {
    bigram.assign(eigenvectors[i].first);
    if (use_bigram && i + 1 < len) {
      bigram.append(eigenvectors[i + 1].first);
    }
    if (fingerprint_map.find(bigram) != fingerprint_map.end()) {
      fingerprint_each_word = fingerprint_map[bigram];
    } else {
      base::FNV128(bigram.data(), bigram.length(), fingerprint_each_word.hash);
      fingerprint_map.insert(make_pair(bigram, fingerprint_each_word));
    }
    uint64 tmp = 0;
    for (int j = 0; j < 128; j++) {
      if (j < 64) {
        tmp = fingerprint_each_word.hash[0] & (one << j);
      } else {
        tmp = fingerprint_each_word.hash[1] & (one << (j - 64));
      }
      if (tmp) {
        vint[j] += eigenvectors[i].second;
      } else {
        vint[j] -= eigenvectors[i].second;
      }
    }
  }
  fingerprint[0] = 0;
  fingerprint[1] = 0;
  for (int i = 0; i < 128; i++) {
    if (vint[i] > 0) {
      if (i < 64) {
        fingerprint[0] |= (one << i);
      } else {
        fingerprint[1] |= (one << (i - 64));
      }
    }
  }
  return true;
}

bool Fingerprint(const vector<pair<string, int> >& eigenvectors,
                 uint64* fingerprint) {
  return FingerprintInternal(eigenvectors, fingerprint, false);
}

bool FingerprintUsingBigram(const vector<pair<string, int> >& eigenvectors,
                         uint64* fingerprint) {
  return FingerprintInternal(eigenvectors, fingerprint, true);
}

bool Fingerprint128(const std::vector<std::pair<std::string, int> >&
                    eigenvectors, uint64* fingerprint) {
  return Fingerprint128Internal(eigenvectors, fingerprint, false);
}

bool Fingerprint128UsingBigram(const std::vector<std::pair<std::string, int> >&
                               eigenvectors, uint64* fingerprint) {
  return Fingerprint128Internal(eigenvectors, fingerprint, true);
}

int32 HammingDistance(const uint64_t & a, const uint64_t &b) {
  uint64 c = a^b;
  int counter = 0;
  while (c) {
    c &= c - 1;
    ++counter;
  }
  return counter;
}

// 这个函数还可以优化
int HammingDistanceFast(const uint64_t & simhash_value1,
                        const uint64_t & simhash_value2) {
  uint64_t count = simhash_value1 ^ simhash_value2;
  uint64_t a = 0x5555555555555555;
  uint64_t b = 0x3333333333333333;
  uint64_t c = 0x0f0f0f0f0f0f0f0f;
  uint64_t d = 0x00ff00ff00ff00ff;
  uint64_t e = 0x0000ffff0000ffff;
  uint64_t f = 0x00000000ffffffff;

  count = (count & a) + ((count>>1) & a);
  count = (count & b) + ((count>>2) & b);
  count = (count & c) + ((count>>4) & c);
  count = (count & d) + ((count>>8) & d);
  count = (count & e) + ((count>>16) & e);
  count = (count & f) + ((count>>32) & f);
  return static_cast<int>(count);
}

}  // namespace simhash
}  // namespace util
