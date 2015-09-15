# Copyright 2015. All Rights Reserved.
# Author: Liqiang Guo(guoliqiang2006@gmail.com)
# Date  : 2015-09-14 09:46:45

namespace cpp nltk.lda

struct modelout {
  1:  i32 M;
  10: i32 V;
  20: i32 K;
  30: double alpha;
  40: double beta;
  50: i32 niter;
  60: i32 save_niter;
  70: i32 twords;
  80: map<string, i32> word2id;
  90: map<i32, i32> id2id;
  100: list<list<double> > phi;
  110:list<list<double> > theta;
  120:list<list<i32> > nword;
  130:list<list<i32> > ntopic;
  140:list<list<i32> > z;
  150:list<i32> nword_sum;
  160:list<list<i32> > doc_words;
}
