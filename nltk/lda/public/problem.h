// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-09-13 16:46:47
// File  : problem.h
// Brief :

#ifndef  NLTK_LDA_PUBLIC_PROBLEM_H_
#define  NLTK_LDA_PUBLIC_PROBLEM_H_

#include <string>
#include <vector>
#include "base/public/logging.h"

namespace nltk{

namespace lda {

class Problem {
 public:
  Problem(const std::string & path);

 public:
  // M_ * doc.size()
  // doc_words_[i][j] word at position j of doc[i]
  std::vector<std::vector<std::string> > doc_words_;
};

}  // namespace lda
}  // namespace nltk

#endif  // NLTK_LDA_PUBLIC_PROBLEM_H_
