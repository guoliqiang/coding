// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#ifndef UTIL_HTTP_SERVER_PARSE_MULTIPART_FORMDATA_H_
#define UTIL_HTTP_SERVER_PARSE_MULTIPART_FORMDATA_H_

#include <string>
#include <map>

namespace util {
namespace http_server {

/*  解析一个form的数据,格式为
head1\r\n
head2\r\n
\r\n
body\r\n
*/
void ParseFormdata(const std::string& form,
                   std::map<std::string, std::string>* result);

/*  解析一个enctype="multipart/formdata"的数据,格式为
--boundary
form1
--boundary\r\n
form2
--boundary\r\n
form4
--boundary--\r\n
*/
bool ParseMultipartFormdata(const std::string& post_content,
          const std::string& boundary,
          std::map<std::string, std::map<std::string, std::string> >* result);
}  // namespace http_server
}  // namespace util

#endif  // UTIL_HTTP_SERVER_PARSE_MULTIPART_FORMDATA_H_
