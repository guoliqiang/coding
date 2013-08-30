// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include <string>
#include <utility>

#include "base/public/at_exit.h"
#include "base/public/flags.h"
#include "base/public/logging.h"
#include "file/public/file.h"
#include "base/public/callback.h"
#include "../public/path_master.h"

using parse_master::DomNode;
using parse_master::ParseMaster;
using path_master::PathMaster;
using path_master::ParserParam;

using namespace std;  // NOLINT
using namespace base;  // NOLINT

DEFINE_string(file, "./testdata/zhidao1.html",
                    "Html file name.");
DEFINE_int32(repeat, 1, "repeat times.");

inline int64 GetTimeInUs() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return (now.tv_sec * 1000 * 1000 + now.tv_usec);
}


class SampleParser {
 public:
  SampleParser() {
  }
  ~SampleParser() {
  }

  bool Parse(ParserParam *param) {
    //  为了测试扫描和触发回调的性能，这里不作任何处理
//    string content;
//    param->node()->GetContent(&content);
    return true;
  }
 private:
  DISALLOW_COPY_AND_ASSIGN(SampleParser);
};

int main(int argc, char *argv[]) {
  LOG(INFO) << "Usage: " << argv[0]
    << "--file=xxx --repeat=";
  base::AtExitManager  atExitManager;

  ParseCommandLineFlags(&argc, &argv, true);
  std::string file = FLAGS_file;
  LOG(INFO) << file;

  int repeat = FLAGS_repeat;
  if (file != "") {
    string html;
    bool success = file::File::ReadFileToString(file, &html);
    if (!success) {
      return -1;
    }

    PathMaster ypm;
    parse_master::ParseMaster pm;
    ypm.Init(&pm);

    vector<string> paths;
    //  注册16个路径作为测试
    paths.push_back("<div class=\"f14\" id=\"question_content\"><cd><pre>");
    paths.push_back("<html><body>");
    paths.push_back("<html><head><title>");
    paths.push_back("<html><head><meta>");
    paths.push_back("<div class=\"f14 p90 pl10 answer_content\">"
        "<span id=\"reply_content_531769369\"><cn><pre>");
    paths.push_back("<html><body><div><div><div><div><div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<body><div><div><div><div><div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div><div><div><div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div><div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><div>"
                    "<p><span class=\"answer_time\">");
    paths.push_back("<div><p><span class=\"answer_time\">");
    paths.push_back("<p style><span class=\"answer_time\">");
    paths.push_back("<div class id=\"best_answer_info\" style align>"
                    "<p style><span class=\"answer_time\">");

    string parser_name = "test";
    SampleParser parser;
    for (int i = 0; i < paths.size(); ++i) {
      ypm.RegistParser(parser_name, paths[i],
          NewPermanentCallback(&parser, &SampleParser::Parse));
    }

    int64 spent_sum = 0;
    int64 spent_max = 0;
    int64 spent_min = 100000;
    ypm.ResetCurrentParser(parser_name);
    for (int i = 0; i < repeat; i++) {
      int64 start = GetTimeInUs();
      pm.SetBuff(html);
      pm.Parse();
      int64 spent = GetTimeInUs() - start;
      spent_max = max(spent, spent_max);
      spent_min = min(spent, spent_min);
      spent_sum += spent;
    }
    int64 average = spent_sum / repeat;
    LOG(INFO) << "total spent: " << spent_sum << "us, spent avg: "\
        << spent_sum / repeat << "us, spent max: " << spent_max
        << "us, spent min: " << spent_min << "us" << endl;
    LOG(INFO) << "about " << 1000000/average << " pages/per second";
  }
  return 0;
}
