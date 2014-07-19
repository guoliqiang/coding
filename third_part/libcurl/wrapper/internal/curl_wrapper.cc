// Copyright 2014 Jike Inc. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-19 05:57:42
// File  : curl_wrapper.cc
// Brief :

#include "../public/curl_wrapper.h"

namespace curl {

CurlWrapper::CurlWrapper(int size /*=5000*/) {
  CHECK((buffer_ = new char[size]) != NULL)
      << "allocate memory error!";
  buffer_size_ = size;
  result_size_ = 0;
  memset(buffer_, 0x00, buffer_size_);
  CURLcode res = curl_global_init(CURL_GLOBAL_ALL);
  CHECK(res == CURLE_OK) << "curl init failed!";
  CHECK((http_curl_ = curl_easy_init()) != NULL)
      << "curl easy init failed!";
}

CurlWrapper::~CurlWrapper() {
  if (http_curl_ != NULL) curl_easy_cleanup(http_curl_);
  if (buffer_ != NULL) delete [] buffer_;
}

bool CurlWrapper::HttpPost(const std::string & url,
    const std::string & data, int64 timeout, std::string * rs) {
  rs->clear();
  if (HttpPost(url, data, timeout) == false) return false;
  std::string tmp(buffer_);
  rs->swap(tmp);
  return true;
}

bool CurlWrapper::HttpPost(const std::string & url,
    const std::string & data, int64 timeout) {
  if (http_head_list_ != NULL) {
    curl_easy_setopt(http_curl_, CURLOPT_HEADER, http_head_list_);
  }
  curl_easy_setopt(http_curl_, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(http_curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(http_curl_, CURLOPT_POST, 1);
  curl_easy_setopt(http_curl_, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(http_curl_, CURLOPT_TIMEOUT, timeout);
  curl_easy_setopt(http_curl_, CURLOPT_WRITEFUNCTION,
      CurlWrapper::HttpCallback);
  curl_easy_setopt(http_curl_, CURLOPT_WRITEDATA, this);
  CURLcode res = curl_easy_perform(http_curl_);
  return CURLE_OK == res ? true : false;
}

bool CurlWrapper::HttpGet(const std::string & url, int64 timeout,
    std::string * rs) {
  rs->clear();
  if (HttpGet(url, timeout) == false) return false;
  std::string tmp(buffer_);
  rs->swap(tmp);
  return true;
}

bool CurlWrapper::HttpGet(const std::string & url, int64 timeout) {
  curl_easy_setopt(http_curl_, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt(http_curl_, CURLOPT_URL, url.c_str());
  curl_easy_setopt(http_curl_, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(http_curl_, CURLOPT_TIMEOUT, timeout);
  curl_easy_setopt(http_curl_, CURLOPT_WRITEFUNCTION,
      CurlWrapper::HttpCallback);
  curl_easy_setopt(http_curl_, CURLOPT_WRITEDATA, this);
  CURLcode res = curl_easy_perform(http_curl_);
  return CURLE_OK == res ? true : false;
}

void CurlWrapper::Reset() {
  if (http_curl_ != NULL) curl_easy_reset(http_curl_);
  result_size_ = 0;
  memset(buffer_, 0x00, buffer_size_);
}

void CurlWrapper::ClearHttp() {
  if (http_curl_ != NULL) curl_easy_cleanup(http_curl_);
  if (buffer_ != NULL) delete [] buffer_;
}

void CurlWrapper::AppendHttpHead(const std::string & buf) {
  http_head_list_ = curl_slist_append(http_head_list_, buf.c_str());
}

int CurlWrapper::HttpCallback(void * contents, int size,
    int nmemb, void * user_ptr) {
  CurlWrapper* mycurl = reinterpret_cast<CurlWrapper*>(user_ptr);
  return mycurl->HttpProcessResult(contents, size, nmemb);
}

int CurlWrapper::HttpProcessResult(void * contents,
  int size, int nmemb) {
  int real_size = size * nmemb;
  CHECK(result_size_ + real_size <= buffer_size_)
      << "buffer is full!";
  memcpy(buffer_ + result_size_, contents, real_size);
  result_size_ += real_size;
  return real_size;
}
}  // namespace curl
