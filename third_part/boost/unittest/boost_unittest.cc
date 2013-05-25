// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-05-25 19:45:57
// File  : boost_unittest.cc
// Brief :

#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/regex.hpp>
#include <iostream>
#include <string>

void TestRegex() {
  std::string line;
  boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );
  while (std::cin) {
    std::getline(std::cin, line);
    boost::smatch matches;
    if (boost::regex_match(line, matches, pat))
      std::cout << matches[2] << std::endl;
  }
}

void TestOnlyHeader() {
  using namespace boost::lambda;  // NOLINT
  typedef std::istream_iterator<int> in;
  std::for_each(in(std::cin),
                in(), std::cout << (_1 * 3) << " ");
}

int main() {
  TestRegex();
}
