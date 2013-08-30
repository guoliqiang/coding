// Copyright 2011 Yunrang Inc. All Rights Reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "third_part/http_client/public/curl_wrapper.h"
#include "third_part/libcurl/include/curl/curl.h"
#include "base/public/logging.h"

using std::string;

namespace util {

// copy from  curl sample "getinmemory.c"

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *ptr, size_t size,
                                  size_t nmemb, void *data) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *) data;
  mem->memory = reinterpret_cast<char*>(
                realloc(mem->memory, mem->size + realsize + 1));
  if (mem->memory == NULL) {
    LOG(ERROR) << "not enough memory (realloc returned NULL)";
    exit(EXIT_FAILURE);
  }
  memcpy(&(mem->memory[mem->size]), ptr, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

int CurlWrapper::GetRespondData(const string& url,
                                string* out,
                                string * err_info) {
  CURL *curl_handle;
  char curl_errbuf[CURL_ERROR_SIZE];
  int err;
  struct MemoryStruct chunk;

  chunk.memory = reinterpret_cast<char*> (malloc(1));
  chunk.size = 0;

  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init();

  // specify URL to get
  curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl_handle, CURLOPT_ERRORBUFFER, curl_errbuf);
  // send all data to this function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  // we pass our 'chunk' struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA,
                   reinterpret_cast<void *> (&chunk));

  // some servers don't like requests that are made without a user-agent
  // field, so we provide one
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  err = curl_easy_perform(curl_handle);

  if (err) {
    err_info->assign(curl_errbuf);
  }
  curl_easy_cleanup(curl_handle);
  out->assign(chunk.memory);

  if (chunk.memory) free(chunk.memory);
  curl_global_cleanup();
  return err;
}
}  // namespace util
