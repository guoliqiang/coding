// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: jaingwang@yunrang.com (Wang Jiang)
// Author: james@yunrang.com (Jun Min Gao)

#include <stdio.h>
#include <string.h>
#include <iostream>  // use stream for test only NOLINT
#include "util/html/html_unescape_char.h"
#include "base/utf.h"
#include "base/flags.h"

DEFINE_bool(sid, false, "unscape standard input");
using util_html::HtmlUtil;

int main(int argc, char *argv[]) {
  base::ParseCommandLineFlags(&argc, &argv, true);
  HtmlUtil util;
  std::string s;
  std::string line, out;
  while (FLAGS_sid && std::getline(std::cin, line)) {
    util.UnescapeString(line, &out);
    std::cout << out << std::endl;
  }
  if (FLAGS_sid) return 0;
  util.Unescape("&#913;", &s);
  fprintf(stdout, "original string for '&#913;' :%s\n", s.c_str());

  util.Unescape("&quot;", &s);
  fprintf(stdout, "original string for '&quot;' :%s\n", s.c_str());

  util.Unescape("&quot", &s);
  fprintf(stdout, "original string for '&quot;' :%s\n", s.c_str());

  return 0;
}
