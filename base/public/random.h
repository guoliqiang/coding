// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2014-11-19 05:22:05
// File  : random.h
// Brief :

#ifndef  __RANDOM_H_
#define  __RANDOM_H_

#include <stdlib.h>
#include "base/public/time.h"
#include "base/public/basictypes.h"

namespace base {
inline uint32_t Random(uint32_t size = RAND_MAX) {
  srand(GetTimeInUsec());
  return rand() % size;
}                                                                                   
                                                                                    
inline float Random(float min, float max, uint32_t base_size = RAND_MAX) {
  float v = Random(base_size);
  return min + (v / float(base_size)) * (max - min);
}

class IRandom {
 private:
  uint32_t seed_;
 public:
  explicit IRandom(uint32_t s) : seed_(s & 0x7fffffffu) { }
  uint32_t Next() {
    static const uint32_t M = 2147483647L;   // 2^31-1
    static const uint64_t A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
    // We are computing
    //       seed_ = (seed_ * A) % M,    where M = 2^31-1
    //
    // seed_ must not be zero or M, or else all subsequent computed values
    // will be zero or M respectively.  For all other values, seed_ will end
    // up cycling through every number in [1,M-1]
    uint64_t product = seed_ * A;

    // Compute (product % M) using the fact that ((x << 31) % M) == x.
    seed_ = static_cast<uint32_t>((product >> 31) + (product & M));
    // The first reduction may overflow by 1 bit, so we may need to
    // repeat.  mod == M is not possible; using > allows the faster
    // sign-bit-based test.
    if (seed_ > M) {
      seed_ -= M;
    }
    return seed_;
  }
  // Returns a uniformly distributed value in the range [0..n-1]
  // REQUIRES: n > 0
  uint32_t Uniform(int n) {
    return Next() % n;
  }

  // Randomly returns true ~"1/n" of the time, and false otherwise.
  // REQUIRES: n > 0
  bool OneIn(int n) {
    return (Next() % n) == 0;
  }

  // Skewed: pick "base" uniformly from range [0,max_log] and then
  // return "base" random bits.  The effect is to pick a number in the
  // range [0,2^max_log-1] with exponential bias towards smaller numbers.
  uint32_t Skewed(int max_log) {
    return Uniform(1 << Uniform(max_log + 1));
  }
};
};  // namespace base

#endif  // __RANDOM_H_
