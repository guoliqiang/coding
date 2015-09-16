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
  70: i32 out_words;
  80: i32 out_topics;
  90: map<string, i32> word2id;
  100: map<i32, i32> id2id;
  110: list<list<double> > phi;
  120:list<list<double> > theta;
  130:list<list<i32> > nword;
  140:list<list<i32> > ntopic;
  150:list<list<i32> > z;
  160:list<i32> nword_sum;
  170:list<list<i32> > doc_words;
}

struct doc_topic {
  1 :  list< list<string> > top_topics;
}

struct topic_word {
  1:  list< list<string> > top_words;
}
