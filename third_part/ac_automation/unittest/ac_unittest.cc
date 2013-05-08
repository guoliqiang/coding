// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-05-08 16:44:10
// File  : ac_unittest.cc
// Brief :
#include "../public/ac_automaton.h"
#include "../../../base/public/logging.h"
#include "../../../base/public/string_util.h"


using namespace ac_automaton;

int main(int argc, char** argv) {
  ACAutomaton ac;
  std::vector<std::string> patterns;
  patterns.push_back("he");
  patterns.push_back("she");
  patterns.push_back("hiad");
  patterns.push_back("ia");
  CHECK(ac.Init(patterns));
  std::string text = "womhiadenshe";

  std::map<std::string, std::vector<size_t> > rs;
  ac.Match(text, &rs);

  for (std::map<std::string, std::vector<size_t> >::iterator
       i = rs.begin(); i != rs.end(); i++) {
    LOG(INFO) << i->first << " " << JoinVector(i->second, ',');
  }
  return 0;
}
