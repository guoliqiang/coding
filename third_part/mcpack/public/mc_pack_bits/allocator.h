#ifndef __ALLOCATOR__
#define __ALLOCATOR__

#include "typelist.h"
#include <string.h>
#include "base/public/logging.h"

class reverse_allocator {
  size_t buf_size;
  char * free_space;
  void * freelist[10];
 public:
  reverse_allocator(char * buffer, size_t size) {
    buf_size = size;
    free_space = buffer + size;
    for (size_t i = 0; i < sizeof(freelist) / sizeof(freelist[0]); i++) {
      freelist[i] = 0;
    }
  }

  inline void * malloc(u_int size) {
    size = (size + 7) & (-1 << 3);
    if(size <= buf_size) {
      buf_size -= size;
      free_space -= size;
      return free_space;
    } else return 0;
  }

  template<typename T>
  inline T * malloc() {
    const unsigned int index = (sizeof(T) + 7) / 8;
    if (index < sizeof(freelist)/sizeof(freelist[0]) &&
        freelist[index] != 0) {
      VLOG(3) << "malloc from freelist!";
      T * tmp = (T *)freelist[index];
      VLOG(3) << "malloc memory address:" << tmp;
      freelist[index] = *(T * *)freelist[index];
      VLOG(3) << "array index:" << index << " is set as:" << freelist[index];
      return tmp;
    } else {
      VLOG(3) << "normal malloc!";
      return (T *)malloc(sizeof(T));
    }
  }
  /*
   * 利用数据块的开始位置构建了一个列表
   * */
  template<typename T>
  inline void free(T * ptr) {
    VLOG(3) << "want to free " << ptr;
    const unsigned int index = (sizeof(T) + 7) / 8;
    if(index < sizeof(freelist) / sizeof(freelist[0])) {
      T * tmp = (T *)freelist[index];
      VLOG(3) << "array index:" << index << " before value:" << tmp;
      freelist[index] = (T *)ptr;
      VLOG(3) << "array index:" << index << "now value:" << freelist[index];
      *(T * *)ptr = tmp;
      VLOG(3) << "now ptr value is :" << ptr;
    } else {
      VLOG(3) << "not do any thing for :" << ptr;
    }
  }
};

template <u_int seg_size>
class mempool_allocator {
  size_t buf_size;
  char * free_space;
  void * memlist;
 public:
  template <typename T>
  inline T *malloc() {
    return (T *)malloc(sizeof(T));
  }
  inline void * malloc(u_int size) {
    size = (size + 7 ) & ( -1 << 3);
    if(size < seg_size - sizeof(void *)) {
      if(size > buf_size) {
        void *tmp = ::malloc(seg_size);
        if(tmp) {
          *(void **)tmp = memlist;
          memlist = tmp;
          free_space = (char *)tmp;
          buf_size = seg_size - sizeof(void *);
        } else
          return 0;
      }
      buf_size -= size;
      free_space += size;
      return free_space;
    } else {
      void *tmp = ::malloc(size + sizeof(void *));
      if(tmp) {
        *(void **)tmp = memlist;
        memlist = tmp;
        return ((char *)tmp + sizeof(void *));
      } else
        return 0;

    }
  }
  template<typename T>
  inline void free( T * ptr) {}
  mempool_allocator() {
    buf_size = 0;
    memlist = 0;
    free_space = 0;
  }
  ~mempool_allocator() {
    while(memlist) {
      void * tmp = *(void **)memlist;
      free(memlist);
      memlist = tmp;
    }
  }
};

template<typename types>
class typed_allocator {
  size_t buf_size;
  char * free_space;
  void * freelist[types::length];
 public:
  typed_allocator(int size):buf_size(size) {
    free_space = (char *)this + sizeof(*this);
    for(int i = 0; i< sizeof(freelist) / sizeof(freelist[0]); i++) {
      freelist[i] = 0;
    }
    memset(free_space, 0, buf_size - sizeof(*this));
  }

  inline void * malloc(int size) {
    size = (size + 7) & (-1 << 3);
    if((size_t)(free_space - (char *)this + size) < buf_size) {
      void * tmp = free_space;
      free_space += size;
      return tmp;
    } else return 0;
  }
  template<typename T>
  T * malloc() {
    if(freelist[index_of<types, T>::value] != 0) {
      T * tmp = (T *)freelist[index_of<types, T>::value];
      freelist[index_of<types, T>::value] = *(T * *)freelist[index_of<types, T>::value];
      return tmp;
    } else return (T *)malloc(sizeof(T));
  }
  template<typename T>
  void free( T * ptr) {
    T * tmp = (T *)freelist[index_of<types, T>::value];
    freelist[index_of<types, T>::value] = (T *)ptr;
    *(T * * )ptr = tmp;
  }
};
#endif
