// Copyright 2012 Yunrang Inc. All Rights Reserved.
// Author: dahaili@yunrang.com (Dahai Li)

#include <string>
#include <vector>

#include "base/thrift.h"
#include "base/yr.h"
#include "file/sstable/public/sstable.h"
#include "indexing/proto/gen-cpp/mergeddoc_types.h"
#include "testing/gtest/include/gtest/benchmark.h"
#include "util/compression/compression.h"
#include "util/gtl/stl_util-inl.h"

namespace {

string kTextData;
vector<string> kWebData;
vector<string> kWebBinData;

const int kTestDataLength = 100000;
const int kTestRateTimes = 100;
const int kTestedWebDataLength = 50;
const char *kCompressionTypes[] = {
  "snappy",
  "lzo",
  "zlib",
  "gzip",
};
enum TestType {
  kRandomText,
  kWebText,
  kWebBinary,
};

}  // namespace

namespace util {

void FillData() {
  kTextData.resize(kTestDataLength);
  for (int i = 0; i < kTestDataLength; ++i) {
    kTextData[i] = 'a' + rand() % 26;
  }
  if (kWebData.empty()) {
    scoped_ptr<file::SSTable> table(
        file::SSTable::Open("util/compression/testdata/sample_input_1000",
                            file::SSTable::ON_DISK));
    CHECK(table.get());
    scoped_ptr<file::SSTable::Iterator> iter(table->NewIterator());
    for (int i = 0; !iter->done() && i < kTestedWebDataLength;
         iter->Next(), ++i) {
      kWebBinData.push_back(iter->value());
      indexing::MergedDoc doc;
      FromStringToThriftFast(iter->value(), &doc);
      kWebData.push_back(doc.content);
    }
  }
}

void DoBenchmark(Compression *com, int iteration, TestType type) {
  if (kTextData.empty()) {
    FillData();
  }

  string compressed_data;
  testing::StopBenchmarkTiming();
  testing::StartBenchmarkTiming();
  for (int i = 0; i < iteration; ++i) {
    switch (type) {
    case kRandomText:
      CHECK(com->Compress(kTextData, &compressed_data));
      break;
    case kWebText:
      for (int j = 0; j < kWebData.size(); ++j)
        CHECK(com->Compress(kWebData[j], &compressed_data));
      break;
    case kWebBinary:
      for (int j = 0; j < kWebBinData.size(); ++j)
        CHECK(com->Compress(kWebBinData[j], &compressed_data));
      break;
    }
  }
  testing::StopBenchmarkTiming();
}

void SnappyComText(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("snappy");
  DoBenchmark(com, iteration, kRandomText);
  delete com;
}

void SnappyComWeb(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("snappy");
  DoBenchmark(com, iteration, kWebText);
  delete com;
}

void SnappyComWebBin(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("snappy");
  DoBenchmark(com, iteration, kWebBinary);
  delete com;
}

void GzipComText(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("gzip");
  DoBenchmark(com, iteration, kRandomText);
  delete com;
}

void GzipComWeb(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("gzip");
  DoBenchmark(com, iteration, kWebText);
  delete com;
}

void GzipComWebBin(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("gzip");
  DoBenchmark(com, iteration, kWebBinary);
  delete com;
}

void ZlibComText(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("zlib");
  DoBenchmark(com, iteration, kRandomText);
  delete com;
}

void ZlibComWeb(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("zlib");
  DoBenchmark(com, iteration, kWebText);
  delete com;
}

void ZlibComWebBin(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("zlib");
  DoBenchmark(com, iteration, kWebBinary);
  delete com;
}

void LzoComText(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("lzo");
  DoBenchmark(com, iteration, kRandomText);
  delete com;
}

void LzoComWeb(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("lzo");
  DoBenchmark(com, iteration, kWebText);
  delete com;
}

void LzoComWebBin(int iteration) {
  Compression *com = Compression::GetCompressionAlgorithm("lzo");
  DoBenchmark(com, iteration, kWebBinary);
  delete com;
}

BENCHMARK(SnappyComText)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(SnappyComWeb)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(SnappyComWebBin)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(GzipComText)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(GzipComWeb)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(GzipComWebBin)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(ZlibComText)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(ZlibComWeb)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(ZlibComWebBin)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(LzoComText)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(LzoComWeb)->ThreadRange(1, testing::NumCPUs());
BENCHMARK(LzoComWebBin)->ThreadRange(1, testing::NumCPUs());

void BenchmarkCompressionRate() {
  const int type_size = arraysize(kCompressionTypes);
  vector<float> compressed_text_lengths(type_size, 0);
  vector<float> compressed_bin_lengths(type_size, 0);
  vector<float> compressed_web_lengths(type_size, 0);
  vector<float> compressed_web_bin_lengths(type_size, 0);
  vector<Compression*> coms;
  for (int i = 0; i < type_size; ++i) {
    coms.push_back(Compression::GetCompressionAlgorithm(kCompressionTypes[i]));
  }
  string compressed_data;
  for (int i = 0; i < kTestRateTimes; ++i) {
    FillData();
    for (int j = 0; j < type_size; ++j) {
      coms[j]->Compress(kTextData, &compressed_data);
      compressed_text_lengths[j] +=
          compressed_data.size() * 100.0 / kTextData.size();
      VLOG(1) << "m: " << kCompressionTypes[j] << "text: " << kTextData.size()
              << ", compressed: " << compressed_data.size();
    }
  }
  int64 web_text_total_length = 0;
  for (int i = 0; i < kWebData.size(); ++i)
    web_text_total_length += kWebData[i].size();
  int64 web_bin_total_length = 0;
  for (int i = 0; i < kWebBinData.size(); ++i)
    web_bin_total_length += kWebBinData[i].size();

  for (int i = 0; i < type_size; ++i) {
    int64 total = 0;
    for (int j = 0; j < kWebData.size(); ++j) {
      CHECK(coms[i]->Compress(kWebData[i], &compressed_data));
      total += compressed_data.size();
    }
    compressed_web_lengths[i] =
        static_cast<float>(total) / web_text_total_length;
    total = 0;
    for (int j = 0; j < kWebBinData.size(); ++j) {
      CHECK(coms[i]->Compress(kWebBinData[i], &compressed_data));
      total += compressed_data.size();
    }
    compressed_web_bin_lengths[i] =
        static_cast<float>(total) / web_bin_total_length;
  }

  LOG(INFO) << "overall compression rate:";
  for (int i = 0; i < type_size; ++i) {
    LOG(INFO) << kCompressionTypes[i] << ",\ttext: "
              << compressed_text_lengths[i]/kTestRateTimes << "%\tbin: "
              << compressed_bin_lengths[i]/kTestRateTimes << "%\tweb text: "
              << compressed_web_lengths[i] * 100 << "%\tweb bin: "
              << compressed_web_bin_lengths[i] * 100 << "%";
  }
  gtl::STLDeleteElements(&coms);
}

}  // namespace util

int main(int argc, char **argv) {
  ParseCommandLineFlags(&argc, &argv, true);
  testing::RUN_ALL_BENCHMARKS();
  util::BenchmarkCompressionRate();

  return 0;
}

