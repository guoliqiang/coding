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
#include "third_part/cpu_profiler/public/cycleclock.h"

#define SCNx64 "llx"
#define SCNd64 "lld"
#define PRIx64 "llx"
#define PRId64 "lld"

// Re-run fn until it doesn't cause EINTR.
#define NO_INTR(fn)  do {} while ((fn) < 0 && errno == EINTR)

// open/read/close can set errno, which may be illegal at this
// time, so prefer making the syscalls directly if we can.

# define safeopen(filename, mode)  open(filename, mode)
# define saferead(fd, buffer, size)  read(fd, buffer, size)
# define safeclose(fd)  close(fd)

// ----------------------------------------------------------------------
// GetenvBeforeMain()
// GetUniquePathFromEnv()
//    Some non-trivial getenv-related functions.
// ----------------------------------------------------------------------

// It's not safe to call getenv() in the malloc hooks, because they
// might be called extremely early, before libc is done setting up
// correctly.  In particular, the thread library may not be done
// setting up errno.  So instead, we use the built-in __environ array
// if it exists, and otherwise read /proc/self/environ directly, using
// system calls to read the file, and thus avoid setting errno.
// /proc/self/environ has a limit of how much data it exports (around
// 8K), so it's not an ideal solution.
const char* GetenvBeforeMain(const char* name) {
#if defined(HAVE___ENVIRON)   // if we have it, it's declared in unistd.h
  if (__environ) {            // can exist but be NULL, if statically linked
    const int namelen = strlen(name);
    for (char** p = __environ; *p; p++) {
      if (strlen(*p) < namelen) {
        continue;
      }
      if (!memcmp(*p, name, namelen) && (*p)[namelen] == '=')  // it's a match
        return *p + namelen+1;                                 // point after =
    }
    return NULL;
  }
#endif
  // static is ok because this function should only be called before
  // main(), when we're single-threaded.
  static char envbuf[16<<10];
  if (*envbuf == '\0') {    // haven't read the environ yet
    int fd = safeopen("/proc/self/environ", O_RDONLY);
    // The -2 below guarantees the last two bytes of the buffer will be \0\0
    if (fd == -1 ||           // unable to open the file, fall back onto libc
        saferead(fd, envbuf, sizeof(envbuf) - 2) < 0) {  // error reading file
      LOG(INFO) << "Unable to open /proc/self/environ, falling back "
                   "on getenv(), which may not work" << name;
      if (fd != -1) safeclose(fd);
      return getenv(name);
    }
    safeclose(fd);
  }
  const int namelen = strlen(name);
  const char* p = envbuf;
  while (*p != '\0') {    // will happen at the \0\0 that terminates the buffer
    // proc file has the format NAME=value\0NAME=value\0NAME=value\0...
    const char* endp = reinterpret_cast<const char *>(
        memchr(p, '\0', sizeof(envbuf) - (p - envbuf)));
    if (endp == NULL)  // this entry isn't NUL terminated
      return NULL;
    else if (!memcmp(p, name, namelen) && p[namelen] == '=')  // it's a match
      return p + namelen + 1;  // point after =
    p = endp + 1;
  }
  return NULL;  // env var never found
}

const char* TCMallocGetenvSafe(const char* name) {
  return GetenvBeforeMain(name);
}

// This takes as an argument an environment-variable name (like
// CPUPROFILE) whose value is supposed to be a file-path, and sets
// path to that path, and returns true.  If the env var doesn't exist,
// or is the empty string, leave path unchanged and returns false.
// The reason this is non-trivial is that this function handles munged
// pathnames.  Here's why:
//
// If we're a child process of the 'main' process, we can't just use
// getenv("CPUPROFILE") -- the parent process will be using that path.
// Instead we append our pid to the pathname.  How do we tell if we're a
// child process?  Ideally we'd set an environment variable that all
// our children would inherit.  But -- and this is seemingly a bug in
// gcc -- if you do a setenv() in a shared libarary in a global
// constructor, the environment setting is lost by the time main() is
// called.  The only safe thing we can do in such a situation is to
// modify the existing envvar.  So we do a hack: in the parent, we set
// the high bit of the 1st char of CPUPROFILE.  In the child, we
// notice the high bit is set and append the pid().  This works
// assuming cpuprofile filenames don't normally have the high bit set
// in their first character!  If that assumption is violated, we'll
// still get a profile, but one with an unexpected name.
// TODO(csilvers): set an envvar instead when we can do it reliably.
bool GetUniquePathFromEnv(const char* env_name, char* path) {
  char* envval = getenv(env_name);
  if (envval == NULL || *envval == '\0')
    return false;
  if (envval[0] & 128) {                  // high bit is set
    snprintf(path, PATH_MAX, "%c%s_%u",   // add pid and clear high bit
             envval[0] & 127, envval+1, (unsigned int)(getpid()));
  } else {
    snprintf(path, PATH_MAX, "%s", envval);
    envval[0] |= 128;                     // set high bit for kids to see
  }
  return true;
}

// ----------------------------------------------------------------------
// CyclesPerSecond()
// NumCPUs()
//    It's important this not call malloc! -- they may be called at
//    global-construct time, before we've set up all our proper malloc
//    hooks and such.
// ----------------------------------------------------------------------

static double cpuinfo_cycles_per_second = 1.0;  // 0.0 might be dangerous
static int cpuinfo_num_cpus = 1;  // Conservative guess

void SleepForMilliseconds(int milliseconds) {
  // Sleep for a few milliseconds
  struct timespec sleep_time;
  sleep_time.tv_sec = milliseconds / 1000;
  sleep_time.tv_nsec = (milliseconds % 1000) * 1000000;
  while (nanosleep(&sleep_time, &sleep_time) != 0 && errno == EINTR);
  // Ignore signals and wait for the full interval to elapse.
}

// Helper function estimates cycles/sec by observing cycles elapsed during
// sleep(). Using small sleep time decreases accuracy significantly.
static int64 EstimateCyclesPerSecond(const int estimate_time_ms) {
  assert(estimate_time_ms > 0);
  if (estimate_time_ms <= 0)
    return 1;
  double multiplier =
      1000.0 / static_cast<double>(estimate_time_ms);  // scale by this much

  const int64 start_ticks = CycleClock::Now();
  SleepForMilliseconds(estimate_time_ms);
  const int64 guess =
      static_cast<int64>(multiplier * (CycleClock::Now() - start_ticks));
  return guess;
}

static bool ReadIntFromFile(const char *file, int *value) {
  bool ret = false;
  int fd = open(file, O_RDONLY);
  if (fd != -1) {
    char line[1024];
    char* err;
    memset(line, '\0', sizeof(line));
    read(fd, line, sizeof(line) - 1);
    const int temp_value = strtol(line, &err, 10);
    if (line[0] != '\0' && (*err == '\n' || *err == '\0')) {
      *value = temp_value;
      ret = true;
    }
    close(fd);
  }
  return ret;
}

// WARNING: logging calls back to InitializeSystemInfo() so it must
// not invoke any logging code.  Also, InitializeSystemInfo() can be
// called before main() -- in fact it *must* be since already_called
// isn't protected -- before malloc hooks are properly set up, so
// we make an effort not to call any routines which might allocate
// memory.

static void InitializeSystemInfo() {
  static bool already_called = false;   // safe if we run before threads
  if (already_called)  return;
  already_called = true;
  bool saw_mhz = false;

  char line[1024];
  char* err;
  int freq;

  // If the kernel is exporting the tsc frequency use that. There are issues
  // where cpuinfo_max_freq cannot be relied on because the BIOS may be
  // exporintg an invalid p-state (on x86) or p-states may be used to put the
  // processor in a new mode (turbo mode). Essentially, those frequencies
  // cannot always be relied upon. The same reasons apply to /proc/cpuinfo as
  // well.
  if (!saw_mhz &&
      ReadIntFromFile("/sys/devices/system/cpu/cpu0/tsc_freq_khz", &freq)) {
      // The value is in kHz (as the file name suggests).  For example, on a
      // 2GHz warpstation, the file contains the value "2000000".
      cpuinfo_cycles_per_second = freq * 1000.0;
      saw_mhz = true;
  }

  // If CPU scaling is in effect, we want to use the *maximum* frequency,
  // not whatever CPU speed some random processor happens to be using now.
  if (!saw_mhz &&
      ReadIntFromFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq",
                      &freq)) {
    // The value is in kHz.  For example, on a 2GHz machine, the file
    // contains the value "2000000".
    cpuinfo_cycles_per_second = freq * 1000.0;
    saw_mhz = true;
  }

  // Read /proc/cpuinfo for other values, and if there is no cpuinfo_max_freq.
  const char* pname = "/proc/cpuinfo";
  int fd = open(pname, O_RDONLY);
  if (fd == -1) {
    perror(pname);
    if (!saw_mhz) {
      cpuinfo_cycles_per_second = EstimateCyclesPerSecond(1000);
    }
    return;
  }

  double bogo_clock = 1.0;
  bool saw_bogo = false;
  int num_cpus = 0;
  line[0] = line[1] = '\0';
  int chars_read = 0;
  do {   // we'll exit when the last read didn't read anything
    // Move the next line to the beginning of the buffer
    const int oldlinelen = strlen(line);
    if (sizeof(line) == oldlinelen + 1)    // oldlinelen took up entire line
      line[0] = '\0';
    else                                   // still other lines left to save
      memmove(line, line + oldlinelen+1, sizeof(line) - (oldlinelen+1));
    // Terminate the new line, reading more if we can't find the newline
    char* newline = strchr(line, '\n');
    if (newline == NULL) {
      const int linelen = strlen(line);
      const int bytes_to_read = sizeof(line)-1 - linelen;
      assert(bytes_to_read > 0);  // because the memmove recovered >=1 bytes
      chars_read = read(fd, line + linelen, bytes_to_read);
      line[linelen + chars_read] = '\0';
      newline = strchr(line, '\n');
    }
    if (newline != NULL) *newline = '\0';

    // When parsing the "cpu MHz" and "bogomips" (fallback) entries, we only
    // accept postive values. Some environments (virtual machines) report zero,
    // which would cause infinite looping in WallTime_Init.
    if (!saw_mhz && strncasecmp(line, "cpu MHz", sizeof("cpu MHz")-1) == 0) {
      const char* freqstr = strchr(line, ':');
      if (freqstr) {
        cpuinfo_cycles_per_second = strtod(freqstr+1, &err) * 1000000.0;
        if (freqstr[1] != '\0' && *err == '\0' && cpuinfo_cycles_per_second > 0)
          saw_mhz = true;
      }
    } else if (strncasecmp(line, "bogomips", sizeof("bogomips")-1) == 0) {
      const char* freqstr = strchr(line, ':');
      if (freqstr) {
        bogo_clock = strtod(freqstr+1, &err) * 1000000.0;
        if (freqstr[1] != '\0' && *err == '\0' && bogo_clock > 0)
          saw_bogo = true;
      }
    } else if (strncasecmp(line, "processor", sizeof("processor")-1) == 0) {
      num_cpus++;  // count up every time we see an "processor :" entry
    }
  } while (chars_read > 0);
  close(fd);

  if (!saw_mhz) {
    if (saw_bogo) {
      // If we didn't find anything better, we'll use bogomips, but
      // we're not happy about it.
      cpuinfo_cycles_per_second = bogo_clock;
    } else {
      // If we don't even have bogomips, we'll use the slow estimation.
      cpuinfo_cycles_per_second = EstimateCyclesPerSecond(1000);
    }
  }
  if (cpuinfo_cycles_per_second == 0.0) {
    cpuinfo_cycles_per_second = 1.0;   // maybe unnecessary, but safe
  }
  if (num_cpus > 0) {
    cpuinfo_num_cpus = num_cpus;
  }
}

double CyclesPerSecond(void) {
  InitializeSystemInfo();
  return cpuinfo_cycles_per_second;
}

int NumCPUs(void) {
  InitializeSystemInfo();
  return cpuinfo_num_cpus;
}

// ----------------------------------------------------------------------
// HasPosixThreads()
//      Return true if we're running POSIX (e.g., NPTL on Linux)
//      threads, as opposed to a non-POSIX thread library.  The thing
//      that we care about is whether a thread's pid is the same as
//      the thread that spawned it.  If so, this function returns
//      true.
// ----------------------------------------------------------------------
bool HasPosixThreads() {
#ifndef _CS_GNU_LIBPTHREAD_VERSION
#define _CS_GNU_LIBPTHREAD_VERSION 3
#endif
  char buf[32];
  //  We assume that, if confstr() doesn't know about this name, then
  //  the same glibc is providing LinuxThreads.
  if (confstr(_CS_GNU_LIBPTHREAD_VERSION, buf, sizeof(buf)) == 0)
    return false;
  return strncmp(buf, "NPTL", 4) == 0;
}

// ----------------------------------------------------------------------

static void ConstructFilename(const char* spec, pid_t pid,
                              char* buf, int buf_size) {
  CHECK_LT(snprintf(buf, buf_size,
                    spec,
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
static T StringToIntegerUntilChar(
    char *text, int base, int c, char **endptr_result) {
  CHECK(endptr_result != NULL);
  *endptr_result = NULL;

  char *endptr_extract;
  if (!ExtractUntilChar(text, c, &endptr_extract))
    return 0;

  T result;
  char *endptr_strto;
  result = StringToInteger<T>(text, &endptr_strto, base);
  *endptr_extract = c;

  if (endptr_extract != endptr_strto)
    return 0;

  *endptr_result = endptr_extract;
  SkipWhileWhitespace(endptr_result, c);

  return result;
}

static char *CopyStringUntilChar(
    char *text, unsigned out_len, int c, char *out) {
  char *endptr;
  if (!ExtractUntilChar(text, c, &endptr))
    return NULL;

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
  /*
   * It's similar to:
   * sscanf(text, "%"SCNx64"-%"SCNx64" %4s %"SCNx64" %x:%x %"SCNd64" %n",
   *        start, end, flags, offset, major, minor, inode, filename_offset)
   */
  char *endptr = text;
  if (endptr == NULL || *endptr == '\0')  return false;

  if (!StringToIntegerUntilCharWithCheck(start, endptr, 16, '-', &endptr))
    return false;

  if (!StringToIntegerUntilCharWithCheck(end, endptr, 16, ' ', &endptr))
    return false;

  endptr = CopyStringUntilChar(endptr, 5, ' ', flags);
  if (endptr == NULL || *endptr == '\0')  return false;
  ++endptr;

  if (!StringToIntegerUntilCharWithCheck(offset, endptr, 16, ' ', &endptr))
    return false;

  if (!StringToIntegerUntilCharWithCheck(major, endptr, 16, ':', &endptr))
    return false;

  if (!StringToIntegerUntilCharWithCheck(minor, endptr, 16, ' ', &endptr))
    return false;

  if (!StringToIntegerUntilCharWithCheck(inode, endptr, 10, ' ', &endptr))
    return false;

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

void ProcMapsIterator::Init(pid_t pid, Buffer *buffer,
                            bool use_maps_backing) {
  pid_ = pid;
  using_maps_backing_ = use_maps_backing;
  dynamic_buffer_ = NULL;
  if (!buffer) {
    // If the user didn't pass in any buffer storage, allocate it
    // now. This is the normal case; the signal handler passes in a
    // static buffer.
    buffer = dynamic_buffer_ = new Buffer;
  } else {
    dynamic_buffer_ = NULL;
  }

  ibuf_ = buffer->buf_;

  stext_ = etext_ = nextline_ = ibuf_;
  ebuf_ = ibuf_ + Buffer::kBufSize - 1;
  nextline_ = ibuf_;

  if (use_maps_backing) {  // don't bother with clever "self" stuff in this case
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

      int nread = 0;            // fill up buffer with text
      while (etext_ < ebuf_) {
        NO_INTR(nread = read(fd_, etext_, ebuf_ - etext_));
        if (nread > 0)
          etext_ += nread;
        else
          break;
      }

      // Zero out remaining characters in buffer at EOF to avoid returning
      // garbage from subsequent calls.
      if (etext_ != ebuf_ && nread == 0) {
        memset(etext_, 0, ebuf_ - etext_);
      }
      *etext_ = '\n';  // sentinel; safe because ibuf extends 1 char beyond ebuf
      nextline_ =
          static_cast<char *>(memchr(stext_, '\n', etext_ + 1 - stext_));
    }
    *nextline_ = 0;                // turn newline into nul
    nextline_ += ((nextline_ < etext_)? 1 : 0);  // skip nul if not end of text
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
    if (filename_offset == 0 || filename_offset > stext_length)
      filename_offset = stext_length;

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
            unsigned long long tmp_file_mapping;  // NOLINT
            unsigned long long tmp_file_pages;  // NOLINT
            unsigned long long tmp_anon_mapping;  // NOLINT
            unsigned long long tmp_anon_pages;  // NOLINT

            sscanf(backing_ptr + 1, "F %llx %lld ) (A %llx  %lld )",  // NOLINT
                   file_mapping ?
                   (long long unsigned int *)file_mapping : (long long unsigned int *)&tmp_file_mapping,  // NOLINT
                   file_pages ?
                   (long long int *)file_pages : (long long int *)&tmp_file_pages,  // NOLINT
                   anon_mapping ?
                   (long long unsigned int * )anon_mapping : (long long unsigned int *)&tmp_anon_mapping,  // NOLINT
                   anon_pages ?
                   (long long int *)anon_pages : (long long int *)&tmp_anon_pages);  // NOLINT
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

int ProcMapsIterator::FormatLine(char* buffer,
                                 int bufsize,
                                 long long unsigned int start,  // NOLINT
                                 long long unsigned int end,  // NOLINT
                                 const char *flags,
                                 long long unsigned int offset,  // NOLINT
                                 long long int inode,  // NOLINT
                                 const char *filename,
                                 dev_t dev) {
  // We assume 'flags' looks like 'rwxp' or 'rwx'.
  char r = (flags && flags[0] == 'r') ? 'r' : '-';
  char w = (flags && flags[0] && flags[1] == 'w') ? 'w' : '-';
  char x = (flags && flags[0] && flags[1] && flags[2] == 'x') ? 'x' : '-';
  // p always seems set on linux, so we set the default to 'p', not '-'
  char p = (flags && flags[0] && flags[1] && flags[2] && flags[3] != 'p')
      ? '-' : 'p';

  const int rc = snprintf(buffer, bufsize,
                          "%08" PRIx64 "-%08" PRIx64 " %c%c%c%c %08" PRIx64
                          " %02x:%02x %-11" PRId64 " %s\n",
                          start, end, r, w, x, p, offset,
                          static_cast<int>(dev/256), static_cast<int>(dev%256),
                          inode, filename);
  return (rc < 0 || rc >= bufsize) ? 0 : rc;
}

namespace tcmalloc {

// Helper to add the list of mapped shared libraries to a profile.
// Fill formatted "/proc/self/maps" contents into buffer 'buf' of size 'size'
// and return the actual size occupied in 'buf'.  We fill wrote_all to true
// if we successfully wrote all proc lines to buf, false else.
// We do not provision for 0-terminating 'buf'.
int FillProcSelfMaps(char buf[], int size, bool* wrote_all) {
  ProcMapsIterator::Buffer iterbuf;
  ProcMapsIterator it(0, &iterbuf);   // 0 means "current pid"

  uint64 start, end, offset;
  int64 inode;
  char *flags, *filename;
  int bytes_written = 0;
  *wrote_all = true;
  while (it.Next(&start, &end, &flags, &offset, &inode, &filename)) {
    const int line_length = it.FormatLine(buf + bytes_written,
                                          size - bytes_written,
                                          start, end, flags, offset,
                                          inode, filename, 0);
    if (line_length == 0)
      *wrote_all = false;     // failed to write this line out
    else
      bytes_written += line_length;
  }
  return bytes_written;
}

#define NO_INTR(fn)  do {} while ((fn) < 0 && errno == EINTR)

void RawWrite(int fd, const char* buf, size_t len) {
  while (len > 0) {
    ssize_t r;
    NO_INTR(r = write(fd, buf, len));
    if (r <= 0) break;
    buf += r;
    len -= r;
  }
}
// Dump the same data as FillProcSelfMaps reads to fd.
// It seems easier to repeat parts of FillProcSelfMaps here than to
// reuse it via a call.
void DumpProcSelfMaps(int fd) {
  ProcMapsIterator::Buffer iterbuf;
  ProcMapsIterator it(0, &iterbuf);   // 0 means "current pid"

  uint64 start, end, offset;
  int64 inode;
  char *flags, *filename;
  ProcMapsIterator::Buffer linebuf;
  while (it.Next(&start, &end, &flags, &offset, &inode, &filename)) {
    int written = it.FormatLine(linebuf.buf_, sizeof(linebuf.buf_),
                                start, end, flags, offset, inode, filename,
                                0);
    RawWrite(fd, linebuf.buf_, written);
  }
}
}  // namespace tcmalloc
