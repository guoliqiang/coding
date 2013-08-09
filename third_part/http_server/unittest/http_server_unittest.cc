// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-08-08 15:25:29
// File  : http_server_unittest.cc
// Brief :
#include "../public/http_server.h"
#include "base/public/at_exit.h"
#include "base/public/logging.h"


using namespace util::http_server;  // NOLINT

bool HelloWord(const Request& req, Respond* res) {
  res->AddContent("<htlm><head><title>Hello Http Server</title></head><body>Hello Http Server</body></html>");
  return true;
}

int main(int argc, char** argv) {
  base::AtExitManager exit;
  base::ParseCommandLineFlags(&argc, &argv, true);
  HttpServer http_server(9010, 1, true);
  http_server.SetDftHandler(HelloWord);
  http_server.TimingStart();
  http_server.Join();
  return 0;
}
