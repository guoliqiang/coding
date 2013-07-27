// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-07-17 17:43:41
// File  : word_count.cc
// Brief :

#include<algorithm>
#include<limits>
#include<string>
#include"stdint.h"
#include"third_part/hadoop_pipes/public/Pipes.hh"
#include"third_part/hadoop_pipes/public/TemplateFactory.hh"
#include"third_part/hadoop_pipes/public/StringUtils.hh"

using namespace std;

class WordCountMapper:public HadoopPipes::Mapper{
 public:
  WordCountMapper(HadoopPipes::TaskContext&context){}
  void map(HadoopPipes::MapContext& context) {
    string line =context.getInputValue();
    vector<string>word = HadoopUtils::splitString(line, " ");
    for (unsigned int i=0; i<word.size(); i++) {
      context.emit(word[i],HadoopUtils::toString(1));
    }
  }
};

class WordCountReducer:public HadoopPipes::Reducer {
 public:
  WordCountReducer(HadoopPipes::TaskContext&context){}
  void reduce(HadoopPipes::ReduceContext& context) {
    int count = 0;
    while (context.nextValue()) {
           count +=HadoopUtils::toInt(context.getInputValue());
    }
    context.emit(context.getInputKey(), HadoopUtils::toString(count));
  }
};

int main(int argc, char** argv) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory<WordCountMapper, WordCountReducer>());
}
