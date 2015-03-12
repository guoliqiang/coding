// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// Date  : 2013-09-08 00:16:12
// File  : kernel.cc
// Brief :

#include "nltk/svm/public/kernel.h"

#include <string>
#include <vector>
#include <map>
#include "base/public/string_util.h"

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

static util::StringEnumParser<KernelType> g_kernel_type;

Parameter::Parameter() {
  c_ = FLAGS_c;
  degree_ = FLAGS_degree;
  gamma_ = FLAGS_gamma;
  coef0_ = FLAGS_coef0;
  mem_size_ = FLAGS_mem_size;
  eps_ = FLAGS_eps;
  std::vector<std::string> foo;
  Tokenize(FLAGS_weights, ";", &foo);
  for (int i = 0; i < foo.size(); i++) {
    std::vector<std::string> bar;
    SplitString(foo[i], ':', &bar);
    CHECK_EQ(bar.size(), 2);
    weights_[StringToInt(bar[0])] = StringToDouble(bar[1]);
  }
  CHECK(g_kernel_type.String2Enum(FLAGS_kernel_type, &kernel_type_));
}

std::string Parameter::ToString() const {
  std::string rs;
  rs += "[paremeter]\n";
  rs += "c=" + DoubleToString(c_) + "\n";
  rs += "degree=" + DoubleToString(degree_) + "\n";
  rs += "gamma=" + DoubleToString(gamma_) + "\n";
  rs += "coef0=" + DoubleToString(coef0_) + "\n";
  rs += "mem_size=" + IntToString(mem_size_) + "\n";
  rs += "eps=" + DoubleToString(eps_) + "\n";
  rs += "weights:";
  LOG(INFO) << rs;
  for (std::map<int32_t, double>::const_iterator i = weights_.begin();
       i != weights_.end(); i++) {
    rs += "\t" + IntToString(i->first) + ":" + DoubleToString(i->second);
  }
  rs += "\n";
  std::string foo;
  CHECK(g_kernel_type.Enum2String(kernel_type_, &foo));
  rs += "kernel type=" + foo + "\n";
  rs += "[parameter]";
  return rs;
}

std::string ProblemNode::ToString() const {
  std::string rs;
  rs += "line no:" + IntToString(line_no) + "\n";
  rs += "lable:" + IntToString(lable) + "\n";
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

}  // namespace svm
}  // namespace nltk
