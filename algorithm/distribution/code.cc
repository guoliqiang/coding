// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// I just want to GH to hss~
// Date  : 2015-06-30 05:23:53
// File  : code.cc
// Brief :

// https://www.topcoder.com/tc?module=Static&d1=features&d2=071708
// http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c15319/A-TR1-Tutorial-Generating-Random-Numbers.htm

// thread safe
// Gauss distribution
static std::tr1::ranlux64_base_01 eng(time(NULL));
static inline double NormalRandom(double mean, double var) {
  std::tr1::normal_distribution<double> normal(mean, var);
  return normal(eng);
}  
