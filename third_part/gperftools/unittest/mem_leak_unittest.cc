// Copyright 2015 Liqiang Guo. All Rights Reserved.
// Author: Liqiang Guo (guoliqiang2006@gmail.com)
// Date  : 2015-01-21 19:18:11
// File  : mem_leark_unittest.cc
// Brief :

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iconv.h>
#include <errno.h>
#include "third_part/gperftools/include/gperftools/heap-checker.h"
#include "base/public/logging.h"
#include "file/public/file.h"

bool ConvertInternal(const std::string & from, const std::string & to,
                     const std::string & content, std::string * out) {
  size_t input_size = content.size();
  char* input_buffer =  new char[input_size];
  CHECK(input_buffer != NULL);
  char* in_buffer = input_buffer;
  memcpy(input_buffer, content.data(), input_size);

  size_t result_buffer_size = input_size * 4;
  char* result_buffer = new char[result_buffer_size];
  CHECK(result_buffer != NULL);
  char* out_buffer = result_buffer;

  iconv_t cd = iconv_open(to.c_str(), from.c_str());
  // fix core
  if (cd == (iconv_t)-1 ||
      iconv(cd, NULL, NULL, NULL, NULL) == size_t(-1)) {
    LOG(WARNING) << "iconv open error!";
    delete[] input_buffer;
    delete[] result_buffer;
    *out = content;
    return false;
  }
  CHECK(cd != (iconv_t)-1);
  CHECK(iconv(cd, NULL, NULL, NULL, NULL) != size_t(-1));
  errno = 0;
  size_t rt = iconv(cd, &in_buffer, &input_size,
                    &out_buffer, &result_buffer_size);
  bool success = false;
  // We ignore invalid bytes.
  if (rt != size_t(-1) || errno == EILSEQ) {
    if (input_size != 0) {
      *out = content;
      LOG(INFO) << "convert error for " << content;
    } else {
      CHECK_EQ(0, input_size);
      out->assign(result_buffer, out_buffer - result_buffer);
      success = true;
    }
  }

  delete[] input_buffer;
  delete[] result_buffer;
  // iconv_close(cd);
  return success;
}

std::string UTF8ToGBK(const std::string & str) {
  std::string rs;
  ConvertInternal("UTF8", "GBK", str, &rs);
  return rs;
}

std::string GBKToUTF8(const std::string & str) {
  std::string rs;
  ConvertInternal("GBK", "UTF8", str, &rs);
  return rs;
}

// http://www.cnblogs.com/chenwenbiao/archive/2011/09/16/2178372.html
bool IsGBKCode(const std::string & str) {
  if (str.size() >= 2) {
     unsigned char ch1 = (unsigned char)str[0];
     unsigned char ch2 = (unsigned char)str[1];
     if (ch1 >= 129 && ch1 <= 254 && ch2 >= 64 && ch2 <= 254) {
       return true;
     }
  }
  return false;
}

bool IsASCIICode(const std::string & str) {
  if (str.size() > 0) {
    unsigned char ch = (unsigned char)str[0];
    if (ch >= 0 && ch <= 127) return true;
  }
  return false;
}

int main(int argc, char** argv) {
  std::string content;
  file::File::ReadFileToString("./gbk.txt", &content);
  LOG(INFO) << GBKToUTF8(content);
  HeapLeakChecker::NoGlobalLeaks();
  return 0;
}
