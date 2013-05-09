// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-05-08 16:44:10
// File  : ac_unittest.cc
// Brief :
#include "../public/ac_automaton.h"
#include "../public/ac_automaton_unit.h"
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
  ACAutomatonUnit<std::string> ac_unit;
  std::vector<std::vector<std::string> > patterns2;
  std::vector<std::string> pattern2;
  pattern2.push_back("清华");
  pattern2.push_back("大学");
  patterns2.push_back(pattern2);
  pattern2.clear();
  pattern2.push_back("北京");
  pattern2.push_back("大学");
  patterns2.push_back(pattern2);
  ac_unit.Init(patterns2);
  std::map<std::vector<std::string>, std::vector<size_t> > rs2;
  std::vector<std::string> text2;
  text2.push_back("大家");
  text2.push_back("说");
  text2.push_back("：");
  text2.push_back("北京");
  text2.push_back("大学");
  text2.push_back("是");
  text2.push_back("一所");
  text2.push_back("好学校");
  ac_unit.Match(text2, &rs2);
  for (std::map<std::vector<std::string>, std::vector<size_t> >::iterator i = rs2.begin();
       i != rs2.end(); i++) {
    LOG(INFO) << JoinVector(i->first, ',') << " find at :" << JoinVector(i->second, ',');
  }

  return 0;
}
