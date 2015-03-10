// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 00:16:12
// File  : kernel.cc
// Brief :

#include "nltk/svr/public/kernel.h"

#include <string>
#include <vector>
#include <map>
#include "base/public/string_util.h"

namespace nltk {
namespace svr {

DEFINE_double(c, 1, "");
DEFINE_double(degree, 3, "");
DEFINE_double(gamma, 0, "");
DEFINE_double(coef0, 0, "");
DEFINE_string(kernel_type, "RBF", "");
DEFINE_int32(mem_size, 1000, "");
DEFINE_double(eps, 1e-3, "");
DEFINE_double(p, 0.1, "the epsilon in loss function of epsilon-SVR");

static util::StringEnumParser<KernelType> g_kernel_type;

Parameter::Parameter() {
  c_ = FLAGS_c;
  degree_ = FLAGS_degree;
  gamma_ = FLAGS_gamma;
  coef0_ = FLAGS_coef0;
  mem_size_ = FLAGS_mem_size;
  eps_ = FLAGS_eps;
  p_ = FLAGS_p;
  CHECK(g_kernel_type.String2Enum(FLAGS_kernel_type, &kernel_type_));
}

std::string Parameter::ToString() {
  std::string rs;
  rs += "[paremeter]\n";
  rs += "c=" + DoubleToString(c_) + "\n";
  rs += "degree=" + DoubleToString(degree_) + "\n";
  rs += "gamma=" + DoubleToString(gamma_) + "\n";
  rs += "coef0=" + DoubleToString(coef0_) + "\n";
  rs += "mem_size=" + IntToString(mem_size_) + "\n";
  rs += "eps=" + DoubleToString(eps_) + "\n";
  rs += "p=" + DoubleToString(p_) + "\n";
  rs += "\n";
  std::string foo;
  CHECK(g_kernel_type.Enum2String(kernel_type_, &foo));
  rs += "kernel type=" + foo + "\n";
  rs += "[parameter]";
}

std::string ProblemNode::ToString() {
  std::string rs;
  rs += "line no:" + IntToString(line_no) + "\n";
  rs += "value:" + IntToString(value) + "\n";
  rs += "sparse array:" + JoinKeysValues(&element.Get());
  return rs;
}

void Kernel::Set(base::shared_ptr<Parameter> para) {
  para_ = para;
  f_ = NULL;
  if (para->kernel_type_ == LINEAR) {
    f_ = &Kernel::linear;
  } else if (para->kernel_type_ == POLY) {
    f_ = &Kernel::poly;
  } else if (para->kernel_type_ == RBF) {
    f_ = &Kernel::rbf;
  } else if (para->kernel_type_ == SIGMOID) {
    f_ = &Kernel::sigmoid;
  } else {
    CHECK(false) << "unknown kernel type";
  }
}

}  // namespace svr
}  // namespace nltk
