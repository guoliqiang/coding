// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "base/public/logging.h"
#include "../public/http_client.h"
#include "file/public/file.h"
#include "base/public/at_exit.h"

DEFINE_string(url, "http://www.baidu.com", "url to get");
DEFINE_bool(utf8, true, "if true , try to convert to utf8 charset");
DEFINE_string(outfile, "out.html", "filename to save response data");

using util::http_client::HttpClient;
using util::http_client::HttpGet;
using util::http_client::HttpGetAndTranslateToUtf8;
using std::string;


int main(int argc, char *argv[]) {
  base::AtExitManager  exit;
  base::ParseCommandLineFlags(&argc, &argv, false);
  CHECK(!FLAGS_url.empty()) << "url CANNOT empty";
  string data;
  if (FLAGS_utf8) {
    if (!HttpGetAndTranslateToUtf8(FLAGS_url, &data)) {
      LOG(INFO) << "fail to download this page";
      return -1;
    }
  } else {
    if (!HttpGet(FLAGS_url, &data)) {
      LOG(INFO) << "fail to download this page";
      return -1;
    }
  }

  LOG(INFO) << "html size = " << data.size();
  if (file::File::WriteStringToFile(data, FLAGS_outfile)) {
    LOG(INFO) << "save html to file:" << FLAGS_outfile;
  } else {
    LOG(INFO) << "fail to save html to file:" << FLAGS_outfile;
  }

  return 0;
}
