// Copyright (c) 2012 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "third_part/bloom_filter/public/bloom.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"

namespace bloom_filter {

static std::string Key(int i, char * buffer) {
  memcpy(buffer, &i, sizeof(int32_t));
  return std::string(buffer, sizeof(int32_t));
}

class BloomTest : public testing::Test {
 private:
  const BloomFilter * policy_;
  std::string filter_;
  std::vector<std::string> keys_;

 public:
  BloomTest() : policy_(new BloomFilter(10)) { }

  ~BloomTest() {
    delete policy_;
  }

  void Reset() {
    keys_.clear();
    filter_.clear();
  }

  void Add(const std::string & s) {
    keys_.push_back(s);
  }

  void Build() {
    std::vector<std::string> key_slices;
    for (size_t i = 0; i < keys_.size(); i++) {
      key_slices.push_back(keys_[i]);
    }
    filter_.clear();
    policy_->CreateFilter(&key_slices[0], key_slices.size(), &filter_);
    keys_.clear();
  }

  size_t FilterSize() const {
    return filter_.size();
  }

  bool Matches(const std::string & s) {
    if (!keys_.empty()) {
      Build();
    }
    return policy_->KeyMayMatch(s, filter_);
  }

  double FalsePositiveRate() {
    char buffer[sizeof(int)];  // NOLINT
    int result = 0;
    for (int i = 0; i < 10000; i++) {
      if (Matches(Key(i + 1000000000, buffer))) {
        result++;
      }
    }
    return result / 10000.0;
  }
};

TEST_F(BloomTest, EmptyFilter) {
  ASSERT_TRUE(!Matches("hello"));
  ASSERT_TRUE(!Matches("world"));
}

TEST_F(BloomTest, Small) {
  Add("hello");
  Add("world");
  ASSERT_TRUE(Matches("hello"));
  ASSERT_TRUE(Matches("world"));
  ASSERT_TRUE(!Matches("x"));
  ASSERT_TRUE(!Matches("foo"));
}

static int NextLength(int length) {
  if (length < 10) {
    length += 1;
  } else if (length < 100) {
    length += 10;
  } else if (length < 1000) {
    length += 100;
  } else {
    length += 1000;
  }
  return length;
}

TEST_F(BloomTest, VaryingLengths) {
  char buffer[sizeof(int)];  // NOLINT
  // Count number of filters that significantly exceed the false positive rate
  int mediocre_filters = 0;
  int good_filters = 0;

  for (int length = 1; length <= 10000; length = NextLength(length)) {
    Reset();
    for (int i = 0; i < length; i++) {
      Add(Key(i, buffer));
    }
    Build();

    ASSERT_LE(FilterSize(), (length * 10 / 8) + 40) << length;

    // All added keys must match
    for (int i = 0; i < length; i++) {
      ASSERT_TRUE(Matches(Key(i, buffer)))
          << "Length " << length << "; key " << i;
    }

    // Check false positive rate
    double rate = FalsePositiveRate();
    LOG(INFO) << "False positives:" << rate * 100 << " length= " << length
              << " bytes = " << static_cast<int>(FilterSize());
    ASSERT_LE(rate, 0.02);   // Must not be over 2%
    if (rate > 0.0125) mediocre_filters++;  // Allowed, but not too often
    else
      good_filters++;
  }
  ASSERT_LE(mediocre_filters, good_filters / 5);
}

}  // namespace bloom_filter
