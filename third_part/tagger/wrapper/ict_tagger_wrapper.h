// Copyright 2013 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang@jike.com)
// I just want to GH to hss~
// Date  : 2013-03-26 15:15:20
// File  : IctTaggerWrapper.h
// Brief :

#ifndef  BASE_ICTTAGGERWRAPPER_H_
#define  BASE_ICTTAGGERWRAPPER_H_
#include <vector>
#include <string>
#include "../public/tagger.h"
#include "../../../base/public/logging.h"
#include "../../../base/public/mutex.h"
#include "../../../base/public/string_util.h"
#include "../../../base/public/concurrent_queue.h"

namespace ict_wrapper {
class IctTaggerMeta {
 public:
  explicit IctTaggerMeta(IctTagger * ptr) : tagger_internal_(ptr) {
    VLOG(3) << "initial ictagger";
    CHECK(ptr->Init())
          << "initial icttagger error";
  }
  ~IctTaggerMeta() {
    tagger_internal_->Exit();
    delete tagger_internal_;
  }
  //
  IctTagger * get() {
    return tagger_internal_;
  }
  //
  void Tagger(std::string str, std::vector<std::string> * words,
             std::vector<std::string> * pos) {
      tagger_internal_->Parse(str, *words, *pos);
  }
  //
  void Tagger(const std::string & str, std::string * pos_str) {
    std::vector<std::string> words;
    std::vector<std::string> pos;
    Tagger(str, &words, &pos);
    for (int i = 0; i < words.size(); i++) {
      TrimString(words[i], "\t \r\n", &words[i]);
      TrimString(pos[i], "\t \r\n", &pos[i]);
      if (words[i].empty() || pos[i].empty()) continue;
      (*pos_str) += words[i] + ":" + pos[i] + "\t";
    }
  }
 //
 private:
  IctTagger * tagger_internal_;
};

class IctTaggerWrapper {
 public:
  static IctTagger* GetInstance() {
    static IctTaggerMeta tg(new IctTagger());
    return tg.get();
  }
  static void Tagger(std::string str, std::vector<std::string> * words,
                     std::vector<std::string> * pos) {
    static base::Mutex mutex;
    {
      base::MutexLock lock(&mutex);
      GetInstance()->Parse(str, *words, *pos);
    }
  }
  static void Tagger(const std::string & str, std::string * pos_str) {
    std::vector<std::string> words;
    std::vector<std::string> pos;
    Tagger(str, &words, &pos);
    for (int i = 0; i < words.size(); i++) {
      TrimString(words[i], "\t \r\n", &words[i]);
      TrimString(pos[i], "\t \r\n", &pos[i]);
      if (words[i].empty() || pos[i].empty()) continue;
      (*pos_str) += words[i] + ":" + pos[i] + "\t";
    }
  }
};

template <class T>
class MultiItem {
 public:
  bool Init(int count) {
    return InitInternal(count);
  }
  virtual ~MultiItem() {
    while (!queue_internal_.Empty()) {
      delete Pop();
    }
  }
  virtual T * Pop() {
    T * rs = NULL;
    queue_internal_.Pop(rs);
    return rs;
  }
  virtual void Push(T * input) {
    queue_internal_.Push(input);
  }
 protected:
  virtual bool InitInternal(int count) = 0;
  base::ConcurrentQueue<T*> queue_internal_;
};

class MultiTagger: public MultiItem<IctTaggerMeta> {
 public:
  void Tagger(std::string str, std::vector<std::string> * words,
              std::vector<std::string> * pos) {
    IctTaggerMeta * ict = Pop();
    ict->Tagger(str, words, pos);
    Push(ict);
  }
  void Tagger(const std::string & str, std::string * pos_str) {
    IctTaggerMeta * ict = Pop();
    ict->Tagger(str, pos_str);
    Push(ict);
  }
 protected:
  virtual bool InitInternal(int count) {
    for (int i = 0; i < count; i++) {
      Push(new IctTaggerMeta(new IctTagger()));
    }
    return true;
  }
};
}
#endif  // ICTTAGGERWRAPPER_H_
