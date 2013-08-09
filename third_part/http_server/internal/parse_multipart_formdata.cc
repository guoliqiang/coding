// Copyright 2010 Yunrang Inc. All Rights Reserved.
// Author: shunpingye@yunrang.com (Shunping Ye)

#include "third_part/http_server/public/parse_multipart_formdata.h"

#include <string>
#include <map>
#include <vector>

#include "base/public/logging.h"
#include "third_part/http_server/public/http_server.h"
#include "third_part/url_util/public/url_util.h"

using namespace std;  // NOLINT
using namespace base;  // NOLINT

using url_util::ParseKvlist;
namespace util {
namespace http_server {

static inline string MyStringTrim(const string& in) {
  if (in.length() < 2)
    return "";
  return in.substr(1, in.length() - 2);
}

static string StringDeleteCRLF(const string& in) {
  char* dup = strdup(in.c_str());
  char* index = dup + strlen(dup) - 1;
  while (index != dup) {
    if (*index == '\n' && *(index-1) == '\r') {
      *index = '\0';
      *(index-1) = '\0';
      index -= 2;
    } else {
      break;
    }
  }
  string out(dup);
  free(dup);
  return out;
}

void ParseFormdata(const string& form,
                   map<string, string>* result) {
  char* data = strdup(form.c_str());
  char* pos = data;
  char* content = NULL;
  char* header = data;

  pos = strstr(pos, "\r\n\r\n");
  if (pos) {
    *pos = 0;
    content = pos + strlen("\r\n\r\n");
  }

  //  1,  处理头部,
  map<string, string> headers;
  ParseKvlist(header, ": ", '\n', &headers, NULL);
  map<string, string>::iterator iter = headers.begin();
  for (; iter != headers.end(); iter++) {
    result->insert(*iter);
  }

  //  2,  处理name 如果头部中，name="file",则特殊处理其filename
  map<string, string>::iterator it = headers.find("Content-Disposition");
  if (it != headers.end()) {
    char* index = const_cast<char*>(strstr(it->second.c_str(), "name="));
    //  解析出name，filename等
    map<string, string> name_and_others;
    ParseKvlist(index, "=", ';', &name_and_others, NULL);
    iter = name_and_others.begin();
    for (; iter != name_and_others.end(); iter++) {
      //  删除引号
      result->insert(make_pair(iter->first, MyStringTrim(iter->second)));
    }
  }
  //  3,  处理content
  result->insert(make_pair("content", StringDeleteCRLF(content)));
  iter = result->begin();
  free(data);
}

bool ParseMultipartFormdata(const string& post_content, const string& boundary,
                               map<string, map<string, string> >* result) {
  LOG(INFO) << "in ParseMultipartFormdata" << endl;
  VLOG(2) << "post content:\n" << post_content;
  //  1,解析出随机字符串
  char* body = strdup(post_content.c_str());
  string real_boundary("--");
  real_boundary.append(boundary);  //  作boundary前加"--"标志开始

  //  边界的结束
  string boundary_end("--");
  boundary_end.append(boundary);
  boundary_end.append("--");
  //  2,解析出各个form的边界
  vector<string> forms;
  char* start = body + real_boundary.length() + 2;
  char* end = NULL;
  while (1) {
    end = strstr(start+1, real_boundary.c_str());
    if (end == NULL) {
      VLOG(3) << "can not find boundary，maybe fail to parse HTTP request";
      return false;
    }
    VLOG(3) << "start = " << (start - body) << ",end = " << (end - body);
    string form(start, end);
    forms.push_back(form);
    if (*(end+real_boundary.length()) == '-'
        && *(end+ real_boundary.length() +1) == '-') {
      break;
    }
    start = end + real_boundary.length() + 2;
  }

  for (int i = 0 ; i < forms.size(); ++i) {
    map<string, string> ret;
    ParseFormdata(forms[i], &ret);
    result->insert(make_pair(ret["name"], ret));
  }
  free(body);
  return true;
}
}
}
