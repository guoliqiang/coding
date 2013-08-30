// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: yongyipeng@yunrang.com (Yongyi PENG)

#include <string>
#include <utility>

#include "base/public/time.h"
#include "base/public/logging.h"
#include "file/public/file.h"
#include "third_part/parser_master/public/parse_master.h"

using parse_master::DomNode;
using parse_master::ParseMaster;
using std::string;
using base::ParseCommandLineFlags;
using base::GetTimeInMs;

DEFINE_string(file, "./test_data/snp.html", "Html file name.");
DEFINE_int32(repeat, 100, "repeat times.");
DEFINE_bool(benchmark_reparse, true, "");

void BenchmarkCreateDomTreeOnce(const string& html) {
  ParseMaster pm;
  pm.SetBuff(html.c_str(), html.length());
  int parse_times = 0;
  int64 start = GetTimeInMs();
  pm.Parse();
  ++parse_times;
  int64 first_end = GetTimeInMs();
  for (int i = 0; i < FLAGS_repeat; i++) {
    pm.Parse();
    ++parse_times;
  }
  int64 end = GetTimeInMs();
  int64 total_time = end - start;
  LOG(INFO) << "create dom tree only in first parsing";
  LOG(INFO) << "total time:" << total_time
            << "\nparse times:" << parse_times
            << "\naverage time(first and non-first):"
            << total_time / parse_times;
  double average = static_cast<double>(end - first_end)/ FLAGS_repeat;
  LOG(INFO) << "average time for non-first parsing:"
            << average;
  LOG(INFO) << "preformance:" << 1000.0 / average << " docs/s";
}

void BenchmarkReCreateDomTree(const string& html) {
  ParseMaster pm;
  pm.SetBuff(html.c_str(), html.length());
  int64 start = GetTimeInMs();
  for (int i = 0; i < FLAGS_repeat; i++) {
    pm.Parse();
  }
  int64 end = GetTimeInMs();
  int64 total_time = end - start;
  LOG(INFO) << "create dom tree each time";
  double average = static_cast<double>(total_time) / FLAGS_repeat;
  LOG(INFO) << "total time:" << total_time
            << "\nparse times:" << FLAGS_repeat
            << "\naverage time for first parsing:" << average;
  LOG(INFO) << "preformance:" << 1000.0 / average << " docs/s";
}


int main(int argc, char *argv[]) {
  base::AtExitManager  atExitManager;
  ParseCommandLineFlags(&argc, &argv, true);
  if (FLAGS_benchmark_reparse) {
    string html;
    CHECK(file::File::ReadFileToString(FLAGS_file, &html)) << FLAGS_file;
    LOG(INFO) << "file size:" << html.length();
    BenchmarkCreateDomTreeOnce(html);
    BenchmarkReCreateDomTree(html);
  }
  return 0;
}
