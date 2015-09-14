# Copyright 2015. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang2006@gmail.com)
# Date  : 2015-09-14 09:46:45

namespace cpp nltk.lda

struct modelout {
 1:  parameter para;
 10: map<i32, map<i32, modelnode> > model;
 20: list<problemnode> node;
 30: map<i32, i32> start;
 40: map<i32, i32> count;
 50: map<i32, featurenode> feature_max_min;
}

