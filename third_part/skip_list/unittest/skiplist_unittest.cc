// Copyright 2014 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2014-11-24 19:25:42
// File  : skiplist_unittest.cc
// Brief :

#include "base/public/logging.h"
#include "third_part/testing/gtest/include/gtest/gtest.h"
#include "third_part/skip_list/public/skiplist.h"
#include "base/public/thread.h"
#include "base/public/mutex.h"

template <typename Key>
struct Comparator {
  int operator()(const Key& a, const Key& b) const {
    if (a < b) {
      return -1;
    } else if (a > b) {
      return +1;
    } else {
      return 0;
    }
  }
};

TEST(SkipList, Normal) {
  skiplist::SkipList<double, Comparator<double> > sl;
  sl.Insert(-10);
  skiplist::SkipList<double, Comparator<double> >::Iterator iterator(&sl);
  iterator.SeekToFirst();
  while (iterator.Valid()) {
    LOG(INFO) << iterator.key();
    iterator.Next();
  }
}

class Reader : public base::Thread {
 public:
  Reader(skiplist::SkipList<double, Comparator<double> > * list)
      : base::Thread(true) {
    list_ = list;
  }

 protected:
  void Run() {
    sleep(1);
    skiplist::SkipList<double, Comparator<double> >::Iterator iterator(list_);
    iterator.SeekToFirst();
    while (iterator.Valid()) {
      LOG(INFO) << iterator.key();
      sleep(3);
      iterator.Next();
    }
  }

 private:
  skiplist::SkipList<double, Comparator<double> > * list_;
};

class Writer : public base::Thread {
 public:
  Writer(skiplist::SkipList<double, Comparator<double> > * list)
      : base::Thread(true) {
    list_ = list;
  }
 protected:
  void Run() {
    for (int i = 0; i < 10; i++) {
      int t = base::Random() % 100;
      {
        base::MutexLock lock(&mutex_);
        if (set_.count(t)) continue;
        set_.insert(t);
        list_->Insert(t);
      }
      LOG(INFO) << i << " insert :" << t;
      if (i % 2 == 0) sleep(1);
    }
  }
 private:
  skiplist::SkipList<double, Comparator<double> > * list_;
  static base::Mutex mutex_;
  static std::set<int> set_;
};

base::Mutex Writer::mutex_;
std::set<int> Writer::set_;

TEST(SkipList, OneWriterTwoReader) {
  skiplist::SkipList<double, Comparator<double> > list;
  Writer writer(&list);
  Reader reader1(&list);
  Reader reader2(&list);
  writer.Start();
  reader1.Start();
  reader2.Start();
  writer.Join();
  reader1.Join();
  reader2.Join();
}

TEST(SkipList, MultiWriterReader) {
  skiplist::SkipList<double, Comparator<double> > list;
  Writer writer1(&list);
  Writer writer2(&list);
  Reader reader1(&list);
  Reader reader2(&list);
  writer1.Start();
  writer2.Start();
  reader1.Start();
  reader2.Start();
  writer1.Join();
  writer2.Join();
  reader1.Join();
  reader2.Join();
}
