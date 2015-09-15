// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 18:16:45
// File  : problem.cc
// Brief :

#include "nltk/lda/public/problem.h"

#include <fstream>  // NOLINT
#include "base/public/string_util.h"

namespace nltk {

namespace lda {

Problem::Problem(const std::string & path) {
  std::ifstream fin(path.c_str(), std::ifstream::in);
  CHECK(fin.good()) << "can not read " << path;
  std::string line;
  std::vector<std::string> words;
  while (getline(fin, line)) {
    words.clear();
    Tokenize(line, " \t", &words);
    doc_words_.push_back(words);
  }
}

}  // namespace lda
}  // namespace nltk
