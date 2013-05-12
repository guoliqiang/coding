// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: james@yunrang.com (Jun Min Gao)

#include "util/double_array_trie/da_trie2.h"

#include <utility>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

#include "base/logging.h"
#include "base/yr.h"
#include "base/hash.h"
#include "base/utf_string_conversions.h"
#include "base/string_util.h"
#include "file/file.h"
#include "util/crypto/md5.h"

DEFINE_bool(da_trie2_enable_cache, false, "");
DEFINE_string(da_trie2_cache_dir, "/tmp/.yr_da_trie2_cache", "");

namespace util {
using file::File;
using crypto::MD5Context;
using crypto::MD5Digest;

static const char* kMagicHead = "\x01\xFA" "DARTS" "\x86\x69";
static const char* kMagicTail = "\xEA\x95" "DARTS" "\x81\xE9";

template <typename StringT>
void DATrieBasic2<StringT>::Save(const std::string& filename) const {
  string content(reinterpret_cast<const char*>(this->units_.data()),
                 this->units_.size() * sizeof(Unit));
  File::WriteStringToFileOrDie(kMagicHead, filename);
  File::AppendStringToFileOrDie(content, filename);
  File::AppendStringToFileOrDie(kMagicTail, filename);
}

template <typename StringT>
void DATrieBasic2<StringT>::Load(const std::string& filename) {
  string content;
  File::ReadFileToStringOrDie(filename, &content);
  CHECK(StartsWithASCII(content, kMagicHead, true)) << "Invalid file head!";
  CHECK(EndsWith(content, kMagicTail, true)) << "Invalid file tail!";
  const Unit* arr =
      reinterpret_cast<const Unit *>(content.data() + strlen(kMagicHead));
  this->array_size_ =
      (content.size() - strlen(kMagicHead) - strlen(kMagicTail)) /
      sizeof(DATrieBasic2<StringT>::Unit);
  this->units_.clear();
  this->units_.insert(this->units_.end(), arr, arr + this->array_size_);
  this->array_ = &this->units_[0];
}

template <typename StringT>
void DATrieBasic2<StringT>::CacheForFile(const std::string& filename) {
  trie_source_files_.push_back(filename);
}

template <typename StringT>
void DATrieBasic2<StringT>::BuildFrom(
  const base::hash_map<StringT, int> &map) {
  if (LoadFromCache()) return;
  // Sort first
  vector<std::pair<StringT, int> > vec;
  VLOG(2) << "Copy map...";
  vec.reserve(map.size());
  std::copy(map.begin(), map.end(), std::back_inserter(vec));
  VLOG(2) << "Sort trie entries...";
  std::sort(vec.begin(), vec.end());
  // Bug fix for 20120313: random file content crash searchroot and rewriter
  const int kMaxKeyLen = 512;
  for (size_t i = 0; i < vec.size(); ++i) {
    if (vec[i].first.size() > kMaxKeyLen) {
      LOG(WARNING) << "Too long key: " << vec[i].first.substr(0, kMaxKeyLen);
      vec.erase(vec.begin() + i);
      i--;
    }
  }
  BuildFromSorted(vec);
}

template <typename StringT>
void DATrieBasic2<StringT>::BuildFrom(const vector<StringT> &keys) {
  base::hash_map<StringT, int> map;
  for (typename vector<StringT>::const_iterator it = keys.begin();
      it < keys.end(); ++it) {
    map[*it] = 1;
  }
  BuildFrom(map);
}

template <typename StringT>
void DATrieBasic2<StringT>::BuildFromSorted(
    const vector<pair<StringT, int> >&pairs) {
  VLOG(2) << "Prepare to build trie...";
  // Split keys and values
  vector<const typename StringT::value_type*> keys(pairs.size());
  vector<size_t> key_lens(pairs.size());
  vector<int> values(pairs.size());
  size_t key_len_sum = 0;
  for (size_t i = 0; i < pairs.size(); i++) {
    keys[i] = pairs[i].first.c_str();
    key_lens[i] = pairs[i].first.length();
    key_len_sum += key_lens[i];
    values[i] = pairs[i].second;
  }  // end for

  VLOG(2) << "Build trie...";
  this->clear();
  size_t i = 0;
  while (pairs.size() > 0 && i < pairs.size() && pairs[i].first.empty()) {
    ++i;
  }
  if (i >= pairs.size()) {
    this->clear();
    VLOG(1) << "Da Trie data is empty!";
    return;
  }
  int ret = this->build(keys.size() - i, &keys[i], &key_lens[i], &values[i]);
  if (ret != 0) {
    this->clear();
    LOG(FATAL) << "Build Double Array failed! " << ret;
  }
  VLOG(1) << "DA Trie Key Size:" << pairs.size() << ", Key Total Length:" <<
      key_len_sum << ", Memory use: " << this->size() * 8 / 1024 << "K bytes.";

  if (FLAGS_da_trie2_enable_cache && !trie_source_files_.empty()) {
    string cache_content(reinterpret_cast<const char*>(this->array()),
                         this->size() * 8);
    File::WriteStringToFileOrDie(trie_source_md5_, cached_file_);
    File::AppendStringToFileOrDie(cache_content, cached_file_);
    VLOG(2) << "Write trie to cache: " << cached_file_;
  }
}

template <typename StringT>
bool DATrieBasic2<StringT>::LoadFromCache() {
  if (!FLAGS_da_trie2_enable_cache || trie_source_files_.empty()) return false;
  string cache_name = Uint64ToString(
      Fingerprint(JoinString(trie_source_files_, ' ')));
  cached_file_ = FLAGS_da_trie2_cache_dir + "/" + cache_name;

  MD5Context ctx;
  crypto::MD5Init(&ctx);
  for (size_t i = 0; i < trie_source_files_.size(); ++i) {
    string content;
    File::ReadFileToStringOrDie(trie_source_files_[i], &content);
    crypto::MD5Update(&ctx,
                      reinterpret_cast<const unsigned char*>(content.data()),
                      content.length());
  }
  MD5Digest digest;
  crypto::MD5Final(&digest, &ctx);
  string md5 = crypto::MD5DigestToBase16(digest);

  trie_source_md5_ = md5;
  if (!File::Exists(FLAGS_da_trie2_cache_dir)) {
    File::CreateDir(FLAGS_da_trie2_cache_dir, 0700);
  }
  if (File::Exists(cached_file_)) {
    string cache_content;
    File::ReadFileToStringOrDie(cached_file_, &cache_content);
    if (0 == std::strncmp(md5.data(), cache_content.data(), md5.size())) {
      this->clear();
      size_t trie_size = cache_content.size() - md5.size();
      char* array = new char[trie_size];
      std::memcpy(array, cache_content.data() + md5.size(), trie_size);
      this->set_array_with_ownership(array, trie_size);
      return true;
    }
  }
  return false;
}

template <typename StringT>
bool DATrieBasic2<StringT>::MatchAny(const StringT& input) const {
  for (size_t i = 0; i < input.size(); ++i) {
    if (darts::DoubleArrayCore2<StringT>::matchAny(
        input.c_str() + i, input.size() - i, 0)) {
      return true;
    }
  }
  return false;
}

template class DATrieBasic2<std::string>;
template class DATrieBasic2<string16>;
}  // namespace util

