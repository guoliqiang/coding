// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: wangbin@yunrang.com (Bin Wang)

#include <algorithm>

#include "testing/gtest/include/gtest/gtest.h"
#include "util/simhash/simhash.h"
#include "file/sstable/public/sstable.h"
#include "file/file.h"
#include "indexing/proto/gen-cpp/mergeddoc_types.h"
#include "base/yr.h"
#include "base/thrift.h"
#include "indexing/base/seg_text_tokenizer.h"
#include "base/scoped_ptr.h"
#include "util/segmenter/word_idf/word_idf_dict.h"

using file::SSTable;
using segmenter::WordIDFDictionary;
using segmenter::WordIDFDictFactory;

DEFINE_string(input_sstable_paths, "util/simhash/test_data/", "");

TEST(Simhash, Fingerprint) {
  vector<string> files;
  file::File::GetFilesInDirOrDie(FLAGS_input_sstable_paths, &files);
  scoped_ptr<SSTable> sstable;
  scoped_ptr<SSTable::Iterator> iter;
  ThriftObjReader thrift_reader;
  indexing::MergedDoc mergeddoc;
  vector<string> words;
  vector<pair<string, int> > words_with_power;
  uint64 digest;
  WordIDFDictionary* idf_dict_ =
      Singleton<WordIDFDictFactory>::get()->GetDict(20110428);
  double idf;
  vector<pair<uint64, string> > digests;
  for (vector<string>::iterator it = files.begin(); it != files.end(); it++) {
    sstable.reset(SSTable::Open(*it, SSTable::ON_DISK));
    iter.reset(sstable->NewIterator());
    while (!iter->done()) {
      words.clear();
      words_with_power.clear();
      thrift_reader.FromStringToThrift(iter->value(), &mergeddoc);
      indexing::GetTermsFromSegString(mergeddoc.seg_body, &words);
      for (vector<string>::iterator it1 = words.begin();
           it1 != words.end(); it1++) {
        idf_dict_->GetIDFLogValue(*it1, &idf);
        words_with_power.push_back(make_pair(*it1, static_cast<int>(idf)));
      }
      if (util::simhash::Fingerprint(words_with_power, &digest)) {
        digests.push_back(make_pair(digest, *it + "\t" + mergeddoc.url));
      }
      iter->Next();
    }
  }
  sort(digests.begin(), digests.end());
  for (vector<pair<uint64, string> >::iterator it = digests.begin();
      it != digests.end(); it++) {
    printf("%.16lx\t%s\n", it->first, it->second.data());
  }
}
