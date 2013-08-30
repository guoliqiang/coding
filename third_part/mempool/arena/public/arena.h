// Copyright 2000 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Sometimes it is necessary to allocate a large number of small
// objects.  Doing this the usual way (malloc, new) is slow,
// especially for multithreaded programs.  An UnsafeArena provides a
// mark/release method of memory management: it asks for a large chunk
// from the operating system and doles it out bit by bit as required.
// Then you free all the memory at once by calling UnsafeArena::Reset().
// The "Unsafe" refers to the fact that UnsafeArena is not safe to
// call from multiple threads.
//
// The global operator new that can be used as follows:
//
//   #include "lib/arena-inl.h"
//
//   UnsafeArena arena(1000);
//   Foo* foo = new (AllocateInArena, &arena) Foo;
//

#ifndef UTIL_ARENA_H_
#define UTIL_ARENA_H_

#include "base/public/logging.h"
#include <vector>
#include <stdlib.h>

using std::vector;

namespace util {

namespace arena {

// This class is thread-compatible.
class UnsafeArena {
 public:
  UnsafeArena(const size_t block_size);
  virtual ~UnsafeArena();
  void Reset();

  // This should be the worst-case alignment for any type.  This is
  // good for IA-32, SPARC version 7 (the last one I know), and
  // supposedly Alpha.  i386 would be more time-efficient with a
  // default alignment of 8, but ::operator new() uses alignment of 4,
  // and an assertion will fail below after the call to MakeNewBlock()
  // if you try to use a larger alignment.
#ifdef __i386__
  static const int kDefaultAlignment = 4;
#else
  static const int kDefaultAlignment = 8;
#endif

 public:
  void* GetMemory(const size_t size, const int align) {
    // common case
    if ( size > 0 && size < remaining_ && align == 1 ) {
      last_alloc_ = freestart_;
      freestart_ += size;
      remaining_ -= size;
      return reinterpret_cast<void*>(last_alloc_);
    }
    return GetMemoryFallback(size, align);
  }

 private:
  struct AllocatedBlock {
    char *mem;
    size_t size;
  };

 private:
  void* GetMemoryFallback(const size_t size, const int align);

  // The returned AllocatedBlock* is valid until the next call to AllocNewBlock
  // or Reset (i.e. anything that might affect overflow_blocks_).
  AllocatedBlock *AllocNewBlock(const size_t block_size);
  // const AllocatedBlock *IndexToBlock(int index) const;

  // Frees all except first block
  void FreeBlocks();
 
 private:
  const size_t block_size_;
  // beginning of the free space in most recent block
  char* freestart_;
  // beginning of the free space when we're empty
  char* freestart_when_empty_;
  // used to make sure ReturnBytes() is safe
  char* last_alloc_;
  // STL vector isn't as efficient as it could be, so we use an array at first
  size_t remaining_;
  // how many of the first_blocks_ have been alloced
  int blocks_alloced_;
  // the length of this array is arbitrary
  AllocatedBlock first_blocks_[16];
  // if the first_blocks_ aren't enough, expand into overflow_blocks_.
  vector<AllocatedBlock>* overflow_blocks_;

  DISALLOW_COPY_AND_ASSIGN(UnsafeArena);
};


}  // namespace arena
}  // namespace util

// Operators for allocation on the arena
// Syntax: new (AllocateInArena, arena) MyClass;
// STL containers, etc.
inline void* operator new(size_t size,
                          util::arena::UnsafeArena *arena) {
  return reinterpret_cast<char*>(arena->GetMemory(size, 1));
}

#endif  // UTIL_ARENA_H_

