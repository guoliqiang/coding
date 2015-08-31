// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-08-30 21:05:34
// File  : multiindexdb.h
// Brief :
// https://github.com/AliceStats/Alice/blob/master/src/alice/multiindex.hpp

#ifndef  THIRD_PART_MULTI_INDEX_PUBLIC_MULTIINDEXDB_H_
#define  THIRD_PART_MULTI_INDEX_PUBLIC_MULTIINDEXDB_H_

#include "third_part/boost/include/boost/multi_index_container.hpp"
#include "third_part/boost/include/boost/multi_index/hashed_index.hpp"
#include "third_part/boost/include/boost/multi_index/ordered_index.hpp"
#include "third_part/boost/include/boost/multi_index/identity.hpp"
#include "third_part/boost/include/boost/multi_index/member.hpp"

namespace multi_index {

namespace bm = boost::multi_index;

// Ordered multi-index container, specific proxy for boost's
// multi_index template
template <typename Key, typename Index, typename Value>
class multiindex {
 public:
  struct entry {
    Key key;  // Key, unordered, non-unique. key->List
    Index index;  // Index, ordered, unique. index->entry
    Value value;
    explicit entry(Key ikey, Index iindex, Value ivalue) :
        key(ikey), index(iindex), value(ivalue) {}
    entry() {}
  };
  typedef entry entry_type;
  // Type for the underlying container
  typedef bm::multi_index_container <
    entry_type, // element to store
    bm::indexed_by <
      bm::hashed_non_unique <
        bm::member <entry_type, Key, &entry_type::key>  // key
      >, 
      bm::ordered_unique <
        bm::member <entry_type, Index, &entry_type::index>  // index
      >
    >
  > multiindexdb;
  typedef typename multiindexdb::size_type size_type;
  typedef Key key_type;
  typedef Index index_type;
  typedef Value value_type;
  // list of keys
  typedef typename multiindexdb::template nth_index<0>::type key_set;
  // iterator for access by key
  typedef typename key_set::iterator key_iterator;
  // list of indexes, ordered
  typedef typename multiindexdb::template nth_index<1>::type index_set;
  // iterator for access by index
  typedef typename index_set::iterator index_iterator;

  key_iterator begin() const { return db.template get<0>().begin(); }
  key_iterator end() const { return db.template get<0>().end(); }
  bool hasKey(const Key &k) const {
    return db.template get<0>().find(k) != db.template get<0>().end();
  }
  key_iterator findKey(const Key &k) const {
    return db.template get<0>().find(k);
  }
  bool hasIndex(const Index &k) const {
    return db.template get<1>().find(k) != db.template get<1>().end();
  }
  index_iterator findIndex(const Index &k) const {
    return db.template get<1>().find(k);
  }
  bool keyErase(const Key & k) { return db.template get<0>().erase(k) != 0; }
  bool indexErase(const Index & k){ return db.template get<1>().erase(k) != 0; }
  index_iterator beginIndex() const { return db.template get<1>().begin(); }
  index_iterator endIndex() const { return db.template get<1>().end(); }
  bool insert(const entry_type &e) const { return db.insert(e).second; }
  size_type size() const { return db.size(); }
  void clear() const { db.clear(); }

  // Sets the value for a specific key
  // This function only sets the value for the all key found.
  bool keySet(const Key &k, const Value &v) const {
    key_set &index = db.template get<0>();
    std::pair<key_iterator, key_iterator> it = index.equal_range(k);
    if (it.first == it.second) return false;
    for (key_iterator i = it.first; i != it.second; i++) {
      index.modify(i, change_value(v));
    }
    return true;
  }
  // Sets the value for a specific Index
  bool indexSet(const Index &i, const Value &v) const {
    index_set &index = db.template get<1>();
    index_iterator it = index.find(i);
    if (it == index.end()) return false;
    index.modify(it, change_value(v));
    return true;
  }
 
 private:
  struct change_value {
   public:
    change_value(const Value &v) : v(v) {}
    void operator()(entry_type& e) { e.value = v; }
   private:
    const Value &v;
  };

 private:
  mutable multiindexdb db;
};

}  // namespace multi_index

#endif  // THIRD_PART_MULTI_INDEX_PUBLIC_MULTIINDEXDB_H_
