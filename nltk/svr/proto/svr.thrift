# Copyright 2013 Jike Inc. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang@jike.com)
# Date  : 2013-09-08 09:46:45

namespace cpp nltk.svr

struct modelnode {
  1:  double b = 0;
  10: double best_obj = 0;
  20: map<i32, double> sparse_alpha;
}

struct parameter {
  1:  double c = 1;
  10: map<i32, double> weights;
  20: double degree;
  30: double gamma;
  40: double coef0;
  50: double mem_size;
  60: double eps;
  70: double p;
  80: i32 kernel_type;
}


struct problemnode {
  1:  i32 value;
  10: map<i32, double> sparse_array;
}

struct featurenode {
  1:  double max;
  10: double min;
}

struct modelout {
 1:  parameter para;
 10: modelnode model;
 20: list<problemnode> node;
 30: map<i32, featurenode> feature_max_min;
}
