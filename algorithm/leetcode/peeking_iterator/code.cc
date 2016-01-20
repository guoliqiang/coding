// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-12-23 16:20:45
// File  : code.cc
// Brief :

// Below is the interface for Iterator, which is already defined for you.
// **DO NOT** modify the interface for Iterator.
class Iterator {
    struct Data;
  Data* data;
public:
  Iterator(const vector<int>& nums);
  Iterator(const Iterator& iter);
  virtual ~Iterator();
  // Returns the next element in the iteration.
  int next();
  // Returns true if the iteration has more elements.
  bool hasNext() const;
};


class PeekingIterator : public Iterator {
public:
  PeekingIterator(const vector<int>& nums) : Iterator(nums) {
      // Initialize any member here.
      // **DO NOT** save a copy of nums and manipulate it directly.
      // You should only use the Iterator interface methods.
      v.clear();
  }

    // Returns the next element in the iteration without advancing the iterator.
  int peek() {
      if (v.size()) {
          return v.front();
      } else {
          int t = Iterator::next();
          v.push_back(t);
          return t;
      }
  }

  // hasNext() and next() should behave the same as in the Iterator interface.
  // Override them if needed.
  int next() {
      if (v.size()) {
          int t = v.front();
          v.pop_front();
          return t;
      } else {
          return Iterator::next(); 
      }
  }

  bool hasNext() const {
      if (v.size()) return true;
      else return Iterator::hasNext();
  }
  std::list<int> v;
};

class PeekingIterator : public Iterator {
public:
  PeekingIterator(const vector<int>& nums) : Iterator(nums) {
      // Initialize any member here.
      // **DO NOT** save a copy of nums and manipulate it directly.
      // You should only use the Iterator interface methods.
      val = 0;
      flag = false;
  }

    // Returns the next element in the iteration without advancing the iterator.
  int peek() {
      if (!flag) {
          val = Iterator::next();
          flag = true;
      }
      return val;
  }

  // hasNext() and next() should behave the same as in the Iterator interface.
  // Override them if needed.
  int next() {
      if (flag) {
          flag = false;
          return val;
      } else {
          return Iterator::next();
      }
  }

  bool hasNext() const {
      if (flag) return true;
      else return Iterator::hasNext();
  }
  
  int val;
  bool flag;
};


class PeekingIterator : public Iterator
{
public:
    PeekingIterator(const vector<int> &nums) : Iterator(nums)
    {
    }

    int peek()
    {
        return Iterator(*this).next();
    }

    int next()
    {
        return Iterator::next();
    }

    bool hasNext() const
    {
        return Iterator::hasNext();
    }
};



