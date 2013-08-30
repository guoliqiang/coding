// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: leilu@yunrang.com(Lei Lu)

#include <string>
#include <iconv.h>
#include <errno.h>
#include "testing/gtest/include/gtest/gtest.h"
#include "util/i18n/encoding.h"
#include "util/i18n/language.h"
#include "util/i18n/yrencoding_detector.h"
#include "util/i18n/yrlanguage_detector.h"
#include "file/file.h"
#include "base/logging.h"
#include "base/flags.h"

DEFINE_string(html, "", "input html file path");

int main(int argc, char** argv) {
  base::ParseCommandLineFlags(&argc, &argv, true);

  std::string content;
  file::File::ReadFileToStringOrDie(FLAGS_html, &content);
  LOG(INFO) << "content size:" << content.length();
  i18n::YREncodingDetector detector;
  i18n::Encoding encoding = detector.DetectEncoding(
        reinterpret_cast<const uint8*>(content.c_str()), content.size());

  // create iconv_t
  iconv_t cd;
  memset(&cd, 0, sizeof(cd));
  if (encoding == i18n::GB2312) {
    cd = iconv_open("utf-8//IGNORE", "gb2312");
    LOG(INFO) << "GB2312";
  } else if (encoding == i18n::GBK) {
    cd = iconv_open("utf-8//IGNORE", "gbk");
    LOG(INFO) << "GBK";
  } else if (encoding == i18n::BIG5) {
    cd = iconv_open("utf-8//IGNORE", "big5");
    LOG(INFO) << "BIG5";
  } else if (encoding == i18n::UTF8) {
    LOG(INFO) << "UTF8";
  } else {
    LOG(INFO) << encoding;
    return 0;
  }

  if (encoding != i18n::UTF8)  {
    size_t input_size = content.size();
    char* input_buffer =  new char[input_size];
    CHECK(input_buffer != NULL);
    char* in_buffer = input_buffer;
    memcpy(input_buffer, content.data(), input_size);

    size_t result_buffer_size = input_size * 4;
    char* result_buffer = new char[result_buffer_size];
    CHECK(result_buffer != NULL);
    char* out_buffer = result_buffer;

    if (iconv(cd, NULL, NULL, NULL, NULL) == size_t(-1)) {
      LOG(INFO) << "***Failed iconv!(1)***";
      return 0;
    }

    size_t rt = iconv(cd,
                      &in_buffer,
                      &input_size,
                      &out_buffer,
                      &result_buffer_size);

    if (rt != size_t(-1) || errno == EILSEQ) {
      // LOG(INFO) << result_buffer;
      content.assign(result_buffer);
    } else {
      switch (errno) {
        // See "man 3 iconv" for an explanation.
        case EILSEQ:
          fprintf (stderr, "Invalid multibyte sequence.\n");
          break;
        case EINVAL:
          fprintf (stderr, "Incomplete multibyte sequence.\n");
          break;
        case E2BIG:
          fprintf (stderr, "No more room.\n");
          break;
        default:
          fprintf (stderr, "Error: %s.\n", strerror (errno));
      }
    }

    delete[] input_buffer;
    delete[] result_buffer;
  }

  i18n::YRLanguageDetector language_detector;
  i18n::Language language = language_detector.DetectLanguage(
      reinterpret_cast<const uint8*>(content.c_str()), content.size());
  LOG(INFO) << i18n::LanguageType2String(language);
  LOG(INFO) << language;
  language = language_detector.DetectLanguageForUTF8(
      reinterpret_cast<const uint8*>(content.c_str()), content.size());
  LOG(INFO) << i18n::LanguageType2String(language);
  LOG(INFO) << language;

  return false;
}
