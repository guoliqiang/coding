#ifndef CQLIST_H_
#define CQLIST_H_

#include <stdio.h>

#define CQLIST_DEFAULT_CAP 128

template <class T>
class cqlist {
 public:
	cqlist() {
		cap = CQLIST_DEFAULT_CAP;
		elems = new T[cap];
		head = tail = 0;
	}

	cqlist(size_t _c) {
		cap = _c;
		elems = new T[cap];
		head = tail = 0;
	}

	void reset(size_t _c) {
		if (elems) delete []elems;
		cap = _c;
		elems = new T[cap];
		head = 0;
    tail = 0;
	}

	virtual ~cqlist() {
		if (elems) delete []elems;
	}

public:
	bool empty() {
    return (head==tail);
  }
	bool full() {
    return (tail + 1) % cap == head;
  }

	bool push(const T& e) {
		if (full()) return false;
		elems[tail] = e;
		tail = (tail + 1) % cap;
		return true;
	}

	bool pop() {
		if (empty()) return false;
		head = (head + 1) % cap;
		return true;
	}

	T& front() {
		return elems[head];
	}

	const T& front() const {
		return elems[head];
	}

	size_t size() {
		return ((tail-head) + cap) % cap;
	}

public:
	class iterator {
	 public:
		iterator():pos(0),ls(NULL) {}
		iterator(int _pos, cqlist<T> *_ls):pos(_pos),ls(_ls){}
		iterator(const iterator& other) {
			pos = other.pos;
			ls = other.ls;
		}
		virtual ~iterator() {}

		iterator & operator = (const iterator& other) {
			pos = other.pos;
			ls = other.ls;
			return *this;
		}

		bool operator==(const iterator& other) {
			return (pos == other.pos && ls == other.ls);
		}

		bool operator!=(const iterator& other) {
			return (!this->operator ==(other));
		}

		//prefix ++
		iterator& operator++() {
			pos ++;
			pos %= ls->cap;
			return *this;
		}

		//postfix ++
		iterator& operator++(int) {
			iterator snapshot(*this);
			pos ++;
			pos %= ls->cap;
			return snapshot;
		}

		//prefix --
		iterator& operator--() {
			pos --;
			if (pos < 0) pos = ls->cap-1;
			return *this;
		}

		//postfix --
		iterator& operator--(int) {
			iterator snapshot(*this);
			pos --;
			if (pos < 0) pos = ls->cap-1;
			return snapshot;
		}

		T& operator*() {
			return ls->elems[pos];
		}

		T& elem() {
			return ls->elems[pos];
		}

	 private:
		size_t pos;
		cqlist<T> *ls;
	};

  friend class iterator;

	iterator begin() {
		return iterator(head, this);
	}

	iterator end() {
    return iterator(tail,this);
	}

 private:
	T* elems;
	size_t cap;
	size_t head;
	size_t tail;
};
#endif /* CQLIST_H_ */
