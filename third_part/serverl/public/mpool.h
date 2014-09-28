#ifndef MPOOL_H_
#define MPOOL_H_

#include "cqlist.h"
#include <stdio.h>

template <class T>
class mpool {
 public:
	mpool(int cap) {
		capacity = cap;
		allocCount = 0;
		pool.reset(cap + 10);
	}

	mpool() {
		capacity = CQLIST_DEFAULT_CAP;
		allocCount = 0;
	}

	virtual ~mpool() {}
 public:
	void resize(int cap) {
		capacity = cap;
		allocCount = 0;
		pool.reset(cap+10);
	}

	T* alloc() {
		if (!pool.empty()) {
			T* res = pool.front();
			pool.pop();
			return res;
		} else {
			if (allocCount < capacity) {
				T* res = new T;
				allocCount ++;
				return res;
			} else return NULL;
		}
	}

	bool deAlloc(T* ptr) {
		return pool.push(ptr);
	}

	size_t size() {
		return pool.size();
	}

	size_t getCapacity() {
		return capacity;
	}

	size_t getAllocCount() {
		return allocCount;
	}

private:
	cqlist<T*> pool;
	size_t capacity;
	size_t allocCount;
};

#endif /* MPOOL_H_ */

