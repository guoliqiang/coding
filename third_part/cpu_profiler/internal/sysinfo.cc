// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// Copyright (c) 2006, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "third_part/cpu_profiler/public/sysinfo.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/sysctl.h>

static void ConstructFilename(const char* spec, pid_t pid,
                              char* buf, int buf_size) {
  CHECK_LT(snprintf(buf, buf_size, spec,
                    static_cast<int>(pid ? pid : getpid())), buf_size);
}

// Finds |c| in |text|, and assign '\0' at the found position.
// The original character at the modified position should be |c|.
// A pointer to the modified position is stored in |endptr|.
// |endptr| should not be NULL.
static bool ExtractUntilChar(char *text, int c, char **endptr) {
  CHECK(text != NULL);
  CHECK(endptr != NULL);
  char *found;
  found = strchr(text, c);
  if (found == NULL) {
    *endptr = NULL;
    return false;
  }
  *endptr = found;
  *found = '\0';
  return true;
}

// Increments |*text_pointer| while it points a whitespace character.
// It is to follow sscanf's whilespace handling.
static void SkipWhileWhitespace(char **text_pointer, int c) {
  if (isspace(c)) {
    while (isspace(**text_pointer) && isspace(*((*text_pointer) + 1))) {
      ++(*text_pointer);
    }
  }
}

template<class T>
static T StringToInteger(char *text, char **endptr, int base) {
  assert(false);
  return T();
}

template<>
int StringToInteger<int>(char *text, char **endptr, int base) {
  return strtol(text, endptr, base);
}

template<>
int64 StringToInteger<int64>(char *text, char **endptr, int base) {
  return strtoll(text, endptr, base);
}

template<>
uint64 StringToInteger<uint64>(char *text, char **endptr, int base) {
  return strtoull(text, endptr, base);
}

template<typename T>
static T StringToIntegerUntilChar(char *text, int base, int c,
                                  char **endptr_result) {
  CHECK(endptr_result != NULL);
  *endptr_result = NULL;
  char *endptr_extract;
  if (!ExtractUntilChar(text, c, &endptr_extract)) return 0;
  T result;
  char *endptr_strto;
  result = StringToInteger<T>(text, &endptr_strto, base);
  *endptr_extract = c;
  if (endptr_extract != endptr_strto) return 0;
  *endptr_result = endptr_extract;
  SkipWhileWhitespace(endptr_result, c);
  return result;
}

static char *CopyStringUntilChar(char *text, unsigned out_len,
                                 int c, char *out) {
  char *endptr;
  if (!ExtractUntilChar(text, c, &endptr)) return NULL;
  strncpy(out, text, out_len);
  out[out_len-1] = '\0';
  *endptr = c;
  SkipWhileWhitespace(&endptr, c);
  return endptr;
}

template<typename T>
static bool StringToIntegerUntilCharWithCheck(
    T *outptr, char *text, int base, int c, char **endptr) {
  *outptr = StringToIntegerUntilChar<T>(*endptr, base, c, endptr);
  if (*endptr == NULL || **endptr == '\0') return false;
  ++(*endptr);
  return true;
}

static bool ParseProcMapsLine(char *text, uint64 *start, uint64 *end,
                              char *flags, uint64 *offset,
                              int *major, int *minor, int64 *inode,
                              unsigned *filename_offset) {
  // It's similar to:
  // sscanf(text, "%llx-%llx %4s %llx %x:%x %llx %n",
  //        start, end, flags, offset, major, minor, inode, filename_offset)
  char *endptr = text;
  if (endptr == NULL || *endptr == '\0')  return false;
  if (!StringToIntegerUntilCharWithCheck(start, endptr, 16, '-', &endptr)) {
    return false;
  }
  if (!StringToIntegerUntilCharWithCheck(end, endptr, 16, ' ', &endptr)) {
    return false;
  }
  endptr = CopyStringUntilChar(endptr, 5, ' ', flags);
  if (endptr == NULL || *endptr == '\0')  return false;
  ++endptr;
  if (!StringToIntegerUntilCharWithCheck(offset, endptr, 16, ' ', &endptr)) {
    return false;
  }
  if (!StringToIntegerUntilCharWithCheck(major, endptr, 16, ':', &endptr)) {
    return false;
  }
  if (!StringToIntegerUntilCharWithCheck(minor, endptr, 16, ' ', &endptr)) {
    return false;
  }
  if (!StringToIntegerUntilCharWithCheck(inode, endptr, 10, ' ', &endptr)) {
    return false;
  }
  *filename_offset = (endptr - text);
  return true;
}

ProcMapsIterator::ProcMapsIterator(pid_t pid) {
  Init(pid, NULL, false);
}

ProcMapsIterator::ProcMapsIterator(pid_t pid, Buffer *buffer) {
  Init(pid, buffer, false);
}

ProcMapsIterator::ProcMapsIterator(pid_t pid, Buffer *buffer,
                                   bool use_maps_backing) {
  Init(pid, buffer, use_maps_backing);
}

void ProcMapsIterator::Init(pid_t pid, Buffer *buffer, bool use_maps_backing) {
  pid_ = pid;
  using_maps_backing_ = use_maps_backing;
  dynamic_buffer_ = NULL;
  if (!buffer) {
    // If the user didn't pass in any buffer storage, allocate it now. This is
    // the normal case; the signal handler passes in a static buffer.
    buffer = dynamic_buffer_ = new Buffer;
  } else {
    dynamic_buffer_ = NULL;
  }
  ibuf_ = buffer->buf_;
  stext_ = etext_ = nextline_ = ibuf_;
  ebuf_ = ibuf_ + Buffer::kBufSize - 1;
  nextline_ = ibuf_;
  if (use_maps_backing) {
    // don't bother with clever "self" stuff in this case
    ConstructFilename("/proc/%d/maps_backing", pid, ibuf_, Buffer::kBufSize);
  } else if (pid == 0) {
    // We have to kludge a bit to deal with the args ConstructFilename
    // expects.  The 1 is never used -- it's only impt. that it's not 0.
    ConstructFilename("/proc/self/maps", 1, ibuf_, Buffer::kBufSize);
  } else {
    ConstructFilename("/proc/%d/maps", pid, ibuf_, Buffer::kBufSize);
  }
  // No error logging since this can be called from the crash dump
  // handler at awkward moments. Users should call Valid() before
  // using.
  NO_INTR(fd_ = open(ibuf_, O_RDONLY));
}

ProcMapsIterator::~ProcMapsIterator() {
  if (fd_ >= 0) NO_INTR(close(fd_));
  delete dynamic_buffer_;
}

bool ProcMapsIterator::Valid() const {
  return fd_ != -1;
}

bool ProcMapsIterator::Next(uint64 *start, uint64 *end, char **flags,
                            uint64 *offset, int64 *inode, char **filename) {
  return NextExt(start, end, flags, offset, inode, filename, NULL, NULL,
                 NULL, NULL, NULL);
}

// This has too many arguments.  It should really be building
// a map object and returning it.  The problem is that this is called
// when the memory allocator state is undefined, hence the arguments.
bool ProcMapsIterator::NextExt(uint64 *start, uint64 *end, char **flags,
                               uint64 *offset, int64 *inode, char **filename,
                               uint64 *file_mapping, uint64 *file_pages,
                               uint64 *anon_mapping, uint64 *anon_pages,
                               dev_t *dev) {
  do {
    // Advance to the start of the next line
    stext_ = nextline_;
    // See if we have a complete line in the buffer already
    nextline_ = static_cast<char *>(memchr(stext_, '\n', etext_ - stext_));
    if (!nextline_) {
      // Shift/fill the buffer so we do have a line
      int count = etext_ - stext_;
      // Move the current text to the start of the buffer
      memmove(ibuf_, stext_, count);
      stext_ = ibuf_;
      etext_ = ibuf_ + count;
      int nread = 0;  // fill up buffer with text
      while (etext_ < ebuf_) {
        NO_INTR(nread = read(fd_, etext_, ebuf_ - etext_));
        if (nread > 0) {
          etext_ += nread;
        } else {
          break;
        }
      }

      // Zero out remaining characters in buffer at EOF to avoid returning
      // garbage from subsequent calls.
      if (etext_ != ebuf_ && nread == 0) {
        memset(etext_, 0, ebuf_ - etext_);
      }
      // sentinel; safe because ibuf extends 1 char beyond ebuf
      *etext_ = '\n';
      nextline_ =
          static_cast<char *>(memchr(stext_, '\n', etext_ + 1 - stext_));
    }
    // turn newline into nul
    *nextline_ = 0;
    // skip nul if not end of text
    nextline_ += ((nextline_ < etext_)? 1 : 0);
    // stext_ now points at a nul-terminated line
    uint64 tmpstart, tmpend, tmpoffset;
    int64 tmpinode;
    int major, minor;
    unsigned filename_offset = 0;
    // for now, assume all linuxes have the same format
    if (!ParseProcMapsLine(
        stext_,
        start ? start : &tmpstart,
        end ? end : &tmpend,
        flags_,
        offset ? offset : &tmpoffset,
        &major, &minor,
        inode ? inode : &tmpinode, &filename_offset)) continue;

    // Depending on the Linux kernel being used, there may or may not be a space
    // after the inode if there is no filename.  sscanf will in such situations
    // nondeterministically either fill in filename_offset or not (the results
    // differ on multiple calls in the same run even with identical arguments).
    // We don't want to wander off somewhere beyond the end of the string.
    size_t stext_length = strlen(stext_);
    if (filename_offset == 0 || filename_offset > stext_length) {
      filename_offset = stext_length;
    }
    // We found an entry
    if (flags) *flags = flags_;
    if (filename) *filename = stext_ + filename_offset;
    if (dev) *dev = minor | (major << 8);

    if (using_maps_backing_) {
      // Extract and parse physical page backing info.
      char *backing_ptr = stext_ + filename_offset +
                          strlen(stext_+filename_offset);
      // find the second '('
      int paren_count = 0;
      while (--backing_ptr > stext_) {
        if (*backing_ptr == '(') {
          ++paren_count;
          if (paren_count >= 2) {
            uint64 tmp_file_mapping;
            uint64 tmp_file_pages;
            uint64 tmp_anon_mapping;
            uint64 tmp_anon_pages;
            sscanf(backing_ptr + 1, "F %llx %lld ) (A %llx  %lld )",  // NOLINT
                   file_mapping ?
                   (long long unsigned int *)file_mapping :  // NOLINT
                   (long long unsigned int *)&tmp_file_mapping,  // NOLINT
                   file_pages ?
                   (long long int *)file_pages :  // NOLINT
                   (long long int *)&tmp_file_pages,  // NOLINT
                   anon_mapping ?
                   (long long unsigned int * )anon_mapping :  // NOLINT
                   (long long unsigned int *)&tmp_anon_mapping,  // NOLINT
                   anon_pages ?
                   (long long int *)anon_pages :  // NOLINT
                   (long long int *)&tmp_anon_pages);  // NOLINT
            // null terminate the file name (there is a space
            // before the first (.
            backing_ptr[-1] = 0;
            break;
          }
        }
      }
    }

    return true;
  } while (etext_ > ibuf_);
  // We didn't find anything
  return false;
}

int ProcMapsIterator::FormatLine(char* buffer, int bufsize, uint64 start,
                                 uint64 end, const char *flags,
                                 uint64 offset, int64 inode,
                                 const char *filename, dev_t dev) {
  // We assume 'flags' looks like 'rwxp' or 'rwx'.
  char r = (flags && flags[0] == 'r') ? 'r' : '-';
  char w = (flags && flags[0] && flags[1] == 'w') ? 'w' : '-';
  char x = (flags && flags[0] && flags[1] && flags[2] == 'x') ? 'x' : '-';
  // p always seems set on linux, so we set the default to 'p', not '-'
  char p = (flags && flags[0] && flags[1] && flags[2] && flags[3] != 'p') ?
           '-' : 'p';

  const int rc = snprintf(buffer, bufsize,
                 "%08llx-%08llx %c%c%c%c %08llx %02x:%02x %-11llx %s\n",
                 (long long unsigned int)start,  // NOLINT
                 (long long unsigned int)end,  // NOLINT
                 r, w, x, p,
                 (long long unsigned int)offset,  // NOLINT
                 static_cast<int>(dev / 256),
                 static_cast<int>(dev % 256),
                 (long long unsigned int)inode,  // NOLINT
                 filename);
  return (rc < 0 || rc >= bufsize) ? 0 : rc;
}
