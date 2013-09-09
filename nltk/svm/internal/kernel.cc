// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-09-08 00:16:12
// File  : kernel.cc
// Brief :
#include "../public/kernel.h"

namespace nltk {
namespace svm {

DEFINE_double(c, 1, "");
DEFINE_double(degree, 3, "");
DEFINE_double(gamma, 0, "");
DEFINE_double(coef0, 0, "");
DEFINE_string(kernel_type, "RBF", "");
DEFINE_int32(mem_size, 1000, "");
DEFINE_double(eps, 1e-3, "");
DEFINE_string(weights, "", "");

Parameter::Parameter() {
  c_ = FLAGS_c;
  degree_ = FLAGS_degree;
  gamma_ = FLAGS_gamma;
  coef0_ = FLAGS_coef0;
  mem_size_ = FLAGS_mem_size;
  eps_ = FLAGS_eps;
  std::vector<std::string> foo;
  Tokenize(FLAGS_weights, "\t ", &foo);
  for (int i = 0; i < foo.size(); i++) {
    std::vector<std::string> bar;
    SplitString(foo[i], ':', &bar);
    CHECK_EQ(bar.size(), 2);
    weights_[StringToInt(bar[0])] = StringToDouble(bar[1]);
  }
  CHECK(g_kernel_type.String2Enum(FLAGS_kernel_type, &kernel_type_));
}


}  // namespace svm
}  // namespace nltk
