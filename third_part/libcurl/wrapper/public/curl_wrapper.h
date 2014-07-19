// Copyright 2014. All Rights Reserved.
// Author: Liqiang Guo(guoliqiang2006@gmail.com)
// Date  : 2014-07-18 18:47:24
// File  : curl_wrapper.h
// Brief :

#ifndef  CURL_WRAPPER_H_
#define  CURL_WRAPPER_H_

#include <string>
#include "third_part/libcurl/include/curl/curl.h"
#include "third_part/libcurl/include/curl/easy.h"
#include "base/public/logging.h"

namespace curl {

class CurlWrapper {
 public:
	CurlWrapper(int size = 500000);
	~CurlWrapper();

	bool HttpPost(const std::string & url, const std::string & data,
      int64 timeout, std::string * rs);
	bool HttpGet(const std::string & url, int64 timeout, std::string * rs);
	void Reset();
	void ClearHttp();

 private:
	bool HttpPost(const std::string & url, const std::string & data,
      int64 timeout);
	bool HttpGet(const std::string & url, int64 timeout);
	void AppendHttpHead(const std::string & buf);
	static int HttpCallback(void * contents, int size,
      int nmemb, void * user_ptr);
	int HttpProcessResult(void * contents, int size,
      int nmemb);

 public:
	CURL * http_curl_;
	curl_slist * http_head_list_;
	char * buffer_;
	int buffer_size_;
	int result_size_;
};
}  // namespace curl

#endif  //  CURL_WRAPPER_H_
